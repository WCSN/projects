/*
 * NflowProc.cpp
 *
 *  Created on: 09.02.2011
 *      Author: wocson
 */

#include <mt/network/UdpSocket.h>

#include "NfReceiver.h"

NfReceiver::NfReceiver(const mt::XmlConfigPtr& cfg)
{

	MT_LOGINIT(NfReceiver);

	try {
		MT_LOGDEBUG("Open listen udp socket for neflows ("<<cfg->get("host")<<"/"<<atoi(cfg->get("port").c_str())<<")");

		m_socket.open(cfg->get("host"), atoi(cfg->get("port").c_str()));

	} catch (const mt::Exception& me) {
		MT_LOGERROR("Exception in process: " << me.displayText());
	}
}

NfReceiver::~NfReceiver()
{

	MT_LOGDEBUG("Close listen udp socket for neflows.");
	m_socket.close();
}

mt::ByteBuffer NfReceiver::recvNflow(void)
{
	mt::DataPacketPtr pack;
	mt::ByteBuffer buff;

	if (m_socket.recv(&pack))
	{
		buff = pack->data();
		return buff;
	}
	return buff;
}

