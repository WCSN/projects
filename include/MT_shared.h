#ifndef __FILE_MT_SHARED_H
#define __FILE_MT_SHARED_H

#include "MT_common.h"
#include "MT_config.h"
#include "MT_threads.h"
#include "MT_log.h"
#ifdef WIN32
	#pragma warning( disable : 4786 4503 )
#endif

class MT_Shared_Object
{
public:
   virtual ~MT_Shared_Object(){};
   virtual void init(void)=0;
};

typedef MT_map<MT_string, MT_Shared_Object*> MT_components;
//extern MT_components Component_map;

typedef MT_Shared_Object* MT_comp_ctor(Config &conf, const MT_string& section);
typedef void MT_comp_dtor(MT_Shared_Object* object, Config &conf, const MT_string& section);

void install_shared_comp(Config &conf, const MT_string& name);
MT_Shared_Object* get_shared_comp(const MT_string& name);
void delete_shared_comp(Config &conf, const MT_string& name);


class MT_reso
{
public:
  MT_reso(const Config &c, const MT_string& name): conf(c), section(name){};
  virtual ~MT_reso(){};
  virtual void init(void){};
  virtual void destroy(void){};
  virtual bool avail(void){return true;};
  virtual void allocate(void){};
  virtual void release(void){}
protected:
//   bool empty;
   const Config& conf;
   MT_string section;
};

template <class T> class MT_pool : public MT_Shared_Object
{
public:
	MT_pool<T>(int Total, const Config &conf, const MT_string& name):sem(Total)
	{
		   MT_string data=conf.Get(name,"Data");
		   MT_automutex m(guard,__FILE__,__LINE__);
		
		   for(int i=0; i<Total;i++)
		   {
		      T* res = new T(conf, data);
		      ress[res]= true;
		      free_res.push_back(res);
		   }
	}
	
	~MT_pool<T>()
	   {
	      MT_automutex m(guard,__FILE__,__LINE__);
	      for(typename MT_map<T*,bool>::iterator i=ress.begin(); i != ress.end(); i++)
	      {
	         i->first->destroy();
	         delete i->first;
	      }
	      ress.clear();
	   }
	
	void init(void)
	{
	   MT_automutex m(guard,__FILE__,__LINE__);
	   for(typename MT_map<T*,bool>::iterator i=ress.begin(); i != ress.end(); i++)
	   {
			i->first->init();
			i->second = true;
			MT_log(MT_DEBUG_3,"Initialized resource "<<i->first);
	   }
	}
	
	T* get(void)
	{
		  MT_log(MT_DEBUG_3,"Free resources in the Pool "<<free_res.size());
	      sem.wait();
	      {
		 	MT_automutex mm(stat_mutex,__FILE__,__LINE__);
		 	db_inuse++;
	      }
	      MT_log(MT_DEBUG_3,"REsource avialable. Free resources in the Pool "<<free_res.size());
	      MT_automutex m(guard,__FILE__,__LINE__);
	      //faster version of commented code below
	      for( typename MT_vector<T*>::iterator vci=free_res.begin(); vci!=free_res.end();){
	      	if( (*vci)->avail()){
	      		T* res = *vci;
	      		ress[ res ] = false;
	      		res->allocate();
	      		free_res.erase(vci);
	      		MT_log(MT_DEBUG_3, "Allocated resource: " << res <<" Free resources in the pool "<<free_res.size());                                       
	      		return res;
	      	} else {
	      		MT_log(MT_DEBUG_3, "Resource "<<*vci<<" is not active.");
	      		vci++;
	      	}
	      }
	      sem.post();
	      MT_automutex mmm(stat_mutex,__FILE__,__LINE__);
	      db_inuse--;
	      MT_exception("Cannot get resource from pool");
	      return 0;
	}
	
	void release (T* res)
	{
	      MT_automutex m(guard,__FILE__,__LINE__);
	      MT_log(MT_DEBUG_3, "Releasing resource: " << res);
	      //faster version of commnted code below
	      if( ress.end()==ress.find(res)){
	            MT_exception("Cannot release resource in the pool");
	      }
	      res->release();
	      ress[ res ] = true;
	      free_res.push_back(res);
	      sem.post();
	      MT_automutex mm(stat_mutex,__FILE__,__LINE__);
		  db_inuse--;
	      return;
	}

protected:
   MT_map<T*,bool> 	ress;
   MT_vector<T*>	free_res;

   MT_semaphore sem;
   MT_mutex guard;
   MT_pool<T>(int Total):sem(Total){};
};

