/*
 * PduLogHelper.h
 *
 *  Created on: 11.02.2010
 *      Author: hakamasy
 */

#ifndef PDULOGHELPER_H_
#define PDULOGHELPER_H_
#include "mt/proto/smpp/pdu/smpp.hpp"
#include <string>
namespace mt{

namespace smpp{

class PduLogHelper{


public :

	static std::string LogDeliverSm(Smpp::DeliverSm* dm);
	static std::string LogDeliverSmResp(Smpp::DeliverSmResp* dmr);

	static std::string LogSubmitSm(Smpp::SubmitSm*);
	static std::string LogSubmitSmResp(Smpp::SubmitSmResp*);

	static std::string LogBindTvResp(Smpp::BindTransceiverResp* btr);
	static std::string LogBindTv(Smpp::BindTransceiver* btr);

	static std::string LogBindTmResp(Smpp::BindTransmitterResp* btr);
	static std::string LogBindTm(Smpp::BindTransmitter* btr);

	static std::string LogEnqLink(Smpp::EnquireLink* enq);
	static std::string LogEnqLinkResp(Smpp::EnquireLinkResp* enqr);

	static std::string LogUnbind(Smpp::Unbind* enq);
	static std::string LogHdr(Smpp::Header* hdr);


};
}
}

#endif /* PDULOGHELPER_H_ */
