#ifndef __COMMON__REF_COUNTER__H
#define __COMMON__REF_COUNTER__H

#include <mt/thread/Mutex.h>

namespace mt {

	/*! \class RefCounter
		\brief References Counter
	*/

	class RefCounter {

	private:
		volatile int m_refCount;
		mt::Mutex m_mutex;

	public:
		RefCounter(const unsigned int _count);
		~RefCounter() {}

	public:
		/** 
		 * Increment references count.
		 */
		void increment();
		/** 
		 * Decrement references count.
		 * @return TRUE if need delete pointer.
		 */
		bool decrement();
		/** 
		 * @return Current count of references
		 */
		int current_count();
		bool null();
	};

}

#endif /* __COMMON__REF_COUNTER__H */
