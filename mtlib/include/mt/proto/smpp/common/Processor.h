#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <map>

#include "mt/common/Queue.h"

#include "mt/thread/AutoMutex.h"
#include "mt/thread/Mutex.h"

#include "mt/thread/InfiniteThread.h"

#include "mt/proto/smpp/common/DataTypes.h"
#include "mt/logger/Logable.h"
#include "mt/proto/smpp/pdu/smpp.hpp"
#include "mt/proto/smpp/common/Codec.h"
#include "mt/data/mysql/MysqlConnector.h"
#include "mt/proto/smpp/common/Reciver.h"
#include "mt/proto/smpp/common/Sender.h"
#include "mt/proto/smpp/common/SMPPHelper.h"


namespace mt {

namespace smpp {
/*
 struct ltstr {
 bool operator()(char* s1, char* s2) const {
 return strcmp(s1, s2) < 0;
 }
 };*/

class Connection {
public:
	BindType type;
	Sender* sender;
	Reciver* reciver;
	Queue<Data*>* in;
	Queue<Data*>* out;
	~Connection();
	Connection(BindType t, Queue<Data*> *q);

};

class ClientConnection {

public:
	std::string systemId;
	Connection* rv;
	Connection* tx;
	Connection* tm;
	ClientConnection() {
		rv = NULL;
		tx = NULL;
		tm = NULL;
	}

	void SetConnection(BindType type, Connection* conn) {

		switch (type) {
		case bindReciever:
			rv = conn;
			break;
		case bindTranciever:
			tx = conn;
			break;
		case bindTransmiter:
			tm = conn;
			break;

		}
	}

};

class Processor: public InfiniteThread {

protected:

	//system id paramer. use only for detail debug log output
	std::string m_systemId;

	//connection type paramer(RX TX TV SRV). use only for detail debug log output
	std::string m_connectionType;

	SMPPHelper *smppHelper;
	MysqlConnector* mysql;
	LoggerPtr hexLogger;
	LoggerPtr CDRlogger;

	std::map<uint32_t, Request*> m_reqMap;
	std::map<std::string, Request*> m_mpMap;

	Queue<Data*>* inQ;
	Queue<Data*>* outQ;
	Queue<Data*>* userQ;
	unsigned long seqNum;
	int requiestId;
	unsigned int stepReqId;

	Mutex seqNumMtx;
	Mutex requestIdMtx;

public:


	Processor();


	virtual ~Processor() {

	}


	int ProcessMP(Data* data);

	//for submit sm response
	Request* GetRequestBySN(uint32_t);
	void setRequestBySN(uint32_t, Request*);

	//for concat sms
	Request* GetRequestByMsisdn(Data*);
	void RemoveRequestByMsisdn(const std::string &to);

	void setInQ(Queue<Data*>* q) {
		inQ = q;
	}

	void setOutQ(Queue<Data*>* q) {
		outQ = q;
	}

	void setUserQ(Queue<Data*>* q) {
		userQ = q;
	}


	Queue<Data*>* GetInQ() {
		return inQ;
	}

	Queue<Data*>* GetOutQ() {
		return outQ;
	}

	Queue<Data*>* GetUserQ() {
		return userQ;
	}


	unsigned long GetSeqNum() {
		AutoMutex a(seqNumMtx);
		return seqNum++;
	}

	unsigned long GetRequestId() {
		unsigned int ret;
		AutoMutex a(requestIdMtx);
		ret = requiestId;
		requiestId += stepReqId;
		return ret;
	}

	void SetRequestId(unsigned id) {
		AutoMutex a(requestIdMtx);
		requiestId = id;
	}

	void SetStepReqId(unsigned int i) {
		AutoMutex a(requestIdMtx);
		stepReqId = i;
	}


	void setSystemId(const std::string &si) {
		m_systemId = si;
		MT_LOG_SETPREFIX(m_systemId + ":" + m_connectionType);
	}

	void setConnectionType(const std::string &c) {
		m_connectionType = c;
		MT_LOG_SETPREFIX(m_systemId + ":" + m_connectionType);
	}
};

}
}
#endif /* CLIENTPROCESSOR_H_ */
