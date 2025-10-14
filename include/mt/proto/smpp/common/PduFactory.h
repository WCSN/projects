/*
 * PduFactory.h
 *
 *  Created on: 17.08.2010
 *      Author: hakamasy
 */

#ifndef PDUFACTORY_H_
#define PDUFACTORY_H_

#include "mt/logger/Logable.h"
#include "mt/proto/smpp/pdu/smpp.hpp"
#include "mt/proto/smpp/common/DataTypes.h"
#include "mt/proto/smpp/common/SMPPHelper.h"
namespace mt {

namespace smpp {

class PduFactory: public Logable {
	SMPPHelper* smppHelper;
	std::string m_serviceType;
public:

	void setServiceType(const std::string &str) {
		m_serviceType = str;
	}

	PduFactory();
	Smpp::SubmitSm *createSubmitSm(const std::string &to,
			const std::string &from, MessagePart part, CodingScheme cs,
			uint8_t registeredDelivery, uint8_t);

	Smpp::DeliverSm* createDeliverSm(const std::string &to,
			const std::string &from, const std::string &body, uint8_t);

};

}
}

#endif /* PDUFACTORY_H_ */
