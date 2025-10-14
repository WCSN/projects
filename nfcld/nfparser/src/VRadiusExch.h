/*
 * VRadiusExch.h
 *
 *  Created on: 11.02.2011
 *      Author: wocson
 */

#ifndef VRADIUSEXCH_H_
#define VRADIUSEXCH_H_

#include <string.h>

#include <mt/tools/ReloadableConfig.h>
#include <mt/logger/Logable.h>
#include <vradius_client.h>


class VRadiusExch: public mt::Logable {
public:
	VRadiusExch(const mt::XmlConfigPtr& cfg);
	virtual ~VRadiusExch();

	std::string getMSISDN(void);
	std::string getCalledStationId(void);

	bool sendVRadiusRequiest(const std::string& ipaddr);

private:

	//VRADIUSCLIENT vrc;

	std::string m_MSISDN;
	std::string m_CalledStationId;
};


#endif /* VRADIUSEXCH_H_ */
