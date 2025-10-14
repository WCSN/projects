#ifndef __COMMON__SINGLETON__H
#define __COMMON__SINGLETON__H

#include <mt/common/SmartPtr.h>

namespace mt {
	/**
	 *  @class Singleton
	 *  @brief Singleton pattern.
	 *
	 * If you want to make sure that only the singleton instance of
	 * <T> is created, and that users cannot create their own
	 * instances of <T>, do the following to class <T>: \n
	 * <ul>
	 *   <li> Make the constructor of <T> private (or protected) </li>
	 *   <li>Make Singleton a friend of <T></li>
	 * </ul>
	 * Here is an example:
	 * @code
	 * class Foo {
	 *   friend class mt::Singleton<Foo>;
	 * private:
	 *   foo () { cout << "foo constructed" << endl; }
	 *   ~foo () { cout << "foo destroyed" << endl; }
	 * };
	 * typedef mt::Singleton<Foo> FooSingleton;
	 * @endcode
	 *
	 */

	template <class T>
		class Singleton {

	public:
		/**
		 * Get instance. The first call will create the object.
		 *
		 * @return Smart pointer to instance of the class.
		 */
		static T* instance();

		/**
		 * Release instance. After call the method the pointer will be deleted and set to NULL.
		 *
		 * <b>Note</b>: if after release() call instance() than second instance will be created.
		 */
		static void release();

	protected:
		static T* m_instance;
	};

	template <class T>
		T* Singleton<T>::m_instance = NULL;

	template <class T>
		T* Singleton<T>::instance() {
		if(!m_instance) {
			m_instance = new T();
		}
		return m_instance;
	}

	template <class T>
		void Singleton<T>::release() {
		if(m_instance) {
			delete m_instance;
			m_instance = NULL;
		}
	}

}


#endif /* __COMMON__SINGLETON__H */
