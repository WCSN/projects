#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "mt/logger/Logable.h"

#include "mt/common/Exception.h"

namespace mt {

#define SOCKTYPE_SERVER 1
#define SOCKTYPE_CLIENT 2

enum SockState {
	CLOSED, FIN_WAIT, ACTIVE
};

class TCPSocket: public Logable {

protected:

	int sd;
	SockState state;
	struct sockaddr_in local;
	SockState GetState() {
		return state;
	}
	;
	void SetState(SockState st) {
		state = st;
	}
	;

	struct sockaddr_in CreateSockAddr_In(char* addr, int port) {

		struct sockaddr_in sockAddrStr;
		memset(&sockAddrStr, 0, sizeof(struct sockaddr_in));

		sockAddrStr.sin_family = AF_INET;
		sockAddrStr.sin_port = htons(port);
		if (addr == NULL)
			sockAddrStr.sin_addr.s_addr = INADDR_ANY;
		else
			sockAddrStr.sin_addr.s_addr = inet_addr(addr);

		return sockAddrStr;
	}
	;

public:
	struct sockaddr_in remote;

	TCPSocket();
	TCPSocket(int s, struct sockaddr_in rmt);
	//	TCPSocket(int type);

	int GetSocket() {
		return sd;
	}

	//	int GetType(){return sockType;};

	TCPSocket* Accept() throw (TcpSocketException);

	//depricated
	void Connect(char* addr, int port) throw (TcpSocketException);

	int32_t connect(const std::string& host, uint16_t port){

		Connect((char*) host.c_str(), port);

		return 0;
	}

	void Listen(char* addr, int port) throw (TcpSocketException);
	void Listen() throw (TcpSocketException);
	void Bind(char* addr, int port) throw (TcpSocketException);
	int Recv(char* buf, unsigned int len);
	int Send(char* buf, unsigned int len);
	int SendTo(char* addr, int port, char* buf, unsigned int len)
			throw (std::exception);

	void Close() {
		if ((GetState() == FIN_WAIT) || (GetState() == ACTIVE)) {
			MT_LOGTRACE("close socket")
			close(sd);
			SetState(CLOSED);
		}
	}
	;

	~TCPSocket() {
		Close();
	}
	;

};

}

#endif
