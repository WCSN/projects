/*
 * ServerProcessor.h
 *
 *  Created on: 01.12.2009
 *      Author: hakamasy
 */

#ifndef SMPP_SERVERPROCESSOR_H_
#define SMPP_SERVERPROCESSOR_H_

#include "mt/proto/smpp/common/Processor.h"
#include "mt/common/Queue.h"
#include <map>
#include "mt/thread/AutoMutex.h"
#include "mt/thread/Mutex.h"

/*
 class Sender;
 class Reciver;
 */

namespace mt {
namespace smpp {

class ServerProcessor: public Processor {

	std::map<std::string, ClientConnection*> clients;
	Mutex clientsMtx;

	void SendData(Data* data);
	std::string GetCurrentClientsInfo();

	static void* ConnectionsMonitor(void* p);
	CodecPtr m_codecFromUCS2;
public:

	void runLoop();
	int GetMessage(Data* data);

	virtual ~ServerProcessor() {
	}

	ClientConnection* GetClient(std::string sysId) {
		ClientConnection* con = NULL;
		AutoMutex a(clientsMtx);
		std::map<std::string, ClientConnection*>::iterator it = clients.find(
				sysId);
		if (it != clients.end()) {
			con = it->second;
		}
		return con;
	}

	int RegisterClient(std::string sysId, ClientConnection* clnt) {
		AutoMutex a(clientsMtx);
		std::map<std::string, ClientConnection*>::iterator it = clients.find(
				sysId);
		if (it == clients.end()) {
			clients[sysId] = clnt;
			return 0;
		}
		MT_LOGERROR("client with system id '"<< sysId<<"' already connected!")
		return -1;
	}

	void UnRegisterClient(std::string sysId) {
		AutoMutex a(clientsMtx);

		std::map<std::string, ClientConnection*>::iterator it = clients.find(
				sysId);
		if (it != clients.end()) {
			clients.erase(it);
		} else {
			MT_LOGERROR("no connected client with system id " <<sysId)
		}
	}

	ServerProcessor() :
		Processor() {
		MT_LOGINIT(ServerProcessor)

	}

};

}
}

#endif /* SERVERPROCESSOR_H_ */
