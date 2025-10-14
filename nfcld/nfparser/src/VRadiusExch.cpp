/*
 * VRadiusExch.cpp
 *
 *  Created on: 11.02.2011
 *      Author: wocson
 */

#include "VRadiusExch.h"

const char *msg[]=
{
		"VRADIUS_SUSSES",
		"VRADIUS_ERROR_TIMEOUT",
		"VRADIUS_ERROR_REMOTE_TIMEOUT",
		"VRADIUS_ERROR_NO_ENTRY",
		"VRADIUS_ERROR_REQUEST_NOT_ALLOWED",
		"VRADIUS_ERROR_ACCESS_DENIED",
		"VRADIUS_ERROR_TOO_MANY_REQUESTS",
		"VRADIUS_ERROR_ATTRIBUTES_NOT_ALLOWED",
		"VRADIUS_ERROR_INVALID_FORMAT",
		"VRADIUS_ERROR_NOT_IMPLEMENTED",
		"VRADIUS_ERROR_SERVER"
};


VRadiusExch::VRadiusExch(const mt::XmlConfigPtr& cfg) {
	MT_LOGINIT(VRadiusExch);
/*
	if (VRadiusInit(&vrc,								// VRADIUSCLIENT *client,
			(char*) cfg->get("dhost").c_str(),			// char* hostname,
			atoi(cfg->get("dport").c_str()),			// int acctPort,
			(char*) cfg->get("username").c_str(),		// char* username,
			(char*) cfg->get("sharedSecret").c_str(),	// char* sharedSecret,
			(char*) cfg->get("lhost").c_str()			// char* localhost);
	) == 0) {
		MT_LOGERROR("Error while initializing VRADIUS interface");
		MT_EXCEPTION(mt::Exception, "Vradius connect not initialize...");
	}
	*/
}

VRadiusExch::~VRadiusExch() {

}

bool VRadiusExch::sendVRadiusRequiest(const std::string& ipaddr) {

	bool st_requiest = false;
/*	VRADIUSREPLY reply;
	char *attrList0[] = { "MSISDN", "CalledStationId" };
	char Ipaddr[32];
	strcpy(Ipaddr,ipaddr.c_str());

	MT_LOGINFO("(VRadius request for IP: " << ipaddr);

	reply = VRadiusGetDataByIpFull(&vrc, // VRADIUSCLIENT *client,
			Ipaddr, // char* ip,
			attrList0, // char** attrList,
			2, // int attrListNum,
			0, // time_t date,
			0); // int useCache);

	if (vradius_error_code == 0) {
		m_MSISDN.assign(reply.data[0]);
		m_CalledStationId.assign(reply.data[1]);
		MT_LOGINFO("Vradius response. MSISDN: " << m_MSISDN << " CalledStationId: " << m_CalledStationId);
		st_requiest = true;
	} else
		MT_LOGERROR("(VRadius_request/VRadiusGetDataByIpFull): "<< msg[vradius_error_code]);

		VRadiusReplyDestroy(&reply);
*/
return st_requiest;
}

std::string VRadiusExch::getMSISDN(void) {
	return m_MSISDN;
}

std::string VRadiusExch::getCalledStationId(void) {
	return m_CalledStationId;
}
