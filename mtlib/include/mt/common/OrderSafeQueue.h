/*
 * OrderSafeQueue.h
 *
 *  Created on: 26.10.2010
 *      Author: hakamasy
 */

#ifndef ORDERSAFEQUEUE_H_
#define ORDERSAFEQUEUE_H_

#include <vector>
#include <stdint.h>

#include "mt/thread/Mutex.h"
#include "mt/thread/AutoMutex.h"
#include <stdint.h>
#include <vector>


namespace mt {

template<class Data> class DataForOrderSafeQueue {

public:

	DataForOrderSafeQueue() {
		m_state = false;
	}

	Data m_data;
	bool m_state;

};

template<class Data> class OrderSafeQueue {

	std::vector<DataForOrderSafeQueue<Data> > m_queue;
	Mutex m_queueMtx;
	Mutex m_endMtx;

	uint32_t m_end;
	uint32_t m_beg;

	size_t m_size;

	void (*m_overflowCallback)();

	bool isLast(uint32_t i) {

		if (m_beg > m_end) {
			if (!((i > m_end) && (i< m_beg)))
				return true;
		} else {
			if ((i > m_beg) && (i<m_end))
				return true;
		}
		return false;

	}

public:

	OrderSafeQueue(size_t size = 256) {
		m_size = size;
		m_end = 0;
		m_beg = 0;
		m_queue.resize(m_size);
	}

	void enlarge(size_t new_sz) {

		if(new_sz <= m_size)
			return;

		AutoMutex a(m_queueMtx);

		if(m_beg >= m_end){
			m_queue.resize(new_sz);
			m_size = new_sz;
		}else{
			m_queue.resize(new_sz);
			uint32_t new_beg = 0;
			for(size_t i = 0, tmp = 0; i< m_beg; i++){
				if(i < new_sz - m_size){
					new_beg = m_size + i;
					m_queue[new_beg].m_data = m_queue[i].m_data;
					m_queue[new_beg].m_state = m_queue[i].m_state;
				}else{
					new_beg = tmp++;
					m_queue[new_beg].m_data = m_queue[i].m_data;
					m_queue[new_beg].m_state = m_queue[i].m_state;
				}
				m_queue[i].m_state = false;
			}

			m_beg = new_beg;
		}

	}

	void push(Data, uint32_t position);
	Data pop();
};

template<class Data> void OrderSafeQueue<Data>::push(Data data, uint32_t sn) {

	m_queueMtx.lock();

	uint32_t pos = sn % m_size;
	if(isLast(pos)){
		m_beg = pos;
	}
	m_queue[pos].m_data = data;
	m_queue[pos].m_state = true;

	m_queueMtx.unlock();
	m_endMtx.unlock();
}

template<class Data> Data OrderSafeQueue<Data>::pop() {

	Data ret;
	bool run = true;
	bool state = false;

	do {
		m_queueMtx.lock();
		if (m_end >= m_size) {
			m_end = 0;
		}

		ret = m_queue[m_end].m_data;
		state = m_queue[m_end].m_state;
		m_queue[m_end].m_state = false;
		if (state) {
			m_end++;
			run = false;
		}
		m_queueMtx.unlock();

		if (!state) {
			m_endMtx.lock();
		}

	} while (run);

	return ret;
}

}

#endif /* ORDERSAFEQUEUE_H_ */