typedef MT_pool<MT_reso> Pool;

class MT_autogetreso
{
	bool alocated;
public:
		MT_autogetreso(MT_pool<MT_reso> &x, bool doGet=true) : alocated(doGet), pool(x), res(0) {if(doGet) res = pool.get();};
		MT_reso* get(void){if(!alocated) res=pool.get(); alocated=true; return res;};
		~MT_autogetreso(){if(alocated)pool.release(res);};
private:
		MT_pool<MT_reso> &pool;
		MT_reso* res;
};

//================================================================== Thread pool
class MT_thread_pool;
class MT_thread_reso;

class MT_thread_params 
{
private:
	MT_thread_pool* 	parent_pool;
	MT_thread_reso* 	reso;

	MT_semaphore 		proc_sem;
	MT_semaphore 		sem;
	int 				func_posted; 

	MT_mutex			chng_mtx;
	MT_thread_func*		func;
	void* 				args;
	bool 				waiting;
	bool				active;
	
	char*				namesz;
	
public:	
	MT_thread_params( MT_thread_pool* _parent_pool, MT_thread_reso* _reso, MT_thread_func* _func=0, void* _args=0 ):
		parent_pool(_parent_pool),reso(_reso),proc_sem(1),sem(0),func_posted(0),func(_func),args(_args),waiting(true),active(true),namesz(0){}; 

	void setFunc( MT_thread_func* _func, void* _args, const char* namesz );
	void start();
	bool isWaiting();
	void deactivate();
private: //methods below are accesible only for MT_thread_flow
	bool isActive();
	void setWaiting(bool state);
	MT_thread_func* getFunc( );
	void* getArgs();

	friend void MT_thread_flow( MT_thread_params* p );
};


void MT_thread_flow( MT_thread_params* p );
//==================================================== class MT_thread_reso  ===
class MT_thread_reso: public MT_reso {
	MT_thread_params*	params;
	MT_thread_id		pid;
	MT_thread_pool*		parent;
	
public:	
	MT_thread_reso(MT_thread_pool* parent_pool, const Config &c, const MT_string& name): MT_reso(c,name){
		params = new MT_thread_params( parent_pool, this );
	};
	virtual ~MT_thread_reso(){
		delete params;
	};
	
	void process( MT_thread_func* _func, void* _args, const char* namesz );
	virtual void init(void);
	virtual void destroy(void);

	virtual bool avail(void){
		return params->isWaiting();
	};
};
//============================= class MT_pool<MT_thread_reso> MT_thread_pool ===
class MT_thread_pool: public MT_pool<MT_thread_reso>
{
public:
	virtual void release (MT_thread_reso* res){
		MT_exception("THREAD CAN'T be released until finished. It is self releasing resource.");
	}; //thread can't be released unlil it's finished
	MT_thread_pool(int Total, const Config& conf, const MT_string& name);
private:
	void finished( MT_thread_reso* res);
	friend void MT_thread_flow( MT_thread_params* p );
};

/*
#ifdef __cplusplus
extern "C"
{
#endif

MT_Shared_Object* sample_ctor(Config &conf, const MT_string& section);

#ifdef __cplusplus
}
#endif
*/

#endif //__FILE_MT_SHARED_H
