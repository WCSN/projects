#ifndef MT_THREADS_H
#define MT_THREADS_H

#ifdef WIN32
	#pragma warning( disable : 4786 )
#endif

#ifdef WIN32
	#include <windows.h>
	#include <process.h>
#else
	#include <pthread.h>
#include <sys/time.h>
#include <iostream>
#ifdef SOLARIS
	#include <synch.h>
#endif //SOLARIS
	#include <semaphore.h>
#endif //WIN32

//#include <stdio.h>

//#include <MT_log.h>
#include <MT_config.h>

#ifdef WIN32
	typedef void MT_thread_handler;
	typedef HANDLE MT_mutex_object ;
	typedef HANDLE MT_semaphore_object ;
	typedef unsigned long MT_thread_id;
#else
	typedef void* MT_thread_handler;
	typedef pthread_mutex_t MT_mutex_object ;
	typedef sem_t MT_semaphore_object ;
	typedef pthread_t MT_thread_id;
	#define MT_thread_join( pid ) pthread_join( pid, 0 ) 
	#define MT_thread_int( pid ) pthread_kill( pid, SIGINT )
	#define MT_thread_pid pthread_self 
#endif

typedef MT_thread_handler MT_thread_func(void *);

#define MT_MTX_DOLOCK 1
#define MT_MTX_DONOTLOCK 0
#define MT_SEM_DOWAIT 1
#define MT_SEM_DONOTWAIT 0

//Threads functions

extern MT_EXTERNAL_SYMBOL MT_thread_id MT_thread_create( MT_thread_func *func, void *arg=NULL, bool detached = false, const size_t = 0);
extern MT_EXTERNAL_SYMBOL int MT_thread_getid(void);

// Crossplatform mutex

class MT_mutex
{
public:

	MT_EXTERNAL_SYMBOL void lock(void);
	MT_EXTERNAL_SYMBOL bool trylock(void);
	MT_EXTERNAL_SYMBOL void unlock(void);
	MT_EXTERNAL_SYMBOL MT_mutex(void);
	MT_EXTERNAL_SYMBOL MT_mutex(const MT_mutex& m);
	MT_EXTERNAL_SYMBOL ~MT_mutex(void);

private:

  MT_mutex_object m;

};

#include "AutoMutexStat.h"

class MT_automutex
{
private:
	bool locked;	
	MT_mutex &m;
	//MT_string m_name;
	unsigned long m_lockTime;

public:
	MT_EXTERNAL_SYMBOL MT_automutex(MT_mutex& Mutex, char DoLock, const char* filename, int line):m(Mutex),
		m_lockTime(0)
		{ 
			/* char line_str[10]; */
			/* sprintf(line_str, "%d", line); */
			/* m_name += filename; */
			/* m_name += ":"; */
			/* m_name += line_str; */
			if (locked=(DoLock==MT_MTX_DOLOCK)) {
				//unsigned long before_lock  = currentTick();
				m.lock();    
				/* m_lockTime = currentTick(); */
				/* AutoMutexStat::instance()->addWaitLockTime(m_name, m_lockTime - before_lock); */
			}
		}

	MT_EXTERNAL_SYMBOL MT_automutex(MT_mutex& Mutex, const char* filename, int line): locked(true),
		m(Mutex),
		m_lockTime(0)
		{ 
			/* char line_str[10]; */
			/* sprintf(line_str, "%d", line); */
			/* m_name += filename; */
			/* m_name += ":"; */
			/* m_name += line_str; */
			/* unsigned long before_lock  = currentTick(); */
			m.lock();    
			/* m_lockTime = currentTick(); */
			/* AutoMutexStat::instance()->addWaitLockTime(m_name, m_lockTime - before_lock); */
		}

   MT_EXTERNAL_SYMBOL ~MT_automutex() {
		 if(locked) {
			 m.unlock();
			 //AutoMutexStat::instance()->addLockingTime(m_name, currentTick() - m_lockTime);
		 }
	 }

	 unsigned long currentTick() {
		 struct timeval tv;
		 gettimeofday(&tv, NULL);
		 return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	 }
};

