#ifndef VRADIUS_CLIENT_H
#define VRADIUS_CLIENT_H

#include <time.h>

#define DEFAULT_ACCT_PORT 3813

#define VRADIUS_ERROR_TIMEOUT 				1
#define VRADIUS_ERROR_REMOTE_TIMEOUT		2
#define VRADIUS_ERROR_NO_ENTRY				3
#define VRADIUS_ERROR_REQUEST_NOT_ALLOWED	4
#define VRADIUS_ERROR_ACCESS_DENIED			5
#define VRADIUS_ERROR_TOO_MANY_REQUESTS		6
#define VRADIUS_ERROR_ATTRIBUTES_NOT_ALLOWED 7
#define VRADIUS_ERROR_INVALID_FORMAT		8
#define VRADIUS_ERROR_NOT_IMPLEMENTED		9
#define VRADIUS_ERROR_SERVER				10

extern int vradius_error_code;

//==============================================================================
typedef struct {
	char **data;
	int n_data;
	char *ip;
	char *msisdn;
	time_t lastUpdateDate;
	time_t reuseDate; 
	time_t sessionStartDate;
} VRADIUSREPLY;

//==============================================================================
typedef struct {
	void* client;
} VRADIUSCLIENT;

//==============================================================================

#ifdef __cplusplus
extern "C" {
#endif

void VRadiusReplyDestroy(VRADIUSREPLY *reply);

int VRadiusInit(VRADIUSCLIENT *client,
		        char* hostname,
                int acctPort, 
                char* username, 
                char* sharedSecret,
                char* localhost);

void VRadiusDestroy(VRADIUSCLIENT *client);

VRADIUSREPLY VRadiusGetDataByIp(VRADIUSCLIENT *client,
		                        char* ip); 

VRADIUSREPLY VRadiusGetDataByIpFull(VRADIUSCLIENT *client,
		                            char* ip,
		                            char** attrList,
		                            int attrListNum,
		                            time_t date, 
		                            int useCache); 

VRADIUSREPLY VRadiusGetDataByMsisdn(VRADIUSCLIENT *client,
		                            char* msisdn);

VRADIUSREPLY VRadiusGetDataByMsisdnFull(VRADIUSCLIENT *client,
		                                char* msisdn, 
		                                char** attrList, 
			                            int attrListNum,
		                                time_t date, 
		                                int useCache);

char* VRadiusGetMsisdn(VRADIUSCLIENT *client,
		               char* ip); 

char* VRadiusGetMsisdnFull(VRADIUSCLIENT *client,
		                   char* ip, 
	                       time_t date, 
	                       int useCache); 

#ifdef __cplusplus
}; //extern "C"
#endif

#endif
