#ifndef _MT_SOCKET_
#define _MT_SOCKET_

#include <MT_common.h>
#include <MT_threads.h>
#include <MT_smart_ptr.h>

#include <MT_string.h>
#include <MT_string.h>
#include <errno.h>

#ifdef WIN32
	#pragma warning( disable : 4786 4503 )
#endif

#ifdef WIN32
	#include <windows.h> //	#include <winsock2.h> already included from windows.h

	typedef SOCKET MT_socket;
#else
	typedef int MT_socket;
	#include <netinet/in.h>
	#include <sys/socket.h>
  #include <time.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <arpa/inet.h>
#endif

#define FAIL -1
#define UDP_PACK_SIZE_LIMIT 65536


//---------------------------------------------------------

class Client_socket
{
	MT_socket sock;
	bool manual;

protected:
	bool 	expired;
	bool 	opened;
	MT_string 	host;
	int 	port;
	
	bool	src_set;	
	MT_string	src_host;
	int		src_port;

public:
	MT_EXTERNAL_SYMBOL Client_socket(bool m=false):manual(m),opened(false),src_set(false){};
	MT_EXTERNAL_SYMBOL Client_socket(MT_socket s, bool m=false):sock(s),manual(m),opened(true),src_set(false){};
	MT_EXTERNAL_SYMBOL Client_socket( const MT_string& _host, int _port, 
									const MT_string& _src_host, int _src_port,
									bool m=false):
		manual(m), expired(false),opened(false), host(_host), port(_port), src_set(true), src_host(_src_host),src_port(_src_port)
	{ 	
		open(host,port);
	};
	MT_EXTERNAL_SYMBOL Client_socket( const MT_string& _host, const MT_string& _port, bool m=false):
		manual(m),expired(false),opened(false),host(_host), src_set(false)
	{ 	port = atoi(_port.c_str());
		open(host,_port);
	};
	MT_EXTERNAL_SYMBOL Client_socket( MT_socket s, const MT_string& _host, int _port, bool m=false):
		sock(s),manual(m),expired(false),opened(true),host(_host),port(_port),src_set(false){}; 
	void setSocket( MT_socket newSock ){ opened=true; sock=newSock; manual=true;} 	

	MT_string getHost() const { return host;};
	int getPort() const { return port; };
	
	~Client_socket(){if(opened && !manual) close();};

	MT_EXTERNAL_SYMBOL bool isOpened(){return opened;}
	MT_EXTERNAL_SYMBOL void open( const MT_string& host, const MT_string& port);
	MT_EXTERNAL_SYMBOL void open( const MT_string& host, int port);
	MT_EXTERNAL_SYMBOL void close( void );
	MT_EXTERNAL_SYMBOL int send( const MT_string& buffer );
	MT_EXTERNAL_SYMBOL int send( const char* buf, unsigned int size );
	MT_EXTERNAL_SYMBOL MT_string recv(int count, int timeout=0);
	MT_EXTERNAL_SYMBOL int read(MT_string& buffer, int count, int timeout=0);
	MT_EXTERNAL_SYMBOL int read(char* buf, int count, int timeout=0 );

	MT_EXTERNAL_SYMBOL void setManual(bool m){manual=m;};

	MT_EXTERNAL_SYMBOL MT_socket getSocket(){return sock;};
	MT_EXTERNAL_SYMBOL bool isTimeOut(){return expired;};
	
	static sockaddr_in createSockAddr( const MT_string& host, const MT_string& port );
	static sockaddr_in createSockAddr( const MT_string& host, int port );
};

//---------------------------------------------------------

class Server_socket
{
protected: 	
	MT_string 	host;
	int 	port; 
	bool 	opened;
public:

	MT_EXTERNAL_SYMBOL Server_socket():opened(false){};
	virtual MT_EXTERNAL_SYMBOL void open(const MT_string& host, const MT_string& port);
	
	MT_EXTERNAL_SYMBOL Server_socket(const MT_string& host, const MT_string& port);
	MT_EXTERNAL_SYMBOL MT_socket accept(void);
	MT_EXTERNAL_SYMBOL MT_socket getSocket(){return sock;};
	
	virtual ~Server_socket(){
		if( opened ) close( sock );
		opened = false;
	}

protected:
	MT_socket sock;
};
//---------------------------------------------------------
typedef map<MT_socket, Client_socket*> ClientsMap;
typedef map<MT_string, Client_socket*> ConnectionMap;
typedef map<Client_socket*, MT_memory<unsigned char> > ConnectionData;

class TCPServerSocket : public Server_socket {

protected: 
	ClientsMap 		clients;
	ConnectionMap 	conns;
	fd_set 			mreadset;
	int 			max_fd;
	MT_memory<Client_socket*> readableBuf;
	ConnectionData	connData;
public:
	bool	expired;

	MT_EXTERNAL_SYMBOL TCPServerSocket(){};
	virtual MT_EXTERNAL_SYMBOL void open(const MT_string& host, const MT_string& port);
	MT_EXTERNAL_SYMBOL TCPServerSocket(const MT_string& host, const MT_string& port)
		{open( host, port);};
	
	//the methos returns sum of events that occured
	MT_EXTERNAL_SYMBOL int readOrAccept( Client_socket** newConnection, Client_socket**& readableList, unsigned int* readableSize, double timeout=0.0 );
	
