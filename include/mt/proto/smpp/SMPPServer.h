/*
 * SMPPServer.h
 *
 *  Created on: 28.10.2009
 *      Author: hakamasy
 */

#ifndef SMPPSERVER_H_
#define SMPPSERVER_H_

#include "mt/common/XmlConfig.h"
#include "mt/common/Queue.h"

#include "mt/proto/smpp/server/ServerInspector.h"
#include "mt/proto/smpp/server/ServerProcessor.h"

#include "mt/logger/Logable.h"

#include "mt/proto/smpp/common/Sender.h"
#include "mt/proto/smpp/common/Reciver.h"
#include "mt/proto/smpp/common/Codec.h"
#include "mt/proto/smpp/common/DataTypes.h"

namespace mt{

using namespace smpp;

class SMPPServer : public Logable {
	char smsId;
	PduFactory *m_pduFactory;
	CodecPtr m_codecToUcs2;
	CodecPtr m_codecTo8bit;
	CodecPtr m_codecTo7bit;
	smpp::ServerInspector* servInsp;
	std::string serviceType;
	smpp::ServerProcessor* proc;
	Sender* sndr;
	Reciver* rcvr;
	Queue<Data*>* inQ;
	Queue<Data*>* outQ;
	Queue<Data*>* userQ;

	std::vector<std::string>* ProcessDeliverContext(std::string, std::string*);
/*
	Smpp::SubmitSm* CreateSubmitSm(const std::string &to, const std::string &to,
			const char* body, unsigned int len, bool multiP, CodingScheme cs,
			unsigned int);
*/
	std::vector<MessagePart>* SegmentateMsg(const char* msg,
			unsigned int len);

public:

	SMPPServer();
	virtual ~SMPPServer(){};
	/*
	 return
	 0 if success
	 -1 if error
	 */
	int init(XmlConfigPtr cfg);

	//return pointer to msg or NULL if error occured; blocking function
	Message* recv();

	//return 0 if SUCCESS, -1 otherwise
	int send(Message*);

};

}


#endif /* SMPPSERVER_H_ */
