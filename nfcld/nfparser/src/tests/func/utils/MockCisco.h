#ifndef __MOCKCISCO__H
#define __MOCKCISCO__H

#include <string>

#include <mt/network/UdpSocket.h>
#include <mt/logger/Logable.h>
#include "../FileData.h"

class MockCisco: public mt::Logable {

public:
	MockCisco(const std::string& lhost, const std::string& host, int32_t port);
	virtual ~MockCisco();
	
	uint32_t sendFlow(const std::string& filename);

private:
	std::string m_dstIp;
	int32_t m_dstPort;

	mt::UdpSocket m_socket;
	FileData *m_ciscoNflow;
};


#endif /* __MOCKCISCO__H */
