/*
 * NflowProc.h
 *
 *  Created on: 09.02.2011
 *      Author: wocson
 */

#ifndef NFPROC_H_
#define NFPROC_H_

#include <mt/logger/Logable.h>
#include <mt/tools/ReloadableConfig.h>
#include <mt/common/DynamicBuffer.h>
#include <mt/network/UdpSocket.h>

#define UDPTIMEOUT 3000

class NfReceiver: public mt::Logable {
public:
	NfReceiver(const mt::XmlConfigPtr& cfg);
	virtual ~NfReceiver();

	mt::ByteBuffer recvNflow(void);

private:
	mt::UdpSocket m_socket;
};

#endif /* NFPROC_H_ */
