#ifndef __MT_COMMON_LIB__MT_SMART_PTR__H
#define __MT_COMMON_LIB__MT_SMART_PTR__H

#include "MT_ref_counter.h"
#include <iostream>
#include <malloc.h>

#ifdef __STATISTIC_SMART_PTRS
#include <TypeCounter.h>
#endif

template <class T>
class MT_smart_ptr
{
protected:
	T* ptr;
	MT_ref_counter *ref_count;

protected:
	void checkPtr() const;
	void delete_ptr();

public:
	MT_smart_ptr();
	MT_smart_ptr(const int value);
	MT_smart_ptr(T* _ptr);
	MT_smart_ptr(const T* _ptr);
	MT_smart_ptr(const MT_smart_ptr& _smart_ptr);
	MT_smart_ptr(T* _ptr, MT_ref_counter* ref_counter);
	~MT_smart_ptr();

public:
	MT_smart_ptr<T>& operator=(MT_smart_ptr<T>& src);
	MT_smart_ptr<T>& operator=(MT_smart_ptr<T> const & src);

	T& operator *() const;
	bool operator !() const;
	T* operator -> () const;

 	bool operator ==(const int i) const;
	bool operator !=(const int i) const;

	bool operator ==(const T* _ptr) const;
	bool operator !=(const T* _ptr) const;

	bool operator == (MT_smart_ptr<T>& src) const;
	bool operator == (MT_smart_ptr<T> const & src) const;

	bool operator != (MT_smart_ptr<T>& src) const;
	bool operator != (MT_smart_ptr<T> const & src) const;


	friend inline bool operator<(MT_smart_ptr<T> const & a, MT_smart_ptr<T> const & b)
	{
		return std::less<T* >() (a.ptr, b.ptr);
	}

	operator bool() const;
	operator int() const;
	T* data() const  	{	return ptr;	}
	const T* const_data() const  { return ptr; }
	MT_ref_counter * counter() const { return ref_count; }
	int count() const {return ref_count->current_count();}
	template<class Y>
		MT_smart_ptr<Y> cast();
};

template<class T>
MT_smart_ptr<T>::MT_smart_ptr() : ptr(NULL),
	ref_count(NULL)
{
#ifdef __STATISTIC_SMART_PTRS
	TypeCounter::addCounter(typeid(T).name());
#endif
}

template<class T>
MT_smart_ptr<T>::MT_smart_ptr(const int value) : ptr(NULL),
	ref_count(NULL)
{
#ifdef __STATISTIC_SMART_PTRS
	TypeCounter::addCounter(typeid(T).name());
#endif
}

template<class T>
MT_smart_ptr<T>::MT_smart_ptr(T* _ptr) : ptr(_ptr),
	ref_count(NULL)
{
	ref_count = new MT_ref_counter(1);
#ifdef __STATISTIC_SMART_PTRS
	TypeCounter::addCounter(typeid(T).name(), true);
#endif

}

template<class T>
MT_smart_ptr<T>::MT_smart_ptr(const T* _ptr) : ptr(_ptr),
	ref_count(NULL)
{
	ref_count = new MT_ref_counter(1);
#ifdef __STATISTIC_SMART_PTRS
	TypeCounter::addCounter(typeid(T).name(), true);
#endif
}

template<class T>
MT_smart_ptr<T>::MT_smart_ptr(const MT_smart_ptr<T>& src)
{
	ptr = src.ptr;
	ref_count = src.ref_count;
	if(ref_count)
		ref_count->increment();
#ifdef __STATISTIC_SMART_PTRS
	TypeCounter::addCounter(typeid(T).name());
#endif
}

template<class T>
MT_smart_ptr<T>::MT_smart_ptr(T* _ptr, MT_ref_counter* ref_counter) : ptr(_ptr),
	ref_count(ref_counter) {
	if(ref_count)
		ref_count->increment();
	}


template<class T>
MT_smart_ptr<T>::~MT_smart_ptr()
{
#ifdef __STATISTIC_SMART_PTRS
	TypeCounter::delCounter(typeid(T).name());
#endif
	if(ref_count && ref_count->decrement())
		delete_ptr();
}

