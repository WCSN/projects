/*
 *
 * conf.cpp
 *
 *  Created on: 28.12.2010
 *      Author: wocson
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
#include <syslog.h>
#include <signal.h>
#include <iostream>
#include <iomanip>
#include <exception>
#include <fstream>
#include <unistd.h>

// Mobitech inc.

#include <mt/utils/System.h>
#include <mt/logger/LoggerFactory.h>
#include <mt/common/XmlConfig.h>
#include <mt/common/Exception.h>
#include <mt/logger/Logable.h>
#include <mt/tools/ReloadableConfig.h>
#include <mt/tools/Application.h>

#include "helper.h"
#include "nfparser.h"
#include "netflowprc.h"
#include "conf.h"
#include "filter.h"

bool Conf::FindConf(void)
{
	string path[] = {".","/usr/local/etc","/etc","../nfparser/etc","/home/wocson/etc"};
	string ffname;

	m_ConfFileName = "not find";
    uint size = ARRAY_COUNT(path);

	for(uint i=0; i <= size ; i++)
	{
		if(i==size)
			ffname = home + "/nfparser.config.xml";
		else
			ffname = path[i] + "/nfparser.config.xml";

		if(is_file(ffname))
		{
			m_ConfFileName = ffname;
			break;
		}
	}

	if(m_ConfFileName!="not find")
		return true;
	else
		return false;
}

void Conf::Init(void)
{
	m_xmlcfg = new mt::XmlConfig(m_ConfFileName);
	strcpy(m_chConfFileName,m_ConfFileName.c_str());
}

void Conf::ReReadConf(void)
{
	m_xmlcfg->reload();
	ReadConf();
}

void Conf::ReadConf(void)
{
	if (isopt('d'))
		MT_LOGINFO(LTYELLOW << "Conf file: " << m_ConfFileName << CLSTD);
	if (m_First)
	{
		if (isopt('o') || isopt('s'))
			DataFileOpen(); // if need for break recreate file if get SIGHUP.
	}

	SetTimeOut();
	SetmaxFlow();
	SetVRadcntA();
	SetTemplcntA();
	SetNflowcntA();
	SetFilters();

	m_First=false;
}

void Conf::DataFileOpen(void)
{
	std::string valpar;

	mt::ConfigList lstcfg = m_xmlcfg->getConfigs("Options/Datafiles");

	if(isopt('o'))
	{
		valpar = lstcfg[0]->get("filedata");
		flDatastream.open(valpar.c_str(), std::fstream::ate | std::fstream::out | std::fstream::app);

		if (flDatastream.is_open())
		{
			MT_LOGINFO(LTBLUE << "Data file: " <<  valpar << " open sussed" << CLSTD);
		}
		else
		{
			MT_LOGINFO(LTBLUE <<"Data file: " <<  valpar << " open failed..." << CLSTD);
		}
	}

	if(isopt('s'))
	{
		valpar = lstcfg[0]->get("filestat");
		flStatstream.open(valpar.c_str(), std::fstream::ate | std::fstream::out | std::fstream::app);
		if (flStatstream.is_open())
		{
			MT_LOGINFO(LTBLUE << "Stat file: " <<  valpar << " open sussed" << CLSTD);
		}
		else
			MT_LOGERROR(LTRED << "Stat file: " <<  valpar << " open failed..." << CLSTD);
	}

	lstcfg = m_xmlcfg->getConfigs("Options/Pidfile");

	m_pidFilename = lstcfg[0]->get("pidfile");
}

void Conf::SetTimeOut(void)
{
	std::string valpar;
	mt::ConfigList lstcfg = m_xmlcfg->getConfigs("Options/Timeoutstat");
	valpar = lstcfg[0]->get("timeoutstatistic");
	m_timeoutstat = (time_t)atol(valpar.c_str());
	MT_LOGINFO("timeout: " <<  m_timeoutstat<<"s");
}

void Conf::SetmaxFlow(void)
{
	std::string valpar;
	mt::ConfigList lstcfg = m_xmlcfg->getConfigs("Options/Flows");
	valpar = lstcfg[0]->get("maxflows");
	maxflows = atoi(valpar.c_str());
	MT_LOGINFO("maxflows: " << maxflows);
}

/*
void Conf::DataFileOpen(void)
{
	umask(0);
    if ( (fd = open(valpar.c_str(), O_WRONLY | O_CREAT, 0666)) < 0)
    {
        MT_LOGERROR (LTRED<<"A terrible error occured"<<CLSTD);
        return;
    }
	else
	{
		if(isopt('d'))
			MT_LOGINFO(LTBLUE<<"Data file: " <<  valpar << " sussed"<<CLSTD);
	}
}
*/

