/*
 * DataTypes.h
 *
 *  Created on: 28.10.2009
 *      Author: hakamasy
 */

#ifndef SMPP_DATATYPES_H_
#define SMPP_DATATYPES_H_

#include <string>
#include <vector>
#include "mt/proto/smpp/pdu/smpp.hpp"

namespace mt {
namespace smpp {

#define MAXSMSLENGTH 140

class PduFactory;

enum MessageType {
	SMS, SUBMIT_RESP, DELIVERY_REPORT, ENQ_LINK, UNBIND, USSD
};
enum CodingScheme {
	DEFAULT = 0x00, UTF8 = 0x02, ASCII = 0x01, UCS2 = 0x08
};
enum BindType {
	bindReciever, bindTransmiter, bindTranciever
};

typedef std::pair<char*, unsigned int> MessagePart;

struct commonHeader {

	unsigned int length;
	unsigned int cmd;
	unsigned int status;
	unsigned int seqN;

};

class Message {


public:


	Message() {
		registeredDelivery = 0;
		id = 0;
		status = 0;
		codingScheme = UCS2;
	}

	CodingScheme codingScheme;
	MessageType type;
	uint8_t registeredDelivery;
	uint32_t id;
	std::string context;
	std::string from;
	std::string to;
	std::string body;
	uint32_t status;
	std::string getSrcSystemId() {
		size_t pos = context.find(" ");
		return context.substr(0, pos);
	}

};

struct UdhHeader {
	char length;
	char refNum;
	char size;
	char msgId;
	char total;
	char partNum;
};

typedef std::pair<unsigned long, bool> PartStatus;

class Request {

	unsigned int status;
	unsigned int totalParts;
	unsigned int partsResp;
	bool m_needDeliverResp;
public:

	//	std::vector<unsigned int> parts;
	void setDeliverResp(bool b) {
		m_needDeliverResp = b;
	}
	bool needDeliverResp() {
		return m_needDeliverResp;
	}

	void SetTotalParts(unsigned int i) {
		totalParts = i;
	}

	unsigned int GetTotalParts() {
		return totalParts;
	}

	int HasResp(unsigned int s) {

		if (s != 0) {
			status = s;
		}
		partsResp++;
		return totalParts - partsResp;
	}

	std::vector<PartStatus>* parts;
	int id;
	Message* msg;

	Request(int i = 0, int c = 1) {
		id = i;
		status = 0;
		totalParts = c;
		partsResp = 0;
		parts = NULL;
		msg = NULL;
		m_needDeliverResp = false;
	}

	~Request() {
		if (parts != NULL) {
			delete parts;
		}
	}

	unsigned int GetStatus() {
		return status;
	}

	void SetStatus(unsigned int i) {
		status = i;
	}

	/*
	 bool PutPartStatus(unsigned long sn, int st) {

	 status = st;
	 bool notifyUser = true;

	 if (st != 0) {
	 return true;
	 }


	 if (parts != NULL) {
	 for (unsigned int i = 0; i < parts->size(); i++) {

	 printf("\n\n\nsn %u parts.first %u\n\n\n", sn, (*parts)[i].first);
	 if ((*parts)[i].first == sn) {
	 (*parts)[i].second = true;
	 }


	 if (!(*parts)[i].second) {
	 printf("\n\n\n\n %u false!\n\n\n\n", i);
	 notifyUser = false;
	 }
	 }
	 }

	 return notifyUser;
	 }
	 ;
	 */

};

class Data {

protected :
	uint8_t m_esmClass;

public:
	void esm_class(uint8_t i) {
		m_esmClass = i;
	}

	bool isNormalMessage() {

		if (!(m_esmClass & 0x3c))
			return true;
		return false;
	}

	bool terminate;
	pthread_mutex_t unbindMtx;
	unsigned char udhMsgId;
	unsigned char udhTotal;
	unsigned char udhPartNum;
	std::string systemId;
	std::string context;
	Request* req;
	Message* msg;
	int id;
	int lastSend;
	Smpp::Header* hdr;
	unsigned int length;
	bool isMultipart;
	bool fromUser;
	std::string msgId;
	int status;
	Data(Smpp::Header* h, bool direct = true) {
		status = 0;
		terminate = false;
		hdr = h;
		fromUser = direct;
		udhMsgId = 0;
		udhTotal = 0;
		udhPartNum = 0;
		lastSend = 0;
		length = 0;
		msg = NULL;
		req = NULL;
	}
	;

	Data() {
		status = 0;
		terminate = false;
		udhMsgId = 0;
		udhTotal = 0;
		udhPartNum = 0;
		lastSend = 0;
		hdr = NULL;
		length = 0;
		msg = NULL;
		fromUser = true;
		req = NULL;
	}
	;

	~Data() {

		if (hdr != NULL) {
			delete hdr;
		}
		if (req != NULL) {
			delete req;
		}
		if (msg != NULL) {
			delete msg;
		}

	}
	;
};

}
}

#endif /* DATATYPES_H_ */
