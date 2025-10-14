#include "mt/network/TCPSocket.h"

namespace mt {

TCPSocket::TCPSocket() {

	MT_LOGINIT(TCPSocket);

	int isopt = 1;
	sd = socket(PF_INET, SOCK_STREAM, 0);
	if (sd <= 0) {
		MT_LOGERROR("can't create socket");
		throw TcpSocketException("can't create socket");
	}
	fcntl(sd, F_SETFD, FD_CLOEXEC);
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &isopt, sizeof(isopt));
	state = CLOSED;

}

TCPSocket::TCPSocket(int s, struct sockaddr_in rmt) {
	MT_LOGINIT(TCPSocket);

	sd = s;
	state = ACTIVE;
	remote = rmt;
}

TCPSocket* TCPSocket::Accept() throw (TcpSocketException) {
	state = ACTIVE;

	socklen_t len = sizeof(struct sockaddr_in);
	int retS = ::accept(sd, (sockaddr*) &remote, &len);
	if (retS < 0) {
		m_logger->error("accept error: %s", strerror(errno));
		throw(TcpSocketException("accept return error"));
	}
	m_logger->trace("accept new connection");
	TCPSocket *retTCPSocket = (TCPSocket*) new TCPSocket(retS, remote);
	return retTCPSocket;
}

void TCPSocket::Connect(char* addr, int port) throw (TcpSocketException) {

	struct sockaddr_in servAddr;
	servAddr = CreateSockAddr_In(addr, port);
	if (::connect(sd, (struct sockaddr*) &servAddr, sizeof(servAddr)) != 0) {
		m_logger->error("can't connect to addr: %s port %u with error: %s", addr,
				port, strerror(errno));
		throw TcpSocketException("can't connect to socket");

	}
	state = ACTIVE;
}

void TCPSocket::Listen(char* addr, int port) throw (TcpSocketException) {

	Bind(addr, port);
	Listen();

}

void TCPSocket::Listen() throw (TcpSocketException) {

	if (listen(sd, SOMAXCONN) != 0) {
		MT_LOGERROR("can't listen socket: " << strerror(errno));
		throw TcpSocketException("can't listen socket");
	}
}

void TCPSocket::Bind(char* addr, int port) throw (TcpSocketException) {

	struct sockaddr_in myAddr;
	myAddr = CreateSockAddr_In(addr, port);
	if (bind(sd, (const sockaddr*) &myAddr, sizeof(myAddr)) != 0) {
		m_logger->error("can't bind to : %s, port %u, with error: (%s)", addr,
				port, strerror(errno));
		throw TcpSocketException("can't bind");
	}

}

int TCPSocket::Recv(char* buf, unsigned int bufLen) {

	int ret = -1;
printf("asdf %d",ret);
	if (GetState() == ACTIVE) {
		ret = recv(sd, buf, bufLen, 0);
		if (ret < 0) {
			m_logger->error("recv return error: %s", strerror(errno));
		} else if (ret == 0) {
			m_logger->warn("recv return fin_ack");
			SetState(FIN_WAIT);
			ret = -1;
		}
	}
	return ret;
}

int TCPSocket::Send(char* buf, unsigned int len) {

	int ret = -1;

	if (GetState() == ACTIVE) {
		ret = send(sd, buf, len, 0);
	}
	return ret;
}

//TODO


int TCPSocket::SendTo(char* addr, int port, char* buf, unsigned int len)
		throw (std::exception) {

	int isopt = 1;

	sd = socket(PF_INET, SOCK_STREAM, 0);
	if (sd < 0) {
		MT_LOGERROR("can't create socket");
		throw TcpSocketException("can't create socket");
	}
	fcntl(sd, F_SETFD, FD_CLOEXEC);
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &isopt, sizeof(isopt));

	Connect(addr, port);

	return Send(buf, len);

}

}