	MT_EXTERNAL_SYMBOL int dropClient( Client_socket* diedClient );
	MT_EXTERNAL_SYMBOL MT_memory<char> getData( Client_socket* cs, unsigned int count, bool discard=true );
	MT_EXTERNAL_SYMBOL unsigned int getDataAvialable( Client_socket* cs );
	MT_EXTERNAL_SYMBOL int addSocket( Client_socket* newClient );
	MT_EXTERNAL_SYMBOL Client_socket* getConnection( MT_string host, int port ) const;
	MT_EXTERNAL_SYMBOL const ClientsMap& getClientsMap() const { return clients;};
		
	virtual ~TCPServerSocket(){
		close();
	}
	void close(){
		if( opened ){
			CITERATE( ClientsMap, clients, ccit ){
				delete ccit->second;
			}
		 	::close( sock );
		}
		opened = false;
	}
private:
	MT_string connectionId( MT_string host, int port ) const;
};

//---------------------------------------------------------
class UDPPacket {
private:
	MT_string			dstHost;
	int				dstPort;
	
	MT_string			srcHost;
	int				srcPort;

	MT_memory<unsigned char> data;	
	unsigned long rcv_time;
	
public:
	//create new packet
  UDPPacket(const MT_string& _dstHost, int _dstPort, const MT_string& _srcHost, int _srcPort, MT_memory<unsigned char> _data):
	dstHost(_dstHost),dstPort(_dstPort),srcHost(_srcHost),srcPort(_srcPort),data(_data), rcv_time(time(NULL)) {
	}
	
	const MT_string& getSrcHost() const {return srcHost;}
	const MT_string& getDstHost() const {return dstHost;}

	int getSrcPort() const {return srcPort;}
	int getDstPort() const {return dstPort;}

	const MT_memory<unsigned char>& getData() const {return data;};	
	
	unsigned long rcvTime() const {return rcv_time;}
	friend class UDPClient_socket;		
};

typedef UDPPacket DataPacket;
typedef MT_smart_ptr<DataPacket> DataPacketPtr;
//---------------------------------------------------------

class UDPClient_socket
{
	MT_socket sock;

	MT_string host;
	int 		port;

protected:
	bool expired;
	bool opened;
	bool manual;

public:
	MT_EXTERNAL_SYMBOL UDPClient_socket(): opened(false),manual(false){};
	MT_EXTERNAL_SYMBOL UDPClient_socket( const MT_string& _host, const MT_string& _port): manual(false)
		{open(_host,_port);};

	void setSocket( MT_socket newSock ){ if(opened && !manual) close(); opened=true; sock=newSock; manual=true;} 	
	
	~UDPClient_socket(){if( opened && !manual) close(); };

	MT_EXTERNAL_SYMBOL void open( const MT_string& host, const MT_string& port);
	MT_EXTERNAL_SYMBOL void open( const MT_string& host, int port);
	MT_EXTERNAL_SYMBOL void close( void );
	MT_EXTERNAL_SYMBOL int sendTo( const MT_string& buffer, const MT_string& dstHost, int dstPort );
	MT_EXTERNAL_SYMBOL int sendTo( const char* strz, const MT_string& dstHost, int dstPort )
		{	MT_string strbuf(strz);
			return sendTo( strbuf,dstHost,dstPort);};
	MT_EXTERNAL_SYMBOL int sendTo( const unsigned char* buf, unsigned int size, const MT_string& dstHost, int dstPort ) const;
	MT_EXTERNAL_SYMBOL bool recv(DataPacketPtr* pack , int timeout=0);

	MT_EXTERNAL_SYMBOL MT_socket getSocket(){return sock;};
	MT_EXTERNAL_SYMBOL bool isTimeOut(){return expired;};
};

//---------------------------------------------------------
class Shared_socket : public Client_socket
{
	MT_mutex mx;

public:
	MT_EXTERNAL_SYMBOL Shared_socket(bool m=false):Client_socket(m){};
	MT_EXTERNAL_SYMBOL Shared_socket(MT_socket s, bool m=false):Client_socket(s,m){};
	MT_EXTERNAL_SYMBOL Shared_socket( const MT_string& host, const MT_string& port, bool m=false):Client_socket(host,port,m){};

	~Shared_socket(){};

	MT_EXTERNAL_SYMBOL void open( const MT_string& host, const MT_string& port)
	{
		MT_automutex am(mx,__FILE__,__LINE__);
		if( !opened )
			Client_socket::open( host, port );
	};
	MT_EXTERNAL_SYMBOL void close( void )
	{
		MT_automutex am(mx,__FILE__,__LINE__);
		if( opened )
			Client_socket::close();
	};
	MT_EXTERNAL_SYMBOL int send( const MT_string& buffer)
	{
		MT_automutex am(mx,__FILE__,__LINE__);
		return Client_socket::send( buffer );
	};
	MT_EXTERNAL_SYMBOL MT_string recv(int count, int timeout=0)
	{
		return ""; // reading from shared socked is unusefull
	};
	MT_EXTERNAL_SYMBOL int read(MT_string& buffer, int count, int timeout=0)
	{
		return -1; // reading from shared socked is unusefull
	};
	MT_EXTERNAL_SYMBOL void setManual(bool m)
	{
		MT_automutex am(mx,__FILE__,__LINE__);
		Client_socket::setManual( m );
	};

	bool is_opened(){ return opened; }
};


#endif
