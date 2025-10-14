/*
 * Sender.h
 *
 *  Created on: 13.11.2009
 *      Author: hakamasy
 */

#ifndef SMPP__SENDER__H__
#define SMPP__SENDER__H__

#include <time.h>

#include "mt/common/Queue.h"

#include "mt/logger/Logable.h"

#include "mt/network/TCPSocket.h"

#include "mt/proto/smpp/common/DataTypes.h"
#include "mt/proto/smpp/common/SmppNwIf.h"
#include "mt/proto/smpp/pdu/smpp.hpp"
#include "mt/thread/InfiniteThread.h"

namespace mt {

namespace smpp {

class Inspector;

class Sender: public SmppNwIf , public InfiniteThread {
	time_t lastActivity;
	Queue<Data*> *queue;
	Inspector* inspector;
	bool needEnqLink;

	int32_t m_sockFd;

public:
	void stop();
	void SendEnqLink(bool t) {
		needEnqLink = t;
	}


	virtual ~Sender();
	void SetSocket(TCPSocket* s) {
		sock = s;
		Activate();
	}

	Sender(Queue<Data*> * q, Inspector* i);

	void Send(Smpp::Header*);
	void runLoop();
};

}

}

#endif /* SENDER_H_ */
