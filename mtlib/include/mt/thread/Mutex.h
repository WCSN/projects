#ifndef __THREAD__MUTEX__H
#define __THREAD__MUTEX__H

#include <pthread.h>

namespace mt {

class Mutex {

public:
	Mutex();
	Mutex(const Mutex& m);
	~Mutex();

	void lock();
	bool trylock();
	void unlock();

private:
  pthread_mutex_t m_mutex;
};

}

#endif /* __THREAD__MUTEX__H */
