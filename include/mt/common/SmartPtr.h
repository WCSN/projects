#ifndef __COMMON__SMART_PTR__H
#define __COMMON__SMART_PTR__H

#include <iostream>

#include <mt/common/RefCounter.h>
#include <mt/common/Exception.h>

namespace mt {
	/*!
	 * @class SmartPtr mt/common/SmartPtr.h
	 * @brief Pointer with references counting.
	 */

	template <class T>
		class SmartPtr {

	public:
		/** @name Constructors & Destructor. */
		//@{
		SmartPtr();
		SmartPtr(const int value);
		SmartPtr(T* _ptr);
		SmartPtr(const T* _ptr);
		SmartPtr(const SmartPtr& _smart_ptr);
		SmartPtr(T* ptr, RefCounter* ref_counter);
		~SmartPtr();
		//@}
	public:
		//@{
		SmartPtr<T>& operator=(SmartPtr<T>& src);
		SmartPtr<T>& operator=(SmartPtr<T> const & src);

		/**
		 * @throws mt::Exception if empty pointer m_ptr.
		 * @return Pointed data.
		 */
		T& operator *() const;
		bool operator !() const;
		/**
		 * @return Pointer to data.
		 */
		T* operator -> () const;

		bool operator ==(const int i) const;
		bool operator !=(const int i) const;

		bool operator ==(const T* _ptr) const;
		bool operator !=(const T* _ptr) const;

		bool operator == (SmartPtr<T>& src) const;
		bool operator == (SmartPtr<T> const & src) const;

		bool operator != (SmartPtr<T>& src) const;
		bool operator != (SmartPtr<T> const & src) const;


		friend inline bool operator<(SmartPtr<T> const & a, SmartPtr<T> const & b) {
			return std::less<RefCounter* >() (a.m_refCount, b.m_refCount);
		}
		//@}
		operator bool() const;
		operator int() const;
		T* data() const  	{	return m_ptr;	}
		const T* const_data() const  { return m_ptr; }
		/**
		 * @see RefCounter
		 * @return Pointer to references counter.
		 */
		RefCounter * counter() const { return m_refCount; }

		/**
		 * @return Current count of references.
		 */
		uint32_t count() const {return m_refCount->current_count();}

		/**
		 * Casting type T to type Y.
		 * 
		 * @return Smart pointer with Y type. If casting is fail, will returned null smart pointer.
		 */
		template<class Y>
			SmartPtr<Y> cast();

	protected:
		T* m_ptr;										/**< Pointer to data. */
		RefCounter* m_refCount;			/**< Pointer to references counter. */

	protected:
		void checkPtr() const;
		void deletePtr();
	};

	template<class T>
		SmartPtr<T>::SmartPtr() : m_ptr(NULL),
		m_refCount(NULL){
	}

	template<class T>
		SmartPtr<T>::SmartPtr(const int value) : m_ptr(NULL),
		m_refCount(NULL){
	}

	template<class T>
		SmartPtr<T>::SmartPtr(T* _ptr) : m_ptr(_ptr),
		m_refCount(NULL) {
			m_refCount = new RefCounter(1);
		}

	template<class T>
		SmartPtr<T>::SmartPtr(const T* _ptr) : m_ptr(_ptr),
		m_refCount(NULL) {
			m_refCount = new RefCounter(1);
		}

	template<class T>
		SmartPtr<T>::SmartPtr(const SmartPtr<T>& src) {
		m_ptr = src.m_ptr;
		m_refCount = src.m_refCount;
		if(m_refCount)
			m_refCount->increment();
	}

	template<class T>
		SmartPtr<T>::SmartPtr(T* ptr, RefCounter* ref_counter) : m_ptr(ptr),
		m_refCount(ref_counter) {
		if(m_refCount)
			m_refCount->increment();
	}

	template<class T>
		SmartPtr<T>::~SmartPtr() {
		if(m_refCount)
		{
			if(m_refCount->decrement())
				deletePtr();
		}
	}

