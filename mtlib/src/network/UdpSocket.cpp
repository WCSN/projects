#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#include <mt/network/UdpSocket.h>

#define UDP_PACK_SIZE_LIMIT 65536

namespace mt {

	UdpSocket::UdpSocket() : m_opened(false),
													 m_manual(false),
													 m_incomingBuffer(UDP_PACK_SIZE_LIMIT){
		MT_LOGINIT(UdpSocket);
	}

	UdpSocket::UdpSocket(const std::string& _host, const std::string& _port) : m_manual(false),
																																						 m_incomingBuffer(UDP_PACK_SIZE_LIMIT){
		MT_LOGINIT(UdpSocket);
		open(_host,_port);
	}

	UdpSocket::~UdpSocket(){
		if( m_opened && !m_manual)
			close();
	}

	void UdpSocket::setSocket(int32_t new_sock) {
		if(m_opened && !m_manual)
			close();
		m_opened=true;
		m_socket=new_sock;
		m_manual=true;
	}

	void UdpSocket::open( const std::string& _host, const std::string& _port){
		open( _host, atoi(_port.c_str()) );
	}

	void UdpSocket::open( const std::string& _host, int32_t _port){
		m_host = _host;
		m_port = _port;

		struct sockaddr_in address = createSockAddr(m_host, m_port);

		if(( m_socket = ::socket( PF_INET, SOCK_DGRAM, 0 ) ) == -1)
  		MT_EXCEPTION(mt::Exception, "Cannot create socket, socket() failed");

		const char opt = 1;
		if(setsockopt(m_socket, SOL_SOCKET,  SO_REUSEADDR, &opt, sizeof(int)) == -1)
			MT_EXCEPTION(mt::Exception, "Cannot create socket, setsockopt() failed");

		if (::bind( m_socket, (struct sockaddr*)&address, sizeof(address)) == -1)
			MT_EXCEPTION(mt::Exception, "Cannot bind the socket, errno=" << errno << " [" << strerror(errno) << "]");

		m_opened = true;
	};

	void UdpSocket::close( void ){
		if (m_opened)
		{
			m_opened = false;
			::close( m_socket );
		}
	}

	int UdpSocket::sendTo( const std::string& buffer, const std::string& dstHost, int32_t dstPort ){
		return sendTo( (const uint8_t*)buffer.c_str(), buffer.length(), dstHost, dstPort );
	}

	int UdpSocket::sendTo( const uint8_t* buf, uint32_t size, const std::string& dstHost, int32_t dstPort ) const {
		struct sockaddr_in dstAddr = createSockAddr(dstHost, dstPort);
		if(!m_opened)
			MT_EXCEPTION(mt::Exception, "Failed to send data to " << dstHost << ":" << dstPort << ". Socket is not opened");

		return sendto(m_socket, buf, size, 0, (struct sockaddr *)&dstAddr, sizeof(struct sockaddr_in));
	}

	int UdpSocket::sendTo(const char* strz, const std::string& dstHost, int dstPort) {
		std::string strbuf(strz);
		return sendTo( strbuf,dstHost,dstPort);
	}

	bool UdpSocket::recv(DataPacketPtr* pack, int32_t timeout) {
		if(!m_opened) {
			MT_LOGWARNING("Failed to receive data from socket. Socket is not opened");
			return false;
		}

		struct sockaddr_in saremote;
		socklen_t salen=sizeof(sockaddr_in);

		int rc = 1;
		fd_set readset;

		struct timeval tv;
		if(timeout) {
			tv.tv_sec = timeout / 1000;
			tv.tv_usec = 1000 * (timeout % 1000);

			FD_ZERO( &readset );
			FD_SET( m_socket, &readset );
			rc = ::select( m_socket + 1, &readset, NULL, NULL, &tv );
		} else
			rc = 1;

		m_expired = false;
		if(rc == 0) {
			m_expired = true;
			return false;
		} else if(rc > 0) {
			while(true) {
				int32_t size = ::recvfrom(m_socket, m_incomingBuffer, UDP_PACK_SIZE_LIMIT, 0, (sockaddr*)&saremote, &salen);
				if(size > 0) {
					m_incomingBuffer.setSize(size);
					MT_LOGDEBUG("UDP: Received " << size << " bytes data from " << inet_ntoa(saremote.sin_addr) << ":" << ntohs(saremote.sin_port));
					*pack = new DataPacket(inet_ntoa(saremote.sin_addr), ntohs(saremote.sin_port), m_host, m_port, m_incomingBuffer);
					return true;
				} else if ((size==-1) && (errno==EINTR)) {
					MT_LOGWARNING("socket::read returns -1 with errno EINTR" );
					continue;
				} else {
					MT_LOGWARNING("Socket read failed, read size=" << size << " (trying to read " << UDP_PACK_SIZE_LIMIT <<
												" bytes from " << m_socket << "), errno=" << errno << "[" << strerror(errno) << "]");
					return false;
				}
			}
		}
		return false;
	}

	sockaddr_in UdpSocket::createSockAddr(const std::string& host, const std::string& port) const {
		return createSockAddr(host, atoi(port.c_str()));
	}

	sockaddr_in UdpSocket::createSockAddr( const std::string& host, int _port ) const {
		struct sockaddr_in address;
		memset(&address, 0, sizeof(sockaddr_in));

		struct hostent hp, *hpres;
		char buffer[8192];
		int h_err=0;
  	gethostbyname_r( host.c_str(), &hp, buffer, 8192, &hpres, &h_err );
		if (hpres == NULL)
  		MT_EXCEPTION(mt::Exception, "gethostbyname() failed. Host name is: '" << host << "', errno=" << h_err);

		address.sin_family = AF_INET;
  	address.sin_port = 0==_port || -1==_port ? 0 : (unsigned short)htons((unsigned short) _port);
  	memcpy((char *)&address.sin_addr, ( char * ) hpres->h_addr, hpres->h_length);
		return address;
	}

}
