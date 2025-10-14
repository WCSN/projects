/*
 * OrderSafeProcessor.h
 *
 *  Created on: 26.10.2010
 *      Author: hakamasy
 */

#ifndef ORDERSAFEPROCESSOR_H_
#define ORDERSAFEPROCESSOR_H_


#include "mt/common/OrderSafeProcessorHandler.h"


namespace mt{


template<class DataIn, class DataOut> class OrderSafeProcessor {

	mt::SmartPtr<OrderSafeProcessorInterface<DataIn, DataOut> > m_interface;
	uint32_t m_threadCount;

	uint32_t m_sn;
	Mutex m_snMtx;

	void (*m_overflowCallback)();

	Queue<std::pair<uint32_t, DataIn> > *m_inQueue;
	OrderSafeQueue<DataOut> *m_outQueue;
	std::vector<OrderSafeProcessorHandler<DataIn, DataOut>*> m_handlers;

	uint32_t getSn(){
		AutoMutex a(m_snMtx);
		return m_sn++;

	}

protected:
	virtual mt::SmartPtr<OrderSafeProcessorInterface<DataIn, DataOut> > getInterface() {
		MT_EXCEPTION_ASSERT(m_interface, "No interface in Order safe Processor");
		return m_interface;
	}

public:
  OrderSafeProcessor(uint32_t c = 1) {
		m_threadCount = c;
		m_inQueue = new Queue<std::pair<uint32_t, DataIn> >;
		m_outQueue = new OrderSafeQueue<DataOut >;
		m_sn = 0;
	}

	OrderSafeProcessor(mt::SmartPtr<OrderSafeProcessorInterface<DataIn, DataOut> > procInterface, uint32_t c = 1){
		m_interface = procInterface;
		m_threadCount = c;
		m_inQueue = new Queue<std::pair<uint32_t, DataIn> >;
		m_outQueue = new OrderSafeQueue<DataOut >;
		m_sn = 0;
	}
	
	virtual ~OrderSafeProcessor() {
	}

	void start(){

		for(uint32_t i = 0; i< m_threadCount; i++ ){

			OrderSafeProcessorHandler<DataIn, DataOut> *handler = new OrderSafeProcessorHandler<DataIn, DataOut>;
			handler->setInQueue(m_inQueue);
			handler->setOutQueue(m_outQueue);
			handler->setInterface(getInterface());
			handler->start();
			m_handlers.push_back(handler);
		}
	}
	
	void enlargeOutputQueue(size_t sz){
		m_outQueue->enlarge(sz);
	}

	void push(DataIn data){
		m_inQueue->push(std::pair<uint32_t, DataIn> (getSn(), data));

	};
	DataOut pop(){

		return m_outQueue->pop();


	};


	void registerOverflowWatchdog(void (*callback)()){
		m_overflowCallback = callback;
	}

	void setOutpuQueueSize(size_t sz){
		m_outQueue->resize(sz);
	}

};



}


#endif /* ORDERSAFEPROCESSOR_H_ */

