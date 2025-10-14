/*
 * ClientReciver.h
 *
 *  Created on: 05.11.2009
 *      Author: hakamasy
 */

#ifndef RECIVER_H_
#define RECIVER_H_

#include "mt/network/TCPSocket.h"
#include "mt/common/Queue.h"
#include "mt/logger/Logable.h"

#include "mt/proto/smpp/common/SmppNwIf.h"
#include "mt/proto/smpp/common/DataTypes.h"

#include "mt/proto/smpp/pdu/smpp.hpp"
#include "mt/thread/InfiniteThread.h"

namespace mt {
namespace smpp {

class Inspector;

class Reciver: public SmppNwIf , public InfiniteThread {

	Queue<Data*>* m_queue;
	Inspector* inspector;
	int SendEnqLinkResp(Smpp::EnquireLink* pdu);
	std::string SendSubmitResp(Smpp::SubmitSm* pdu);
	int SendDeliverResp(Smpp::DeliverSm* pdu);


public:

	void stop();

	virtual ~Reciver();
	Reciver(Queue<Data*>*, Inspector* i);

	void SetSocket(TCPSocket* s);
	void runLoop();

};
}
}

#endif /* CLIENTRECIVER_H_ */