void Conf::SetVRadcntA(void)
{
	std::string valpar;
	ConnectAttr_host cntVrad;
	char buff[265];

	mt::ConfigList lstcfg = m_xmlcfg->getConfigs("Connections/VRadiusIP");


	for(uint16_t i=0; i < lstcfg.size(); i++)
	{
		cntVrad.slhost = lstcfg[i]->get("lhost");
		cntVrad.lhost = getIPuint32(cntVrad.slhost);

		cntVrad.sdhost = lstcfg[i]->get("dhost");
		cntVrad.dhost = getIPuint32(cntVrad.sdhost);

		valpar = lstcfg[i]->get("dport");
		cntVrad.dport = atoi(valpar.c_str());

		cntVrad.username = lstcfg[i]->get("username");
		cntVrad.sharedSecret = lstcfg[i]->get("sharedSecret");

		if(isopt('d'))
		{
//			 std::cout << "VRadServ: l: "<< cntVrad.slhost << " " << std::hex << std::setw(8) << std::setfill('0') << std::uppercase << cntVrad.lhost;
			 sprintf(buff,"VRadServ: l: %s %08X:%u d: %s %08X:%u uname: %s secSh: %s",cntVrad.slhost.c_str(),cntVrad.lhost,cntVrad.lport,cntVrad.sdhost.c_str(),cntVrad.dhost,cntVrad.dport,cntVrad.username.c_str(),cntVrad.sharedSecret.c_str());
			 MT_LOGINFO(buff);
		}
		 vVRadCnt.push_back(cntVrad);
	}
}

void Conf::SetTemplcntA(void)
{
	std::string valpar;
	ConnectAttr_host cntTempl;
	char buff[265];

	mt::ConfigList lstcfg = m_xmlcfg->getConfigs("Connections/TemplateProcessorIP");

	for(uint16_t i=0; i < lstcfg.size(); i++)
	{
		cntTempl.slhost = lstcfg[i]->get("lhost");
		cntTempl.lhost = getIPuint32(cntTempl.slhost);

		valpar = lstcfg[i]->get("lport");
		cntTempl.lport = atoi(valpar.c_str());

		cntTempl.sdhost = lstcfg[i]->get("dhost");
		cntTempl.dhost = getIPuint32(cntTempl.sdhost);

		valpar = lstcfg[i]->get("dport");
		cntTempl.dport = atoi(valpar.c_str());

		cntTempl.iD = lstcfg[i]->get("id");

		if(isopt('d'))
		{
			sprintf(buff,"TemplServ: IDnf: %s L: %s %08X:%u d: %s %08X:%u uname: %s secSh: %s",cntTempl.iD.c_str(), cntTempl.slhost.c_str(),cntTempl.lhost,cntTempl.lport,cntTempl.sdhost.c_str(),cntTempl.dhost,cntTempl.dport,cntTempl.username.c_str(),cntTempl.sharedSecret.c_str());
			MT_LOGINFO(buff);
		}
		vTemplCnt.push_back(cntTempl);
	}
}

