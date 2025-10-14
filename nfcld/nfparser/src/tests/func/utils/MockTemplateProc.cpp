/*
 * MockTemplateProc.cpp
 *
 *  Created on: 26.01.2011
 *      Author: wocson
 */

#include <cppunit/extensions/HelperMacros.h>
#include "../srvfunc.h"
#include "MockTemplateProc.h"

#define STD_TEMPLPROC_PORT 7777

MockTemplateProc::MockTemplateProc(void)
{
	MT_LOGINIT(MockTemplateProc);
	m_socket.open("0.0.0.0",STD_TEMPLPROC_PORT); //???
}


MockTemplateProc::~MockTemplateProc()
{

}

/*
CommandPtr MockTemplateProc::recvCommandFromNfparser(void)
{
	mt::DataPacketPtr pack;
//	CommandPtr cmdptr;

	if(m_socket.recv(&pack,UDPTIMEOUT))
	{
		mt::ByteBuffer buff = pack->data();
//		cmdptr = new Command(buff);
	}
return cmdptr;
}
*/