extern unsigned int req_total, req_active, req_passed, db_inuse;
extern MT_mutex stat_mutex;

class MT_semaphore
{
	//unsigned int semaValue; //must be checked manually to avoid misoperation of wait by a signal
	//MT_mutex	svMtx;
public:

   MT_EXTERNAL_SYMBOL int wait(bool breakBySignal=false);
   MT_EXTERNAL_SYMBOL void post(void);
   MT_EXTERNAL_SYMBOL MT_semaphore(unsigned int Count);
   MT_EXTERNAL_SYMBOL ~MT_semaphore(void);

private:

  MT_semaphore_object s;

};

class MT_autosemaphore
{
public:
   MT_EXTERNAL_SYMBOL MT_autosemaphore(MT_semaphore& Sema, char DoWait=MT_SEM_DOWAIT):s(Sema)
      { if (DoWait)  s.wait();    }

   MT_EXTERNAL_SYMBOL ~MT_autosemaphore() {s.post();};
private:
   MT_semaphore &s;
};

/////////////////////////////////////////////////////////////////////////////
typedef enum {MT_READ, MT_WRITE} MT_rwlock_t;

class MT_rwmutex
{
private:
	MT_mutex read_mut;
	MT_semaphore write_sm;
	MT_mutex count_mut;
	int read_count;

public:
	MT_EXTERNAL_SYMBOL MT_rwmutex():write_sm(1),read_count(0){};

	MT_EXTERNAL_SYMBOL void lock( MT_rwlock_t rw );
	MT_EXTERNAL_SYMBOL void unlock( MT_rwlock_t rw );
};


class MT_autorwmutex
{
private:
	bool locked; 
public:
   MT_EXTERNAL_SYMBOL MT_autorwmutex(MT_rwmutex& Mutex, MT_rwlock_t rw_type, char DoLock=MT_MTX_DOLOCK):m(Mutex), tp(rw_type)
      {    if (locked=(MT_MTX_DOLOCK==DoLock))  m.lock(tp);    }

   MT_EXTERNAL_SYMBOL ~MT_autorwmutex() {if(locked) m.unlock(tp);};
private:
   MT_rwmutex &m;
	MT_rwlock_t tp;
};

/////////////////////////////////////////////////////////////////////////////

template <class A, class B> class MT_smap: public MT_map<A,B>
{
private:
	MT_rwmutex mx;
	MT_mutex global;
public:

	B& operator[](const A& a)
	{
		typename MT_map<A,B>::iterator i;
		{
			MT_autorwmutex m( mx, MT_READ );
			i = MT_map<A,B>::find(a);
		}
		if (i == end())
		{
			B b;
			pair<A,B> p(a,b);
			MT_autorwmutex m( mx, MT_WRITE );
			return MT_map<A,B>::insert(p).first->second;
		}
		else
			return i->second;
	};

	typename MT_map<A,B>::iterator find(const A& a)
	{
		MT_autorwmutex m( mx, MT_READ );
		return MT_map<A,B>::find( a );
	};

	void erase(typename MT_map<A,B>::iterator i)
	{
		MT_autorwmutex m( mx, MT_WRITE );
		MT_map<A,B>::erase( i );
	};

	void erase(const A& i)
	{
		MT_autorwmutex m( mx, MT_WRITE );
		MT_map<A,B>::erase( i );
	};

	MT_rwmutex& get_mutex(void)
	{
		return global;
	}

	const typename MT_map<A,B>::const_iterator begin(void)
	{
		return MT_map<A,B>::begin();
	}

	const typename MT_map<A,B>::const_iterator end(void)
	{
		return MT_map<A,B>::end();
	}

	bool empty(void)
	{
		MT_autorwmutex m( mx, MT_READ );
		return MT_map<A,B>::empty();
	}

	void clear(void)
	{
		MT_autorwmutex m( mx, MT_WRITE );
		MT_map<A,B>::clear();
	}
	
	
/*	MT_map<A,B>::iterator erase( MT_map<A,B>::iterator it )
	{
		MT_automutex m( mx ,__FILE__,__LINE__);
		return MT_map<A,B>::erase( it );
	};*/

};

void sigpipehandler(int signum);

#endif
