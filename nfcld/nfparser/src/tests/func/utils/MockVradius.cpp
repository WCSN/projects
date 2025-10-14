/*
 * MockTemplateProc.cpp
 *
 *  Created on: 26.01.2011
 *      Author: wocson
 */

#include "MockVradius.h"
#include "../srvfunc.h"
#include "md5.h"

#define STD_VRADIUS_PORT 3813

MockVradius::MockVradius(void)
{
	MT_LOGINIT(MockVradius);
	m_socket.open("0.0.0.0",STD_VRADIUS_PORT); //???
}

MockVradius::~MockVradius()
{
	m_socket.close();
}

void MockVradius::generateResponseAuthentificator(mt::ByteBuffer& out, const std::string& secret)
{
   memcpy( out + 4, m_auth, 16 );
   mt::ByteBuffer dataToEncode( out );
   dataToEncode.append( (uint8_t*)secret.c_str(), secret.length() );
   uint8_t authBuf[16];
   librad_md5_calc( authBuf, dataToEncode, dataToEncode.getSize() );
   memcpy( out + 4, authBuf, 16 );
}


mt::ByteBuffer MockVradius::recvRequestFromNfparser(void)
{
	mt::DataPacketPtr pack;
	mt::ByteBuffer buff;
	if(m_socket.recv(&pack,UDPTIMEOUT))
	{
		m_dstIp = pack->srcHostAsString();
		m_dstPort = pack->srcPort();
		buff = pack->data();
/*
	std::cout << "\n  RECV: ";
		for(uint_t i=0; i < 20; i++)
			std::cout << std::hex << (uint16_t)buff[i] << " ";
		std::cout << "\n";
*/
		memcpy(m_auth, buff+4, 16);
/*

    std::cout << "\n  auth: ";
    		for(uint_t i=0; i < 16; i++)
    			std::cout << std::hex << (uint16_t)m_auth[i] << " ";
    		std::cout << "\n";
*/
	}

 return buff;
}


bool MockVradius::sendResponse2NfParser(mt::ByteBuffer& resp)
{
/*
	std::cout << "0-SEND: ";
	for(uint_t i=0; i < 20; i++)
		std::cout << std::hex << (uint16_t)resp[i] << " ";
	std::cout << "\n";
*/
	generateResponseAuthentificator(resp, "secret");
/*
	std::cout << "1-SEND: ";
		for(uint_t i=0; i < 20; i++)
			std::cout << std::hex << (uint16_t)resp[i] << " ";
		std::cout << "\n";
*/

	return ( m_socket.sendTo( resp, resp.getSize(), m_dstIp, m_dstPort) > 0);
}
