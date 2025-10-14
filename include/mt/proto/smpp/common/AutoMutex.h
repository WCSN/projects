#ifndef AUTOMUTEX
#define AUTOMUTEX

#include "pthread.h"
#include "errno.h"
#include "stdio.h"
#include "string.h"
class AutoMutex {
	bool locked;
	pthread_mutex_t *mutex;
public :

	AutoMutex(pthread_mutex_t *m) : mutex(m) {
		while(pthread_mutex_lock( mutex )){
			printf("mutext lock return error %s\n", strerror(errno));
		}
		locked = true;

	};

	~AutoMutex() {
		if(locked) {
			pthread_mutex_unlock(mutex);
		}
	};
	void SetLockStatus(bool b) {locked = b;};
};

#endif


