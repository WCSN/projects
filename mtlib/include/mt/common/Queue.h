#ifndef __COMMON__QUEUE__H
#define __COMMON__QUEUE__H

#include "mt/common/NonBlockingQueue.h"
#include <semaphore.h>

namespace mt {

/*!
 *  \class Queue mt/common/Queue.h
 *  \brief Thread safe blocked queue.
 */

template<typename T>
	class Queue : public NonBlockingQueue<T> {
	sem_t m_queueSemaphore;

public:
	void push(T);
	/**
	 * Pop front element from the queue.
	 * If the queue is empty, current thread will be blocked.
	 */
	T pop();
	Queue();
	virtual ~Queue();
};

template<typename T> Queue<T>::~Queue() {
}

template<typename T> Queue<T>::Queue() {
	::sem_init(&m_queueSemaphore, 0, 0);
}

template<typename T>
void Queue<T>::push(T t) {
	NonBlockingQueue<T>::push(t);
	::sem_post(&m_queueSemaphore);
}

template<typename T>
T Queue<T>::pop() {

	while (true) {
		if (sem_wait(&m_queueSemaphore)) {
//			printf("SEM_WAIT was interrupted by a signal.\n");
			//TODO
			continue;
		}
		break;
	}

	return NonBlockingQueue<T>::pop();
}

}

#endif
