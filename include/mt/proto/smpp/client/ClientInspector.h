/*
 * TmClientInspector.h
 *
 *  Created on: 03.11.2009
 *      Author: hakamasy
 */

#ifndef TMCLIENTINSPECTOR_H_
#define TMCLIENTINSPECTOR_H_


#include "mt/common/XmlConfig.h"
#include "mt/thread/Mutex.h"

#include "mt/proto/smpp/common/Inspector.h"
#include "mt/proto/smpp/common/Reciver.h"
#include "mt/proto/smpp/common/Sender.h"

namespace mt{
namespace smpp{


class ClientInspector : public Inspector{

	Mutex terminateMtx;

//	static void* Run(void*);

public :

	void NetworkError(SmppNwIf*);
	ClientInspector(XmlConfigPtr cfg, Processor* p);
	void runLoop();

};

}

}

#endif /* TMCLIENTINSPECTOR_H_ */
