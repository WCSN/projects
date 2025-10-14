/*
 * conf.h
 *
 *  Created on: 28.12.2010
 *      Author: wocson
 */

#ifndef CONF_H_
#define CONF_H_

#include <string.h>
#include <fstream>

#include "filter.h"

//Network attributs;

class ConnectAttr_host
{
public:
	uint32_t 	lhost;
	std::string slhost;
	uint16_t 	lport;

	uint32_t 	dhost;
	std::string sdhost;
	uint16_t 	dport;

	std::string username;
	std::string sharedSecret;
	std::string iD;

	ConnectAttr_host(void){	lhost = dhost = 0L; lport = dport = 0;
							iD = username = sharedSecret = "none"; slhost = sdhost = "H.Z.H.Z";}
};

class Conf: public mt::Logable
{
private:
	string m_ConfFileName;
	char m_chConfFileName[256];
	bool m_First;

public:
	mt::XmlConfig  *m_xmlcfg;

	std::vector < ConnectAttr_host > vVRadCnt;
	std::vector < ConnectAttr_host > vTemplCnt;
	std::vector < ConnectAttr_host > vNFlowSrc;

	std::fstream flDatastream;
	std::fstream flStatstream;

	std::fstream m_pidfilestream;
	string m_pidFilename;

	time_t m_timeoutstat;
	uint16_t maxflows;

	std::vector < Filter > vFilters;

	Conf(std::string);
	Conf(void);

	void ReReadConf(void);
	~Conf();
private:

	void Init(void);
	bool FindConf(void);
	void ReadConf(void);
	void DataFileOpen();

	void SetTimeOut();
	void SetmaxFlow();
	void SetVRadcntA();
	void SetTemplcntA();
	void SetNflowcntA();
	void SetFilters();
};

extern Conf *conf;

#endif /* CONF_H_ */
