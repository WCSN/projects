/*
 * OrderSafeProcessorHandler.h
 *
 *  Created on: 26.10.2010
 *      Author: hakamasy
 */

#ifndef ORDERSAFEPROCESSORHANDLER_H_
#define ORDERSAFEPROCESSORHANDLER_H_

#include "mt/common/OrderSafeQueue.h"
#include "mt/common/OrderSafeProcessorInterface.h"

#include "mt/common/Queue.h"
#include "mt/thread/InfiniteThread.h"

namespace mt {

template<class DataIn, class DataOut> class OrderSafeProcessorHandler: public mt::InfiniteThread {

	Queue<std::pair<uint32_t, DataIn> > *m_inQueue;
	OrderSafeQueue<DataOut> *m_outQueue;
	mt::SmartPtr<OrderSafeProcessorInterface<DataIn, DataOut> > m_interface;
public:

	void setInQueue(Queue<std::pair<uint32_t, DataIn> > * q) {
		m_inQueue = q;
	}

	void setOutQueue(OrderSafeQueue<DataOut>* q) {
		m_outQueue = q;
	}

	void setInterface(const mt::SmartPtr<OrderSafeProcessorInterface<DataIn, DataOut> >& i) {
		m_interface = i;
	}

	OrderSafeProcessorHandler();

	void runLoop();

};

template<class DataIn, class DataOut> OrderSafeProcessorHandler<DataIn,
		DataOut>::OrderSafeProcessorHandler() {

	m_inQueue = NULL;
	m_outQueue = NULL;
}

template<class DataIn, class DataOut> void OrderSafeProcessorHandler<DataIn,
		DataOut>::runLoop() {

	std::pair<uint32_t, DataIn> p = m_inQueue->pop();
	uint32_t sn = p.first;
	DataIn dataIn = p.second;
	DataOut dataOut = m_interface->run(dataIn);
	m_outQueue->push(dataOut, sn);

}

}
#endif /* ORDERSAFEPROCESSORHANDLER_H_ */
