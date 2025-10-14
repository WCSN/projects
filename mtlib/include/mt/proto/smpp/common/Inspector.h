#ifndef SMPP_INSPECTOR_H_
#define SMPP_INSPECTOR_H_

#include <string>


#include "mt/proto/smpp/common/Reciver.h"
#include "mt/proto/smpp/common/Sender.h"
#include "mt/network/TCPSocket.h"
#include "mt/proto/smpp/common/Exceptions.h"
#include "mt/proto/smpp/common/Processor.h"
#include "mt/proto/smpp/common/DataTypes.h"


#include "mt/logger/Logable.h"

namespace mt{

namespace smpp{

class BindParams{

public :
	std::string sysId;
	BindType type;

};

class Inspector :  public SmppNwIf, public InfiniteThread{

protected :
	unsigned char ton;
	unsigned char npi;
	BindType type;
	Sender* sndr;
	Reciver* rcvr;
	Processor* proc;
	char *recvBuf;
	std::string addr;
	std::string password;
	std::string systype;
	unsigned short port;

	void SendBind(TCPSocket* sock) throw (std::exception);
	BindParams RecvBind(TCPSocket* sock) throw (std::exception);

	void SendBindResp(TCPSocket* sock, BindType t) throw (std::exception);
	int RecvBindResp(TCPSocket* sock) throw (std::exception);
	void SendUnbind(TCPSocket* sock) throw (std::exception);
	int RecvUnbindResp(TCPSocket* sock) throw (std::exception);

public :
	void SetTon(unsigned char c){ton = c;};
	void SetNpi(unsigned char n){npi = n;};

	std::string getSystemId(){return m_systemId;};
	virtual ~Inspector();
	Inspector(Processor* p = NULL);
	virtual void NetworkError(SmppNwIf*) = 0;
//	virtual void Run() = 0;
	void SetSender(Sender* s){sndr = s;};
	void SetReciver(Reciver* s){rcvr = s;};
	void SetProcessor(Processor* s){proc = s;};
	Processor* GetProcessor(){return proc;};


};

}
}
#endif /* INSPECTOR_H_ */