template<class T>
MT_smart_ptr<T>& MT_smart_ptr<T>::operator = (MT_smart_ptr<T>& src)
{
	if(ref_count)
	{
		if(ref_count->decrement())
			delete_ptr();
	}
	ptr = src.ptr;
	ref_count = src.ref_count;
	if(ref_count)
		ref_count->increment();
	return *this;
}

template<class T>
MT_smart_ptr<T>& MT_smart_ptr<T>::operator = (MT_smart_ptr<T> const & src)
{
	if(ref_count)
	{
		if(ref_count->decrement())
			delete_ptr();
	}

	ptr = src.ptr;
	ref_count = src.ref_count;
	if(ref_count)
		ref_count->increment();
	return *this;
}

template<class T>
T& MT_smart_ptr<T>::operator * () const
{
	checkPtr();
	return *ptr;
}

template<class T>
bool MT_smart_ptr<T>::operator !() const
{
	return (ptr == NULL);
}

template<class T>
T* MT_smart_ptr<T>::operator -> () const
{
	checkPtr();
	return ptr;
}

template<class T>
bool MT_smart_ptr<T>::operator ==(const int i) const
{
	bool isEmpty = (ptr == NULL);
	return (i == 0) ? isEmpty : !isEmpty;
}

template<class T>
bool MT_smart_ptr<T>::operator !=(const int i) const
{
	return !(operator == (i));
}

template<class T>
bool MT_smart_ptr<T>::operator ==(const T* _ptr) const
{
	return (ptr == _ptr);
}

template<class T>
bool MT_smart_ptr<T>::operator !=(const T* _ptr) const
{
	return (ptr != _ptr);
}

template<class T>
bool MT_smart_ptr<T>::operator == (MT_smart_ptr<T>& src) const
{
	return (ptr == src.ptr);
}

template<class T>
bool MT_smart_ptr<T>::operator == (MT_smart_ptr<T> const & src) const
{
	return (ptr == src.ptr);
}

template<class T>
bool MT_smart_ptr<T>::operator != (MT_smart_ptr<T>& src) const
{
	return (ptr != src.ptr);
}

template<class T>
bool MT_smart_ptr<T>::operator != (MT_smart_ptr<T> const & src) const
{
	return (ptr != src.ptr);
}

template<class T>
MT_smart_ptr<T>::operator bool() const
{
	return (ptr != NULL);
}

template<class T>
MT_smart_ptr<T>::operator int() const
{
	return (ptr == NULL) ? 0 : 1;
}

template<class T>
void MT_smart_ptr<T>::checkPtr() const
{
	if(!ptr)
	{
#ifdef _RELEASE_BUILD
		MT_exception("Error access memory for '%s'", typeid(T).name());
#else
		std::cout << "ALARM! AHTUNG! Error access memory for '" << typeid(T).name() << "'. Process has been crashed while you read this message" << std::endl;
#endif
	}
}

template<class T>
void MT_smart_ptr<T>::delete_ptr()
{
#ifdef __STATISTIC_SMART_PTRS
	TypeCounter::decrementRealObjectsCount(typeid(T).name());
#endif
	delete ptr;
	delete ref_count;
}

template<class T>
template<class Y>
MT_smart_ptr<Y> MT_smart_ptr<T>::cast() {

	Y* new_ptr = dynamic_cast<Y*>(ptr);
	if(!new_ptr)
		return MT_smart_ptr<Y>();
	return MT_smart_ptr<Y>(new_ptr, ref_count);
}

template <class T, class Y>
class MT_polymorph_smart_ptr : public MT_smart_ptr<T>
{
public:
	MT_polymorph_smart_ptr(const MT_smart_ptr<Y>& smart_ptr);
};

template <class T, class Y>
	MT_polymorph_smart_ptr<T,Y>::MT_polymorph_smart_ptr(const MT_smart_ptr<Y>& smart_ptr) : MT_smart_ptr<T>()
{
	MT_smart_ptr<T>::ptr = ((T*)smart_ptr.data());
	MT_smart_ptr<T>::ref_count = smart_ptr.counter();
	if(MT_smart_ptr<T>::ref_count)
		MT_smart_ptr<T>::ref_count->increment();
}

#endif //__MT_COMMON_LIB__MT_SMART_PTR__H
