/*
 * SMPPHelper.h
 *
 *  Created on: 15.02.2010
 *      Author: hakamasy
 */

#ifndef SMPPHELPER_H_
#define SMPPHELPER_H_

#include "mt/proto/smpp/pdu/smpp.hpp"
#include "mt/proto/smpp/common/Codec.h"
#include "mt/logger/Logable.h"
namespace mt {

namespace smpp {


class SMPPHelper : public Logable {
	CodecPtr codecFromUCS2;
	uint8_t m_udhSmsId;

	int32_t msgToString(const std::vector<Smpp::Uint8>&, const size_t offset, const CodecPtr&, std::string&);
	CodingScheme getCodingScheme(const Smpp::DataCoding &dc);

public:
	SMPPHelper();

	int32_t init();

	int32_t getMessage(Smpp::SubmitSm* sm, std::string &body);
	int32_t getMessage(Smpp::DeliverSm* pdu, std::string &body);

	Smpp::SmeAddress createSmeAdress(const std::string&);

	CodingScheme getCodingScheme(Smpp::SubmitSm* pdu) {
		return getCodingScheme(pdu->data_coding());
	}

	CodingScheme getCodingScheme(Smpp::DeliverSm* pdu) {
		return getCodingScheme(pdu->data_coding());
	}



};
}

}
#endif

