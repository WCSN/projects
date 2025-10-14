/*
 * SMPPClient.h
 *
 *  Created on: 28.10.2009
 *      Author: hakamasy
 */

#ifndef SMPPCLIENT_H_
#define SMPPCLIENT_H_

#include "mt/common/XmlConfig.h"

#include "mt/proto/smpp/common/Sender.h"
#include "mt/proto/smpp/common/Reciver.h"
#include "mt/proto/smpp/common/Codec.h"
#include "mt/proto/smpp/common/DataTypes.h"

#include "mt/proto/smpp/client/ClientProcessor.h"
#include "mt/proto/smpp/client/ClientInspector.h"

#include "mt/logger/Logable.h"

#include "mt/data/mysql/MysqlConnector.h"

#include "mt/common/Queue.h"

namespace mt {

using namespace smpp;

class SMPPClient: public Logable {

	smpp::PduFactory* m_pduFactory;
	unsigned int m_startReqId;
	unsigned int m_stepReqId;
	std::string m_systemId;
	std::string m_connectionType;

	MysqlConnector* m_mysql;
	smpp::CodecPtr m_codecToUcs2;
	smpp::CodecPtr m_codecTo8bit;
	smpp::CodecPtr m_codecTo7bit;
	std::string m_serviceType;
	smpp::ClientProcessor* m_proc;
	Queue<Data*>* inQ;
	Queue<Data*>* outQ;
	Queue<Data*>* userQ;
	smpp::Inspector* insp;
	uint32_t GetReqId();

public:
	std::string getSystemId() {
		return m_systemId;
	}

	void setStartReqId(unsigned int s) {
		m_startReqId = s;
	}

	void setStepReqId(unsigned int s) {
		m_stepReqId = s;
	}

	bool isActive();

	~SMPPClient();
	void close();
	SMPPClient();

	int init(XmlConfigPtr cfg);

	//return pointer to msg or NULL if error occured; blocking function
	Message* recv();

	//return message ID if SUCCES
	//return -1 if error
	int send(Message*);

};

}

#endif /* SMPPCLIENT_H_ */
