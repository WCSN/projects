#ifndef __NETWORK__UDP_SOCKET__H
#define __NETWORK__UDP_SOCKET__H

#include <mt/network/DataPacket.h>
#include <mt/logger/Logable.h>

namespace mt {
	/*!
	 * \class UdpSocket
	 * \brief UDP socket implementation.
	 */

	class UdpSocket : public Logable{

	public:
		//@{
		/// Default constructor.
		UdpSocket();
		/**
		 * @param _host IP address at string format. Socket will be binded on this host.
		 * @param _port Socket will be binded on this port.
		 * @throws mt::Exception if an error occurred at creation socket.
		 * @throws mt::Exception if an error occurred at setting socket's options.
		 * @throws mt::Exception if an error occurred at binding socket.
		 */
		UdpSocket(const std::string& _host, const std::string& _port);
		virtual ~UdpSocket();
		//@}

		/**
		 * The method set new socket. If old socket was opened than it will be closed.
		 *
		 * @param newSock New socket's descriptor.
		 */
		void setSocket(int32_t newSock);

		/**
		 * Create and bind socket. Option SO_REUSEADDR set to the socket.
		 *
		 * @param host IP address as string format. Socket will be binded on this host.
		 * @param port Socket will be binded on this port.
		 * @throws mt::Exception if an error occurred at creation socket.
		 * @throws mt::Exception if an error occurred at setting socket's options.
		 * @throws mt::Exception if an error occurred at binding socket.
		 */
		void open(const std::string& host, const std::string& port);

		/**
		 * Create and bind socket. Option SO_REUSEADDR set to the socket.
		 *
		 * @param host IP address as string format. Socket will be binded on this host.
		 * @param port Socket will be binded on this port.
		 * @throws mt::Exception if an error occurred at creation socket.
		 * @throws mt::Exception if an error occurred at setting socket's options.
		 * @throws mt::Exception if an error occurred at binding socket.
		 */
		void open(const std::string& host, int32_t port);

		/// Close the socket.
		void close();

		/**
		 * Send data to host.
		 *
		 * @param strz Data to send.
		 * @param dstHost Destination host.
		 * @param dstPort Destination port.
		 * @throws mt::Exception if the socket wasn't opened.
		 *
		 * @return Upon successful completion, shall return the number of bytes sent. Otherwise, -1 shall be returned.
		 */
	    int32_t sendTo(const char *strz, const std::string & dstHost, int32_t dstPort);

	    /**
		 * Send data to host.
		 *
		 * @param buf Data to send.
		 * @param size Size of data.
		 * @param dstHost Destination host.
		 * @param dstPort Destination port.
		 * @throws mt::Exception if the socket wasn't opened.
		 *
		 * @return Upon successful completion, shall return the number of bytes sent. Otherwise, -1 shall be returned.
		 */
	    int32_t sendTo(const uint8_t *buf, uint32_t size, const std::string & dstHost, int32_t dstPort) const;

	    /**
		 * Send data to host.
		 *
		 * @param buffer Data to send.
		 * @param dstHost Destination host.
		 * @param dstPort Destination port.
		 * @throws mt::Exception if the socket wasn't opened.
		 *
		 * @return Upon successful completion, shall return the number of bytes sent. Otherwise, -1 shall be returned.
		 */
	    int32_t sendTo(const std::string & buffer, const std::string & dstHost, int32_t dstPort);

	    /**
		 * Receive data.
		 *
		 * @param pack Pointer to pointer to data packet. The method will create DataPacket with received data.
		 * @param timeout Timeout for receive packet at milliseconds.
		 *
		 * @return TRUE if packet was received. Otherwise, will return FALSE.
		 */
	    bool recv(DataPacketPtr *pack, int32_t timeout = 0);

	    /**
		 * Receive data.
		 *
		 * @param pack Pointer to pointer to data packet. The method will create DataPacket with received data.
		 * @param timeout Timeout for receive packet at milliseconds.
		 *
		 * @return TRUE if packet was received. Otherwise, will return FALSE.
		 */
	    bool recvFrom(DataPacketPtr *pack, int32_t timeout = 0);




	    int32_t getSocket(){
				return m_socket;
			}

	    /**
		 * @return TRUE if last packet was timeouted. Otherwise, will return FALSE.
		 */
	    bool isTimeOut() {
	        return m_expired;
	    }

	private:

	    /**
		 * Creation socket address.
		 *
		 * @param host Host address.
		 * @param port Port.
		 *
		 * @return struct sockaddr_in for host and port.
		 */
	    sockaddr_in createSockAddr(const std::string & host, const std::string & port) const;

	    /**
		 * Creation socket address.
		 *
		 * @param host Host address.
		 * @param port Port.
		 *
		 * @return struct sockaddr_in for host and port.
		 */
	    sockaddr_in createSockAddr(const std::string & host, int32_t _port) const;

	private:
		int32_t m_socket;						/**< The socket's descriptor. */
		std::string m_host;					/**< The socket's local host address. */
		int32_t m_port;							/**< The socket's local port. */

		bool m_expired;							/**< Set to TRUE if last packet was timeouted. */
		bool m_opened;							/**< Set to TRUE if socket was opened. */
		bool m_manual;							/**< Set to TRUE if socket was set manually. */

		mt::ByteBuffer m_incomingBuffer;
	};
}
#endif /* __NETWORK__UDP_SOCKET__H */
