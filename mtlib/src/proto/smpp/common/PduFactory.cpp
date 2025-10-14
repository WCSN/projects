/*
 * PduFactory.cpp
 *
 *  Created on: 17.08.2010
 *      Author: hakamasy
 */



#include "mt/proto/smpp/common/PduFactory.h"
#include "mt/proto/smpp/common/SMPPHelper.h"

namespace mt{
namespace smpp{


PduFactory::PduFactory(){
	MT_LOGINIT(PduFactory)

	smppHelper = new SMPPHelper();
	smppHelper->init();

}


Smpp::SubmitSm *PduFactory::createSubmitSm(const std::string &to,
		const std::string &from, MessagePart part, CodingScheme cs = UCS2,
		uint8_t registeredDelivery = 0x00, uint8_t esmClass = 0x00){

	Smpp::SubmitSm* sm = NULL;
	try {
		sm = new Smpp::SubmitSm();
		//sm->service_type(serviceType.c_str());

		sm->source_addr(smppHelper->createSmeAdress(from));
		sm->destination_addr(smppHelper->createSmeAdress(to));

		sm->data_coding(cs);
		sm->registered_delivery(registeredDelivery);

		char* msgBody  = part.first;
		uint32_t msgBodyLen = part.second;

		std::cout<<"body length: "<<msgBodyLen<<std::endl;

		sm->esm_class(esmClass);

		sm->short_message((const Smpp::Uint8*) msgBody, msgBodyLen);
	} catch (std::exception e) {
		MT_LOGWARNING("can't create submit sm pdu. exception("<< e.what()<<")");
		delete sm;
		return NULL;
	}
	return sm;

}





Smpp::DeliverSm* PduFactory::createDeliverSm(const std::string &to,
		const std::string &from, const std::string &body, uint8_t esm_class) {
	Smpp::DeliverSm* sm = NULL;
	try {
		sm = new Smpp::DeliverSm();
		sm->service_type(m_serviceType.c_str());
		sm->esm_class(esm_class);
		sm->source_addr(smppHelper->createSmeAdress(from));
		sm->destination_addr(smppHelper->createSmeAdress(to));

		sm->registered_delivery(0x00000000);
		sm->short_message((const Smpp::Uint8*) body.c_str(), body.length());
		sm->insert_8bit_tlv(0x0427, 2);
		sm->insert_string_tlv(0x001e, std::string(body));
	}
	catch (std::exception e) {
		MT_LOGWARNING("can't create pdu. " << e.what())
		delete sm;
		return NULL;
	}
	return sm;
}




}
}
