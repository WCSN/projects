/*
 * MockTemplateProc.h
 *
 *  Created on: 26.01.2011
 *      Author: wocson
 */

#ifndef MOCKTEMPLATEPROC_H_
#define MOCKTEMPLATEPROC_H_

#include <mt/network/UdpSocket.h>
#include <mt/logger/Logable.h>
//#include <command/src/Command.h>


class MockTemplateProc: public mt::Logable
{
public:
	MockTemplateProc(void);
	virtual ~MockTemplateProc();
//	CommandPtr recvCommandFromNfparser(void);

private:
	mt::UdpSocket m_socket;
};

#endif /* MOCKTEMPLATEPROC_H_ */
