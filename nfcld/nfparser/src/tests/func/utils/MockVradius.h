/*
 * MockTemplateProc.h
 *
 *  Created on: 26.01.2011
 *      Author: wocson
 */

#ifndef MOCKVRADIUS_H_
#define MOCKVRADIUS_H_

#include <mt/network/UdpSocket.h>
#include <mt/network/DataPacket.h>
#include <mt/logger/Logable.h>

class MockVradius: public mt::Logable
{
public:
	MockVradius(void);
	virtual ~MockVradius();

	mt::ByteBuffer recvRequestFromNfparser(void);
	bool sendResponse2NfParser(mt::ByteBuffer& resp);

private:
	std::string m_dstIp;
	uint16_t m_dstPort;
	uint8_t m_auth[16];
	mt::UdpSocket m_socket;
	void generateResponseAuthentificator(mt::ByteBuffer& out, const std::string& secret);
};

#endif /* MOCKVRADIUS_H_ */
