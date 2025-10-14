/*
 * OrderSafeProcessorInterface.h
 *
 *  Created on: 26.10.2010
 *      Author: hakamasy
 */

#ifndef ORDERSAFEPROCESSORINTERFACE_H_
#define ORDERSAFEPROCESSORINTERFACE_H_

namespace mt{

template<class DataIn, class DataOut> class OrderSafeProcessorInterface {

public:
	virtual ~OrderSafeProcessorInterface(){};
	virtual DataOut run(DataIn) = 0;
};

}
#endif /* ORDERSAFEPROCESSORINTERFACE_H_ */
