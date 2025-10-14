#ifndef _WCSOCKET_H_
#define _WCSOCKET_H_

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h> 

#include <mt/logger/LoggerFactory.h>
#include <mt/logger/Logable.h>

#include "netflowprc.h"

class WC_Socket: public mt::Logable
{
private:
    struct sockaddr_in addr;
    int sock;
    void Init(void);

public:
        int Listener(void);
        WC_Socket(void);
        ~WC_Socket();
};

uint32_t getIPuint32(std::string);
uint32_t getIPuint32(char *);

#endif  /* _WCSOCKET_H_ */

