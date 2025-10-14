#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <algorithm>
#include <set>

#include <mt/logger/LoggerFactory.h>
#include <mt/logger/Logable.h>

#include "helper.h"
#include "wcsocket.h"
#include "nfparser.h"
#include "netflowprc.h"

using namespace std;

void WC_Socket::Init(void)
{
	//
}

WC_Socket::WC_Socket(void)
{
	MT_LOGINIT(WC_Socket);
//	Init();
}

int WC_Socket::Listener(void)
{

	   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	    {
	    	MT_LOGERROR(LTRED<<"ERROR open socket..."<<CLSTD);
	        return ERROR_SCKT;
	    }

	    addr.sin_family = AF_INET;
	    addr.sin_port = htons(conf->vNFlowSrc[0].dport);
	    addr.sin_addr.s_addr = htonl(INADDR_ANY);

	    if(!isopt('D'))
	        MT_LOGINFO(LTGREEN<<"Netflow receive on port " << conf->vNFlowSrc[0].dport << CLSTD);

	    if( bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	    {
	    	MT_LOGERROR(LTRED<<"ERROR bind sockaddress..."<<CLSTD);
	        return ERROR_SCKT;
	    }

	    nfpcsr->Parser(sock);
    
    return sock;
}


WC_Socket::~WC_Socket()
{
   close(sock);
}


uint32_t getIPuint32(std::string valpar)
{
	sockaddr_in net_address;
	inet_aton(valpar.c_str(),&net_address.sin_addr);	// IP as string to uint
	return htonl(net_address.sin_addr.s_addr);			// get IP as number uint32_t for src_net;
}


uint32_t getIPuint32(char *varpar)
{
	sockaddr_in net_address;
	inet_aton(varpar,&net_address.sin_addr); // IP as string to uint
	return htonl(net_address.sin_addr.s_addr) ; // get IP as number uint32_t for src_net;
}
