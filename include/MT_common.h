#ifndef MT_COMMON_H
#define MT_COMMON_H

#ifdef WIN32
	#pragma warning( disable : 4786 4503 )
	#define MT_EXTERNAL_SYMBOL __declspec(dllexport)
	#define strtok_r(a,b,c) strtok(a,b)
#else
	#define MT_EXTERNAL_SYMBOL
	#include <typeinfo>
#endif

//MT_map definition
#include <algorithm>
#include <vector>

void MT_EXTERNAL_SYMBOL MT_exception(const char* Message,...);

using namespace std;

template <class A> class MT_vector: public vector<A>
{
public:
	MT_vector():std::vector<A>(){}
	MT_vector( const std::vector<A>& v ):std::vector<A>(v){}
	int lastOf( const A& value ) const {
		int index;
		for( index=this->size()-1;index>=0 && (value!=this->operator[](index)); index--);
		return index;
	};
	int firstOf( const A& value ) const {
		int index,sz=this->size();
		for( index=0;index<sz && (value!=this->operator[](index)); index++);
		return index==sz?-1:index;
	};
	int eraseValue(const A& value, bool all=true){
		for( typename vector<A>::iterator it=this->begin(); it!=this->end();){
			if( *it == value ){
				it = this->erase(it);
				if(!all) break;
			} else 
				it++;
		}
		return 0;
	};
	int pushNew(const A& value){
		if( -1 == lastOf(value) )
			this->push_back(value);
		else return 1;
		return 0;
	};
};
#ifdef WIN32
	template <class A, class B> class MT_map : public std::vector<std::pair<A,B> >
	{
		public:

		B& operator[](const A& a)
		{
			MT_map::iterator it = find( a );
			if( it != end() ) return it->second;

			B b;
			A a_cpy = a;
			pair<A,B> new_pair( a_cpy, b );

			push_back( new_pair );
			return ( end() - 1 )->second;
		};

		std::pair<iterator,bool> insert(std::pair<A,B> p,bool doCheck=true)
		{
			bool findRslt=false;
			if(doCheck){
				iterator it = find( p.first );
				if( it != end() ) return pair<iterator,bool>(it,true);
			}
			push_back( p );
			return pair<iterator,bool>(end() - 1,false);
		};

		typename MT_map::iterator find(const A& a)
		{
			for (MT_map::iterator i = begin(); i != end(); i++)
			{
				if (i->first == a)
					return i;
			}
			return end();
		};

		//MT_map::iterator& operator=( B b ){ this->second = b; }
	};
#else
	#ifdef __cplusplus
		#include <map>
        template <class A, class B> class MT_map : public map<A,B>{};
	#endif
#endif

