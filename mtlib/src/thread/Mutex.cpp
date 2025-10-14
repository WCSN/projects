#include <mt/thread/Mutex.h>
#include <mt/common/Exception.h>

namespace mt {

	Mutex::Mutex() {
		if(int error = ::pthread_mutex_init(&m_mutex,NULL))
			MT_EXCEPTION(mt::Exception, "Failed to create a MUTEX object Error no = " << error);
	}

	Mutex::Mutex(const Mutex& p) {
		if(int error = ::pthread_mutex_init(&m_mutex,NULL))
			MT_EXCEPTION(mt::Exception, "Failed to create a MUTEX object Error no = " << error);
	}

	Mutex::~Mutex() {
		::pthread_mutex_destroy(&m_mutex);
	}

	void Mutex::lock() {
		::pthread_mutex_lock(&m_mutex);
	}

	bool Mutex::trylock() {
		return ::pthread_mutex_trylock(&m_mutex) == 0;
	}

	void Mutex::unlock() {
		::pthread_mutex_unlock(&m_mutex);
	}

}
