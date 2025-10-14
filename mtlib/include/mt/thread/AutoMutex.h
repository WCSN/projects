#ifndef __THREAD__AUTO_MUTEX__H
#define __THREAD__AUTO_MUTEX__H

#include "mt/thread/Mutex.h"

namespace mt {

	class AutoMutex {

	public:
	    AutoMutex(Mutex& mutex) : m_mutex(mutex) { m_mutex.lock(); }

	    ~AutoMutex() { m_mutex.unlock(); }

	private:
		AutoMutex(const AutoMutex& autoMutex);
		AutoMutex& operator=(const AutoMutex& autoMutex);

		Mutex &m_mutex;
	};
}

#endif /* __THREAD__AUTO_MUTEX__H */