	template<class T>
		SmartPtr<T>& SmartPtr<T>::operator = (SmartPtr<T>& src) {
			if(m_refCount)
			{
				if(m_refCount->decrement())
					deletePtr();
			}

			m_ptr = src.m_ptr;
			m_refCount = src.m_refCount;
			if(m_refCount)
				m_refCount->increment();
			return *this;
		}

	template<class T>
		SmartPtr<T>& SmartPtr<T>::operator = (SmartPtr<T> const & src) {
			if(m_refCount)
			{
				if(m_refCount->decrement())
					deletePtr();
			}

			m_ptr = src.m_ptr;
			m_refCount = src.m_refCount;
			if(m_refCount)
				m_refCount->increment();
			return *this;
		}

	template<class T>
		T& SmartPtr<T>::operator * () const {
		checkPtr();
		return *m_ptr;
	}

	template<class T>
		bool SmartPtr<T>::operator !() const {
		return (m_ptr == NULL);
	}

	template<class T>
		T* SmartPtr<T>::operator -> () const {
		checkPtr();
		return m_ptr;
	}

	template<class T>
		bool SmartPtr<T>::operator ==(const int i) const {
		bool isEmpty = (m_ptr == NULL);
		return (i == 0) ? isEmpty : !isEmpty;
	}

	template<class T>
		bool SmartPtr<T>::operator !=(const int i) const {
		return !(operator == (i));
	}

	template<class T>
		bool SmartPtr<T>::operator ==(const T* _ptr) const {
		return (m_ptr == _ptr);
	}

	template<class T>
		bool SmartPtr<T>::operator !=(const T* _ptr) const {
		return (m_ptr != _ptr);
	}

	template<class T>
		bool SmartPtr<T>::operator == (SmartPtr<T>& src) const {
		return (m_ptr == src.m_ptr);
	}

	template<class T>
		bool SmartPtr<T>::operator == (SmartPtr<T> const & src) const {
		return (m_ptr == src.m_ptr);
	}

	template<class T>
		bool SmartPtr<T>::operator != (SmartPtr<T>& src) const {
		return (m_ptr != src.m_ptr);
	}

	template<class T>
		bool SmartPtr<T>::operator != (SmartPtr<T> const & src) const {
		return (m_ptr != src.m_ptr);
	}

	template<class T>
		SmartPtr<T>::operator bool() const {
		return (m_ptr != NULL);
	}

	template<class T>
		SmartPtr<T>::operator int() const {
		return (m_ptr == NULL) ? 0 : 1;
	}

	template<class T>
		void SmartPtr<T>::checkPtr() const {
		if(!m_ptr)
		{
#ifdef NDEBUG
			MT_EXCEPTION(mt::Exception, "Error access memory for '" << typeid(T).name() << "'");
#else
			std::cout << "ALARM! AHTUNG! Error access memory for '" << typeid(T).name() << "'. Process has been crashed while you read this message" << std::endl;
#endif
		}
	}

	template<class T>
		template<class Y>
		SmartPtr<Y> SmartPtr<T>::cast() {

		Y* new_ptr = dynamic_cast<Y*>(m_ptr);
		if(!new_ptr)
			return SmartPtr<Y>();
		return SmartPtr<Y>(new_ptr, m_refCount);
	}

	template<class T>
		void SmartPtr<T>::deletePtr() {
		delete m_ptr;
		delete m_refCount;
	}

	/*! @class PolymorphSmartPtr
	 *  @brief Polymorphic pointer with references counting.
	 */
	template <class T, class Y>
		class PolymorphSmartPtr : public SmartPtr<T> {
	public:
		PolymorphSmartPtr(const SmartPtr<Y>& smart_ptr);
	};

	template <class T, class Y>
		PolymorphSmartPtr<T,Y>::PolymorphSmartPtr(const SmartPtr<Y>& smart_ptr) : SmartPtr<T>() {

		SmartPtr<T>::m_ptr = ((T*)smart_ptr.data());
		SmartPtr<T>::m_refCount = smart_ptr.counter();
		if(SmartPtr<T>::m_refCount)
			SmartPtr<T>::m_refCount->increment();
	}

} //namespace mt

#endif /* __COMMON__SMART_PTR__H */
