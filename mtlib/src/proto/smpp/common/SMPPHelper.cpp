/*
 * SMPPHelper.cpp
 *
 *  Created on: 15.02.2010
 *      Author: hakamasy
 */
#include "mt/proto/smpp/common/SMPPHelper.h"

namespace mt {

namespace smpp {

SMPPHelper::SMPPHelper() {

	MT_LOGINIT(SMPPHelper)

}

int32_t SMPPHelper::init() {
	if (!codecFromUCS2)
		codecFromUCS2 = CodecFactory::getCodec("UCS-2BE", "UTF-8");

	return 0;
}

Smpp::SmeAddress SMPPHelper::createSmeAdress(const std::string &addr) {

	Smpp::Npi npi;
	Smpp::Npi ton;
	Smpp::Address smpp_addr(addr);
	bool alphanum = false;

	for (unsigned int i = 0; i < addr.length(); i++) {
		if (addr[i] > 57 || addr[i] < 48) {
			alphanum = true;
			break;
		}
	}

	if (alphanum) {
		npi = Smpp::Npi::Unknown;
		ton = Smpp::Ton::Alphanumeric;
	} else {
		npi = Smpp::Npi::E164;
		if (addr.length() < 11) {
			ton = Smpp::Ton::Unknown;
		} else {
			ton = Smpp::Ton::International;
		}
	}

	return Smpp::SmeAddress((const Smpp::Ton&) ton, (const Smpp::Npi&) npi,
			smpp_addr);
}

int32_t SMPPHelper::getMessage(Smpp::SubmitSm* pdu, std::string& body) {

	size_t indx = 0;
	CodecPtr cdc;

	Smpp::ShortMessage sm = pdu->short_message();

	switch (pdu->data_coding()) {
	case 0x08:
		cdc = codecFromUCS2;
		break;

	}

	std::vector<Smpp::Uint8> smv = sm;


	if (pdu->esm_class() & 0x40) {
		indx = 6;
	}

	if ( msgToString(smv, indx, cdc, body) < 0)
		return -1;

	return 0;
}

int32_t SMPPHelper::getMessage(Smpp::DeliverSm* pdu, std::string &body) {


	size_t indx = 0;
	CodecPtr cdc;

	Smpp::ShortMessage sm = pdu->short_message();

	switch (pdu->data_coding()) {
	case 0x08:
		cdc = codecFromUCS2;
		break;

	}

	std::vector<Smpp::Uint8> smv = sm;


	if (pdu->esm_class() & 0x40) {
		indx = 6;
	}

	if ( msgToString(smv, indx, cdc, body) < 0)
		return -1;

	return 0;
}

int32_t SMPPHelper::msgToString(const std::vector<Smpp::Uint8>& smv,
		const size_t offset, const CodecPtr& cdc, std::string& body) {

	size_t j = 0;
	char buf[256];

	if (smv.size() > 255) {
		MT_LOGWARNING("message body incorrect length: "<<smv.size())
		return -1;
	}

	size_t indx = offset;
	for (j = 0; indx < smv.size(); indx++, j++) {
		buf[j] = smv[indx];
	}
	buf[j] = '\0';

	if (cdc) {
		if (cdc->Encode(buf, j, body) < 0) {
			return -1;
		}
	} else {
		body.assign(buf);
	}

	return 0;
}



CodingScheme SMPPHelper::getCodingScheme(const Smpp::DataCoding &dc) {
	CodingScheme cs;

	switch (dc) {
	case 0x08:
		cs = UCS2;
		break;
	default:
		cs = DEFAULT;
	}
	return cs;

}


}
}
