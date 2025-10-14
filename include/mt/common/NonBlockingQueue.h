#ifndef NONBLOCKINGQUEUE_H
#define NONBLOCKINGQUEUE_H

#include <queue>
#include "mt/thread/AutoMutex.h"
#include "mt/common/Exception.h"

namespace mt {

	/**
	 * @class NonBlockingQueue mt/common/NonBlockingQueue.h
	 * @brief Thread safe simple queue.
	 */

	template <class T>
		class NonBlockingQueue {

	public:
		NonBlockingQueue();
		virtual ~NonBlockingQueue();

		bool empty();
		size_t size();
		void push(T);

		/**
		 * Pop front element from the queue.
		 * @throws mt::Exception if the queue is empty.
		 */
		T pop();

	private:
		std::queue<T> *m_queue;
		Mutex m_queueMutex;
	};

	template<class T>
		NonBlockingQueue<T>::~NonBlockingQueue() {
		delete m_queue;
	}

	template<class T>
		NonBlockingQueue<T>::NonBlockingQueue() {
		m_queue = new std::queue<T>;
	}

	template<class T>
		bool NonBlockingQueue<T>::empty() {
		AutoMutex a(m_queueMutex);
		return m_queue->empty();
	}

	template<class T> 
		size_t NonBlockingQueue<T>::size() {
		AutoMutex a(m_queueMutex);
		return m_queue->size();
	}

	template<class T>
		void NonBlockingQueue<T>::push(T t) {
		AutoMutex a(m_queueMutex);
		m_queue->push(t);
	}

	template<class T>
		T NonBlockingQueue<T>::pop() {
		MT_EXCEPTION_ASSERT(!empty(), "Can't pop. The queue is empty.");

		AutoMutex a(m_queueMutex);
		T data = m_queue->front();
		m_queue->pop();
		return data;
	}
}


#endif /* NONBLOCKINGQUEUE_H */
