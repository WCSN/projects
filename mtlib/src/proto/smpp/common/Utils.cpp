/*
 * Utils.cpp
 *
 *  Created on: 23.09.2010
 *      Author: hakamasy
 */

#include "mt/proto/smpp/common/Utils.h"

namespace mt {
namespace smpp {


uint8_t Utils::m_udhSmsId = 0;

std::vector<MessagePart>* Utils::segmentateSmsBody(const uint8_t* msg,
		uint32_t len) {

	std::vector<MessagePart>* vect = new std::vector<MessagePart>;
	MessagePart part;

	if (len <= MAXSMSLENGTH) {
		char* ptr = new char[MAXSMSLENGTH];
		memcpy(ptr, msg, len);
		part.first = ptr;
		part.second = len;
		vect->push_back(part);
	} else {

		char count = 1;
		unsigned int cur = 0, partLen;

		struct UdhHeader udh;
		size_t udhLen = sizeof(udh);
		udh.length = 0x05;
		udh.refNum = 0x00;
		udh.size = 0x03;
		udh.msgId = m_udhSmsId++;

		uint8_t endPart = 0;
		if(len % (MAXSMSLENGTH - 6)){
			endPart = 1;
		}

		udh.total = len / (MAXSMSLENGTH - 6) + endPart;
		//		m_logger->debug("total len is %d, part count is %x\n", len, udh.total);

		do {
			char* ptr = new char[MAXSMSLENGTH];
			udh.partNum = count++;
			memcpy(ptr, &udh, udhLen);

			if (cur + MAXSMSLENGTH - udhLen < len) {
				memcpy(ptr + udhLen, msg + cur, MAXSMSLENGTH - udhLen);
				partLen = MAXSMSLENGTH;
			} else {
				memcpy(ptr + udhLen, msg + cur, len - cur);
				partLen = len - cur + udhLen;
			}

			part.first = ptr;
			part.second = partLen;

			vect->push_back(part);
			cur += partLen - udhLen;

		} while (cur < len);
	}
	return vect;
}

}
}