void Conf::SetNflowcntA(void)
{
	std::string valpar;
	ConnectAttr_host cntNFsrc;
	char buff[265];

	mt::ConfigList lstcfg = m_xmlcfg->getConfigs("Connections/NflowSourceIP");

	for(uint16_t i=0; i < lstcfg.size(); i++)
	{
		cntNFsrc.sdhost = lstcfg[i]->get("dhost");
		cntNFsrc.dhost = getIPuint32(cntNFsrc.sdhost);
		valpar = lstcfg[i]->get("dport");
		cntNFsrc.dport = atoi(valpar.c_str());

		if(isopt('D'))
			MT_LOGINFO("listen port:" << cntNFsrc.dport);

		if(isopt('d'))
		{
			sprintf(buff,"NF_Src: l: %s %08X:%u d: %s %08X:%u uname: %s secSh: %s",cntNFsrc.slhost.c_str(),cntNFsrc.lhost,cntNFsrc.lport,cntNFsrc.sdhost.c_str(),cntNFsrc.dhost,cntNFsrc.dport,cntNFsrc.username.c_str(),cntNFsrc.sharedSecret.c_str());
			MT_LOGINFO(buff);
		}
		vNFlowSrc.push_back(cntNFsrc);
	}
}

void Conf::SetFilters(void)
{
	char buff[265];
	uint16_t slash_pos=0;
	std::string valpar;
//	int szv = 0;

	mt::ConfigList lstcfg = m_xmlcfg->getConfigs("Filters/Filter");

	for(mt::ConfigList::iterator it = lstcfg.begin(); it != lstcfg.end(); it++)
	{
// set id filter
		Filter filter;
		mt::ConfigList lstcfgsub;

		filter.iD = (*it)->get("id");
		if(isopt('d'))
			MT_LOGINFO(LTGREEN "Filter ID: " << filter.iD << "=================" << CLSTD);
// set srcNet
		lstcfgsub = (*it)->getConfigs("srcNet");

		mt::ConfigList::iterator itsb;

		//szv = lstcfgsub.size();
		//if( szv )
		for(itsb = lstcfgsub.begin(); itsb != lstcfgsub.end(); itsb++)
		{
			valpar = (*itsb)->get("network");
			slash_pos = valpar.find("/");

			filter.netattr.network = getIPuint32(valpar.substr(0,slash_pos).c_str());
			filter.netattr.netmask = 0xffffffff << (32 - (atoi(valpar.substr(slash_pos+1).c_str())));

			valpar = (*itsb)->get("operation");
			filter.netattr.op = valpar.compare("exclude")!=0;
			if(isopt('d'))
			{
				sprintf(buff,"%08X/%08X op:%u",filter.netattr.network,filter.netattr.netmask,filter.netattr.op);
				MT_LOGINFO("SrcNet: " << buff );
			}
			filter.vsrcNets.push_back(filter.netattr);
		}/*
		else
		{
			filter.netattr.network = 0L;
			filter.netattr.netmask = 0L;
			filter.netattr.op = true;
			if(isopt('d'))
			{
				sprintf(buff,"%08X/%08X op:%u",filter.netattr.network,filter.netattr.netmask,filter.netattr.op);
				MT_LOGINFO("SrcNet: " << buff);
			}
			filter.vsrcNets.push_back(filter.netattr);
		}*/

//get dstNet
		lstcfgsub = (*it)->getConfigs("dstNet");
		//szv = lstcfgsub.size();
		//if( szv )
		for(itsb = lstcfgsub.begin(); itsb != lstcfgsub.end(); itsb++)
		{
			valpar = (*itsb)->get("network");

			slash_pos = valpar.find("/");

			filter.netattr.network = getIPuint32(valpar.substr(0,slash_pos).c_str());
			filter.netattr.netmask = 0xffffffff << (32 - (atoi(valpar.substr(slash_pos+1).c_str())));
			valpar = (*itsb)->get("operation");

			filter.netattr.op = valpar.compare("exclude")!=0;

			if(isopt('d'))
			{
				sprintf(buff,"%08X/%08X op:%u",filter.netattr.network,filter.netattr.netmask,filter.netattr.op);
				MT_LOGINFO("DstNet: " << buff);
			}
			filter.vdstNets.push_back(filter.netattr);
		}/*
		else
		{
			filter.netattr.network = 0L;
			filter.netattr.netmask = 0L;
			filter.netattr.op = true;
			if(isopt('d'))
			{
				sprintf(buff,"%08X/%08X op:%u",filter.netattr.network,filter.netattr.netmask,filter.netattr.op);
				MT_LOGINFO("DstNet: " << buff);
			}
			filter.vdstNets.push_back(filter.netattr);
		}*/

// get Protocols
		lstcfgsub = (*it)->getConfigs("protocol");
		//szv = lstcfgsub.size();
		//if( szv )
		for (itsb = lstcfgsub.begin(); itsb != lstcfgsub.end(); itsb++)
		{
			valpar = (*itsb)->get("value");
			filter.netaddpar.val = atoi(valpar.c_str());
			valpar = (*itsb)->get("operation");

			filter.netaddpar.op = valpar.compare("exclude")!=0;

			if (isopt('d'))
				MT_LOGINFO("Protocol: " << filter.netaddpar.val << " op:" << filter.netaddpar.op);

			filter.vProts.push_back(filter.netaddpar);
		}/*
		else
		{
			filter.netaddpar.val = 0;
			filter.netaddpar.op = true;
			if (isopt('d'))
				MT_LOGINFO("Protocol: " << filter.netaddpar.val << " op:" << filter.netaddpar.op);
			filter.vProts.push_back(filter.netaddpar);
		}*/

// get Tos
		lstcfgsub = (*it)->getConfigs("typeos");
		//szv = lstcfgsub.size();
		//if( szv )
		for (itsb = lstcfgsub.begin(); itsb != lstcfgsub.end(); itsb++)
		{
			valpar = (*itsb)->get("value");
			filter.netaddpar.val = atoi(valpar.c_str());

			valpar = (*itsb)->get("operation");
			filter.netaddpar.op = valpar.compare("exclude")!=0;

			if (isopt('d'))
				MT_LOGINFO("TOS: " <<  filter.netaddpar.val << " op:" << filter.netaddpar.op);
			filter.vToss.push_back(filter.netaddpar);
		}/*
		else
		{
			filter.netaddpar.val = 0;
			filter.netaddpar.op = true;
			if (isopt('d'))
				MT_LOGINFO("TOS: " <<  filter.netaddpar.val << " op:" << filter.netaddpar.op);
			filter.vProts.push_back(filter.netaddpar);
		}*/

// getPorts
		lstcfgsub = (*it)->getConfigs("dstport");
		//szv = lstcfgsub.size();
		//if( szv )
		for (itsb = lstcfgsub.begin(); itsb != lstcfgsub.end(); itsb++)
		{
			valpar = (*itsb)->get("value");
			filter.netaddpar.val = atoi(valpar.c_str());
			valpar = (*itsb)->get("operation");

			filter.netaddpar.op = valpar.compare("exclude") != 0;

			if (isopt('d'))
				MT_LOGINFO("Port: " << filter.netaddpar.val << " op:" << filter.netaddpar.op);
			filter.vDstPorts.push_back(filter.netaddpar);
		}/*
		else
		{
			filter.netaddpar.val = 0;
			filter.netaddpar.op = true;
			if (isopt('d'))
				MT_LOGINFO("Port: " << filter.netaddpar.val << " op:" << filter.netaddpar.op);
			filter.vProts.push_back(filter.netaddpar);
		}*/

		vFilters.push_back(filter); //Store all filters
	}//for //Filters
}

Conf::Conf(void)
{
	m_First=true;

	MT_LOGINIT(Conf);
	if(FindConf())
	{
		Init();
		ReadConf();
	}
}

Conf::Conf(std::string conffilename):m_ConfFileName(conffilename)
{
	m_First=true;
	MT_LOGINIT(Conf);

    if(is_file(m_ConfFileName))
    {
    	Init();
    	ReadConf();
    }
}

Conf::~Conf()
{
//	fclose(outfile);
	delete m_xmlcfg;
	flDatastream.close();
	flStatstream.close();
}
