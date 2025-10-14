#include <iostream>
#include <fstream>
#include "MockCisco.h"


MockCisco::MockCisco(const std::string& lhost,const std::string& host, int32_t port): m_dstIp(host), m_dstPort(port)
{
	m_socket.open(lhost, 0);
	MT_LOGINIT(MockCisco);
}

MockCisco::~MockCisco() {
	m_socket.close();
	delete m_ciscoNflow;
}

uint32_t MockCisco::sendFlow(const std::string& filename) {

	m_ciscoNflow = new FileData(filename);
	mt::ByteBuffer buffer = m_ciscoNflow->getDataFromFile();

	return m_socket.sendTo(buffer,buffer.getSize(), m_dstIp, m_dstPort);
}
