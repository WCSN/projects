/*
 * RvServerInspector.h
 *
 *  Created on: 01.12.2009
 *      Author: hakamasy
 */

#ifndef SERVERINSPECTOR_H_
#define SERVERINSPECTOR_H_

#include "mt/common/XmlConfig.h"

#include "mt/thread/Mutex.h"
#include "mt/thread/AutoMutex.h"

#include "mt/proto/smpp/common/Inspector.h"
#include "mt/proto/smpp/common/Reciver.h"
#include "mt/proto/smpp/common/Sender.h"
#include <vector>

namespace mt {
namespace smpp {

class ServerInspector: public Inspector {
	std::vector<TCPSocket*> socks4del;
	TCPSocket* serverSock;
	Mutex mtx;

	void runLoop();
	bool CheckClient(BindParams);

public:
	int Init();

	ServerInspector(XmlConfigPtr cfg);
	void NetworkError(SmppNwIf*);

};

}
}

#endif /* RVSERVERINSPECTOR_H_ */