#ifdef __cplusplus
  template <class T> class MT_memory
  {
  private:
  protected:
  public:
	
	MT_EXTERNAL_SYMBOL MT_memory(unsigned int Count=0):buf(0),len(Count){
		if(len){
			buf=(T*)malloc(Count*sizeof(T));
			if(0==buf) MT_exception("MT_memory: Failed to allocate %d units.", Count);
		}
	};
	
	MT_EXTERNAL_SYMBOL MT_memory<T>(const MT_memory<T>& src):buf(0),len(0){ operator=(src); };
	
	virtual MT_EXTERNAL_SYMBOL ~MT_memory(){
		if(buf){
			free((void*)buf);
		};
	}
	operator const T* () const {return buf;};
	
	operator T* (){return buf;};
	
	T* buf;	
	unsigned int len;

	unsigned int length() const {return len;};
	
	virtual unsigned int append( T* src, unsigned int size ){
		if(0==(buf=(T*)realloc((void*)buf,sizeof(T)*(len+size))))
			MT_exception("MT_memory<>::append Failed to allocate %d of type '%s'",len+size, typeid(T).name()); 
		else {
			memcpy( buf+len, src, size*sizeof(T));
			len += size;
		}
		return len;};
	
	virtual unsigned int append( MT_memory<T>& src ){ return append( src, src.len); };
	
	virtual MT_memory<T>& operator=(const MT_memory<T>& src){ 
		if(buf){
			 free((void*)buf);
		}
		len=src.len;
		buf=(T*)malloc(len*sizeof(T));
		if(!buf) MT_exception("MT_memory<>::failed to acclocate memory. Requested size=%d elements of %s",len, typeid(T).name());
		memcpy(buf,src.buf,len*sizeof(T));
		return *this;};

	virtual	unsigned int truncate( unsigned int startPos, int length=-1 ){
		if( startPos<0 || startPos>len ) MT_exception("MT_memory: Out of bounds.current length=%d but start=%d.", len, startPos );
		if( length!=-1 && startPos+length>len ) MT_exception("MT_memory: Out of bounds.Start %d + Length=%d exceeds current length of data %d", startPos, length, len );
		if( -1==length ) length = len - startPos;
		T* newbuf=(T*)malloc(length*sizeof(T));
		if(0==newbuf) MT_exception("MT_memory: Failed to allocate %d units of %s.", length,typeid(T).name());
		memcpy( newbuf, buf+startPos, length );
		free((void*)buf);
		buf = newbuf;
		len = length;
		return len;
	}
  };

  template <class T> class MT_memorybuf: public MT_memory<T>
  {
  unsigned int bufsize;
  const static unsigned int BUFINC = 32;

  public:

	MT_EXTERNAL_SYMBOL MT_memorybuf(unsigned int Count=0):MT_memory<T>(Count){
		if(Count && this->buf ) free((void*)this->buf);
		this->buf=(T*)malloc( (bufsize=(( this->len / BUFINC + 1 )* BUFINC )) * sizeof(T));
		if(!this->buf) MT_exception("MT_memorybuf<>::failed to acclocate memory. Requested size=%d elements of %s",(( this->len / BUFINC + 1 )* BUFINC ), typeid(T).name());
	};
	MT_EXTERNAL_SYMBOL MT_memorybuf<T>(const MT_memorybuf<T>& src){ 
		this->operator=(src); };
	MT_EXTERNAL_SYMBOL MT_memorybuf<T>(const MT_memory<T>& src){ 
		this->operator=(src); };

	virtual MT_EXTERNAL_SYMBOL ~MT_memorybuf(){
		free( (void*)this->buf );
		this->buf = 0;
	}

	virtual unsigned int append( T* src, unsigned int size ){
		if( bufsize < (this->len+size) )
			if(0==(this->buf=(T*)realloc((void*)this->buf,(bufsize=( (this->len + size)/ BUFINC + 1 )* BUFINC )*sizeof(T))))
			{
				MT_exception("Memmory allocation failed. Failed to allocate memory block with size %d !", bufsize );
			}
		 memcpy(this->buf+this->len*sizeof(T),src,size*sizeof(T));
		 this->len += size;
		 return this->len;};

	virtual unsigned int append_data( void* src, unsigned int size ){
		return append( (T*)src, size/sizeof(T) );
	}

	virtual unsigned int append( MT_memorybuf<T>& src ){ return append( src, src.len); };
	
	virtual MT_memorybuf<T>& operator=(const MT_memorybuf<T>& src){
		this->len=src.len;
		if(this->buf) free((void*)this->buf);
		this->buf=(T*)malloc( (this->bufsize=(( this->len / BUFINC + 1 )* BUFINC )) * sizeof(T));
		if(!this->buf) MT_exception("MT_memorybuf<>::failed to acclocate memory. Requested size=%d elements of %s",(( this->len / BUFINC + 1 )* BUFINC ), typeid(T).name());
		memcpy(this->buf,src.buf,this->len*sizeof(T)); return *this;};
	
	virtual MT_memorybuf<T>& operator=(const MT_memory<T>& src){
		this->len=src.len;
		if(this->buf) free((void*)this->buf);
		this->buf=(T*)malloc( (this->bufsize=(( this->len / BUFINC + 1 )* BUFINC )) * sizeof(T));
		if(!this->buf) MT_exception("MT_memorybuf<>::failed to acclocate memory. Requested size=%d elements of %s",(( this->len / BUFINC + 1 )* BUFINC ), typeid(T).name());
		memcpy(this->buf,src.buf,this->len*sizeof(T)); return *this;};
  };

#endif

////////////////////////////////////// helper
class auto_pointer{
	void** ptr;
public:
	auto_pointer( void** p):ptr(p){};
	~auto_pointer(){if(*ptr) free(*ptr);};
};
////////////////////////////////////// helper
template <class T> class MT_autopointer{
	T** ptr;
public:
	MT_autopointer( T** p):ptr(p){};
	~MT_autopointer(){if(*ptr) delete *ptr;};
};
////////////////////////////////////// helper
#include <MT_string.h>

/* class Printable{ */
/* protected:	 */
/* 	MT_memory<char> 	charBuf; */
/* public: */
/* 	virtual operator MT_string() const =0; */
/* 	virtual operator char*(){ */
/* 		return (char*)(operator const char*()); */
/* 	};	 */
/* 	virtual operator const char*() {  */
/* 	 	MT_string str = operator MT_string(); */
/* 	 	charBuf = MT_memory<char>( str.length() + 1); */
/* 	 	memcpy( charBuf, str.c_str(), str.length() ); */
/* 	 	charBuf.buf[ str.length() ] = 0; */
/* 	 	return charBuf; */
/* 	}; */
/* 	virtual ~Printable(){}; */
/* }; */

//iterators
#define CITERATE( ColType, ColName, IteratorName ) for( ColType::const_iterator IteratorName=ColName.begin();IteratorName!=ColName.end();IteratorName++) 
#define ITERATE( ColType, ColName, IteratorName )  for( ColType::iterator IteratorName=ColName.begin();IteratorName!=ColName.end();IteratorName++) 


#endif
