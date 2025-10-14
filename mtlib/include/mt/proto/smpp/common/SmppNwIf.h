/*
 * SmppNwIf.h
 *
 *  Created on: 11.11.2009
 *      Author: hakamasy
 */

#ifndef SMPPNWIF_H_
#define SMPPNWIF_H_

#include <string>
#include <pthread.h>

#include "mt/network/TCPSocket.h"

#include "mt/logger/Logable.h"

#include "mt/proto/smpp/common/DataTypes.h"
#include "mt/proto/smpp/pdu/smpp.hpp"

#include "mt/thread/Mutex.h"

#define RECBBUFLEN 1024

namespace mt {

namespace smpp {


union _RcvBuf{
	uint8_t buf[RECBBUFLEN];
	struct commonHeader hdr;
};

typedef union _RcvBuf RcvBuf;


class SmppNwIf: virtual public Logable {

protected:
	RcvBuf* m_rcvBuf;

	uint32_t m_maxRps;
	uint32_t m_sendTimeout;
	std::string m_systemId;
	std::string m_connectionType;


	bool notifyInspector;
	TCPSocket* sock;
	bool hexLogEnable;
	bool smppLogEnable;
	LoggerPtr CDRlogger;
	BindType type;
	LoggerPtr hexLogger;
	LoggerPtr smppLogger;
	bool deletInactive;
	bool active;
	Mutex activeMtx;
	Mutex deletInactiveMtx;
	void LogPduHex(bool rcv, const uint8_t* const ptr, unsigned int len);
	void LogPdu(Smpp::Header* h, bool rcv = false);
	void LogSmResp(Smpp::SubmitSmResp* smr, bool rcv);
	void LogSm(Smpp::SubmitSm* sm, bool rcv);

public:

	void setMaxRps(uint32_t max) {
		m_maxRps = max;
		m_sendTimeout = 1000000 / m_maxRps;
	}

	virtual ~SmppNwIf();

	void SetBindType(const BindType &t) {
		type = t;
	}

	BindType GetBindType() {
		return type;
	}

	void setSystemId(const std::string &si) {
		m_systemId = si;
		MT_LOG_SETPREFIX(m_systemId + ":" + m_connectionType);
	}

	void setConnectionType(const std::string &c) {
		m_connectionType = c;
		MT_LOG_SETPREFIX(m_systemId + ":" + m_connectionType);
	}

	std::string GetSystemId() {
		return m_systemId;
	}

	TCPSocket* GetSocket() {
		return sock;
	}

	bool IfInactive();
	void SetIfInactive(bool);

	SmppNwIf();

	bool IsActive();
	void Activate();
	void Deactivate();

	int SendPdu(TCPSocket* s, Smpp::Header* h);


	Smpp::Header* RecvPdu(TCPSocket* s);
	int SendPdu(Smpp::Header* h) {
		return SendPdu(sock, h);
	}
	Smpp::Header* RecvPdu() {
		return RecvPdu(sock);
	}

};

}
}

#endif /* SMPPNWIF_H_ */
