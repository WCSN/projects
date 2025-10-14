/*
    netflwprcr.cpp
*/

#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <vector>
#include <iostream>
using namespace std;

// Mobitech inc.
#include <Command.h>
#include <CommandSender.h>

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#include "netflowprc.h"
#include "nfparser.h"
#include "helper.h"
#include "conf.h"
#include "filter.h"

char *msg[] =
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

char *attrList0[] = {"MSISDN","CalledStationId"};

void NetflowPr::VRadius_request(void)
{
	if(isopt('d'))
		MT_LOGINFO("Send request to VRadius by srcIP: "<< get_ipaddr(flow5->srcaddr));

	VRADIUSREPLY reply;
	reply = VRadiusGetDataByIpFull(
									&vrc,								// VRADIUSCLIENT *client,
									get_ipaddr(flow5->srcaddr),			// char* ip,
									attrList0,              			// char** attrList,
									2,                      			// int attrListNum,
									0,                      			// time_t date,
									0);                     			// int useCache);

	if(vradius_error_code == 0)
	{
		VRadReq_countOk++;

		 strcpy(ulMSISDN, reply.data[0]);
		 strcpy(CalledStationId, reply.data[1]);

		 if(isopt('d'))
			 MT_LOGINFO("Vradius response. MSISDN: " << ulMSISDN << " CalledStationId: " << CalledStationId);
	}
	else
	{
		VRadReq_countErr++;
		if(isopt('d'))
			MT_LOGERROR("(VRadius_request/VRadiusGetDataByIpFull): "<< msg[vradius_error_code] << " for IP:" << get_ipaddr(flow5->srcaddr));
	}
	VRadiusReplyDestroy(&reply);
}

void NetflowPr::SendInfo2TemplateProcessor(void)
{
	char buff[512];
	if(isopt('d'))
	{
		sprintf(buff,"SendInfo2TemplateProcessor: Command::GET_SETTING_WITH_SEARCH_BASE \"NFPARCER1\" MSISDN=%s CalledStationId=%s \"wap,mms;wap\"",ulMSISDN,CalledStationId);
		MT_LOGINFO(buff);
	}

//	CommandPtr command = new Command (Command::GET_SETTING_WITH_SEARCH_BASE,"NFPARCER1",ulMSISDN,CalledStationId,"wap,mms;wap");

	CommandPtr command = new Command(
									Command::GET_SETTING_WITH_SEARCH_BASE,
									conf->vTemplCnt[0].iD.c_str(),	// next nfparser must do get from conf  and run with other conf. -c conffile
									ulMSISDN,						// this
									CalledStationId,				// this
									"wap,mms;wap"					// this
									);

	cmndSender.sendCommand(command);
}


NetflowPr::NetflowPr(void)
: cmndSender (CommandSender(	conf->vTemplCnt[0].slhost.c_str(),   // Templ
								conf->vTemplCnt[0].lport,
								conf->vTemplCnt[0].sdhost.c_str(),
								conf->vTemplCnt[0].dport))
{
    MT_LOGINIT(NetflowPr);

	uint16_t sizeip = sizeof("255.255.255.255");
	char sdhost[sizeip];
	char username[128];
	char sharedSecret[128];
	char slhost[sizeip];

	strcpy(sdhost,conf->vVRadCnt[0].sdhost.c_str());
	strcpy(username,conf->vVRadCnt[0].username.c_str());
	strcpy(sharedSecret,conf->vVRadCnt[0].sharedSecret.c_str());
	strcpy(slhost,conf->vVRadCnt[0].slhost.c_str());

	if( VRadiusInit (
						&vrc,				  		// VRADIUSCLIENT *client,
						sdhost,						// char* hostname,  	//dhost
						conf->vVRadCnt[0].dport,     // int acctPort,		//dport
						username,  					// char* username,
						sharedSecret,  				// char* sharedSecret,
						slhost 						// char* localhost);	//lhost
					) == 0)
	{
		MT_LOGERROR(LTRED<<"Error while initializing VRADIUS interface"<<CLSTD);
	}

	VRadReq_countOk = VRadReq_countErr = cur_count = 0LL;
	fl_count = 0;
	flow_sequence_current = sys_uptime = 0L;

	m_SZHeader = sizeof(Nfv5_Header);
	m_SZFlow = sizeof(Nfv5_Flow);
}

NetflowPr::~NetflowPr()
{
	VRadiusDestroy(&vrc);
}

void NetflowPr::ntoh_hdr5(void)
{
  	header5->count				= ntohs (header5->count);
	header5->version			= ntohs (header5->version);
  	header5->SysUptime			= ntohl (header5->SysUptime);
  	header5->unix_secs			= ntohl (header5->unix_secs);
  	header5->unix_nsecs			= ntohl (header5->unix_nsecs);
  	header5->flow_sequence		= ntohl (header5->flow_sequence);
	header5->engine_tag        	= ntohs (header5->engine_tag);
	header5->sampling_interval 	= ntohs (header5->sampling_interval);
}

void NetflowPr::ntoh_flow5(void)
{
    flow5->srcaddr   =  ntohl(flow5->srcaddr);
 	flow5->dstaddr   =  ntohl(flow5->dstaddr);
 	flow5->nexthop   =  ntohl(flow5->nexthop);
	flow5->input     =  ntohs(flow5->input);
	flow5->output    =  ntohs(flow5->output);
	flow5->dPkts     =  ntohl(flow5->dPkts);
	flow5->dOctets   =  ntohl(flow5->dOctets);
	flow5->First     =  ntohl(flow5->First);
	flow5->Last      =  ntohl(flow5->Last);
	flow5->srcport   =  ntohs(flow5->srcport);
	flow5->dstport   =  ntohs(flow5->dstport);
	//flow5->pad1;                       uint8_t   pad1;
	//flow5->tcp_flags;                  uint8_t   tcp_flags;
	//flow5->prot;                       uint8_t   prot;
	//flow5->tos;                        uint8_t   tos;
	flow5->src_as    =  ntohs(flow5->src_as);
	flow5->dst_as    =  ntohs(flow5->dst_as);
	//flow5->src_mask;                   uint8_t   src_mask;
	//flow5->dst_mask;                   uint8_t   dst_mask;
	flow5->pad2      =  ntohs(flow5->pad2);
} // inet_ntoa  struct in_addr addr

char*  NetflowPr::get_ipaddr (unsigned long addr)
{
	in_addr address;
	static char	ipaddr[sizeof("255.255.255.255")+1];

	address.s_addr = htonl(addr);
	strcpy(ipaddr,	inet_ntoa(address));

/*	sprintf (ipaddr, "%u.%u.%u.%u",
				(unsigned char)  addr,
            	(unsigned char) (addr >> 8),
            	(unsigned char) (addr >> 16),
            	(unsigned char) (addr >> 24));
*/
  	return (ipaddr);
}

void NetflowPr::Parser(int sock)
{
	uint64_t byteRecv = 0LL;
	uint64_t packetsRecv = 0LL;
	uint64_t flowsRecv = 0LL;
	uint64_t ByteRecv = 0LL;
	uint64_t PacketsRecv = 0LL;
	uint64_t FlowsRecv = 0LL;

	time_t Timesave;
	time_t Timecurr;

	int sizebuffsock = (m_SZHeader + conf->maxflows*m_SZFlow);
	char buffout[512];

	Timecurr = Timesave = gettimesec();

	for (;;)
	{
		if(!(m_buffsock = (uint8_t*)malloc(sizebuffsock)))
		{
			MT_LOGERROR(LTRED<<"allocate memory for m_buffsock " << sizebuffsock <<CLSTD);
			return;
		}

		int32_t byte_recv = recvfrom (sock, m_buffsock, sizebuffsock, 0, NULL, NULL);

  		if (byte_recv <= 0)
  		{
	  		MT_LOGERROR(LTRED<<"recvfrom header " << strerror (errno)<<CLSTD);
	  		MT_LOGERROR(LTRED<<"increase maxflows parameter in options ...:" << byte_recv << " bytes");
       	}
  		else
  		{
  			header5 = (Nfv5_Header *) m_buffsock;
	  		ntoh_hdr5(); //V5 V7

  			if(isopt('d'))
  				MT_LOGINFO("RECV...:" << byte_recv << " bytes");

  			PacketsRecv++;
  			FlowsRecv += (uint64_t) header5->count;
			ByteRecv += (uint64_t) byte_recv;

			packetsRecv++;
			flowsRecv += (uint64_t) header5->count;
			byteRecv += (uint64_t) byte_recv;

			switch (header5->version)
			{
			case 5:
				out_header5();
				Parse_v5();
				break;
			case 7:
				//out_header7();
				Parse_v7();
				break;
			case 9:
				//out_header9();
				Parse_v9();
				break;
			default:
				MT_LOGERROR(LTRED<<"Flow version: " << header5->version << "... Ignoring packet"<<CLSTD);
					continue;
			}

			// output stats for VRADIUS request and data received
			time_t iTime = Timecurr - Timesave; //interval time

			if ( iTime >= conf->m_timeoutstat )
			{
				if (isopt('s'))
				{
					sprintf(buffout,": Timeout: %lu s. VRadiusRequest: Ok:%llu | E:%llu All:(%llu)",
												conf->m_timeoutstat,
												VRadReq_countOk, VRadReq_countErr,
												VRadReq_countOk + VRadReq_countErr);

					MT_LOGINFO(LTCYAN<<"Stat: "<<buffout<<CLSTD);
					conf->flStatstream<<getdate(date,2,"%Y%m%d %H%M%S")<<buffout<<"\n";

					uint64_t kbrcv = byteRecv/1024;
					sprintf(buffout,"%7.2f p/s [%llu] | %7.2f f/s [%llu] | %7.2f Kb/s [%llu]",
											((float)(packetsRecv)/(float)iTime), packetsRecv,
											((float)(flowsRecv)/(float)iTime), flowsRecv,
											(float)kbrcv/(float)iTime, kbrcv);
					MT_LOGINFO(LTCYAN<<"Stat: "<<buffout<<CLSTD);
					conf->flStatstream<<"\t"<<buffout<<"\n";

					kbrcv = ByteRecv/1024;
					sprintf(buffout,"Total: %llu p %llu f %llu Kb", PacketsRecv, FlowsRecv, kbrcv);
					MT_LOGINFO(LTCYAN<<"Stat: "<<buffout<<CLSTD);
					conf->flStatstream<<"\t"<<buffout<<"\n";
					conf->flStatstream.flush();

					Timesave = gettimesec();
					byteRecv = packetsRecv = flowsRecv = 0; // Drop counts
				}//if stat
			}//if timeout
  		}//if recv

  		Timecurr = gettimesec();
  		free(m_buffsock);
  		m_buffsock = NULL;
	}//for
}

void NetflowPr::Parse_v5(void)
{
	//MT_LOGINFO(LTGREEN<<"flow_sequence_current:" << flow_sequence_current << "header5->flow_sequence:" << header5->flow_sequence <<CLSTD);
	if (flow_sequence_current != header5->flow_sequence)
	{
		if (flow_sequence_current == 0)
			flow_sequence_current = header5->flow_sequence;
		else if (isopt('w'))
		{
			MT_LOGINFO(LTCYAN<<"Warning! flow_sequence_current: " << flow_sequence_current << " != "<< "header5->flow_sequence: " << header5->flow_sequence << " lost" << (header5->flow_sequence - flow_sequence_current) << "packets! or source nflows changed."<<CLSTD);
			if(isopt('D'))
				syslog(LOG_NOTICE,"Warning! Packets lost! Or source nflows changed.");
		}

		flow_sequence_current = header5->flow_sequence;
	}

	if (sys_uptime > header5->SysUptime)
	{
		if (isopt('d'))
			MT_LOGINFO(LTBLUE<<"warning! router was rebooted or reinitialized!"<<CLSTD);
	}

	sys_uptime = header5->SysUptime;

	for (fl_count = 0; fl_count < header5->count; fl_count++)
	{
		flow5 = (Nfv5_Flow *) (m_buffsock + m_SZHeader + fl_count * m_SZFlow);

		ntoh_flow5();
		out_flow5();

		for(std::vector<Filter>::iterator it = conf->vFilters.begin(); it != conf->vFilters.end(); it++)
		{
			if (it->Check(flow5->srcaddr, flow5->dstaddr,	flow5->dstport, flow5->tos, flow5->prot))
			{
				VRadius_request(); // vradius_error_code
				if (vradius_error_code == 0)
					SendInfo2TemplateProcessor();
				else
					MT_LOGERROR(LTRED<<"VRadius request. Return vradius_error_code:"<< vradius_error_code <<" "<<msg[vradius_error_code]<<CLSTD);
				break;
			}
		}
		flow_sequence_current += header5->count;
	}//for
}

void NetflowPr::Parse_v7(void)
{
	MT_LOGINFO(LTBLUE<<"Netflow version 7 temporary not implemented"<<CLSTD);
}

void NetflowPr::Parse_v9(void)
{
	MT_LOGINFO("Netflow version 9 temporary not implemented");
}

void NetflowPr::out_header5(void)
{
	if(isopt('d'))
	{
		MT_LOGINFO(LTGREEN<<"Header_v5"<<CLSTD);
		MT_LOGINFO(" Netflow_ver       = " << header5->version);					//  uint16_t  version;
		MT_LOGINFO(" Flows_counts      = " << header5->count);						//  uint16_t  count;
		MT_LOGINFO(" SysUptime         = " << header5->SysUptime);					//  uint32_t  SysUptime;
		MT_LOGINFO(" Unix_secs         = " << header5->unix_secs);					//  uint32_t  unix_secs;
		MT_LOGINFO(" Unix_nsecs        = " << header5->unix_nsecs);					//  uint32_t  unix_nsecs;
		MT_LOGINFO(" Flow_sequence     = " << header5->flow_sequence);				//  uint32_t  flow_sequence;
		MT_LOGINFO(" Engine_tag        = " << (unsigned int)header5->engine_tag);	//  uint8_t	engine_tag;
		MT_LOGINFO(" Engine_id         = " << (unsigned int)header5->engine_tag);	//  uint8_t	engine_id;
		MT_LOGINFO(" Sampling_interval = " << header5->sampling_interval);			//  uint16_t  sampling_interval;
	}
	//out to datafile
	if(isopt('o'))
	{
		conf->flDatastream
			<<"Netflow_ver="<<header5->version
			<<" Flows_counts="<<header5->count
			<<" SysUptime="<<header5->SysUptime
			<<" Unix_secs="<<header5->unix_secs
			<<" Unix_nsecs="<<header5->unix_nsecs
			<<" Flow_sequence="<<header5->flow_sequence
			<<" Engine_tag="<<(unsigned int)header5->engine_tag
			<<" Engine_id="<<(unsigned int)header5->engine_tag
			<<" Sampling_interval="<<header5->sampling_interval << std::endl;
			conf->flDatastream.flush();
	}
}

void NetflowPr::out_flow5(void)
{
	if(isopt('d'))
	{
		MT_LOGINFO(LTGREEN<<"Flow_v5: "<< fl_count+1 << "(" << header5->count << ")"<<CLSTD);
		MT_LOGINFO( "srcaddr   = " << get_ipaddr(flow5->srcaddr));		// uint32_t  srcaddr;
		MT_LOGINFO( "dstaddr   = " << get_ipaddr (flow5->dstaddr));		// uint32_t  dstaddr;
		MT_LOGINFO( "nexthop   = " << get_ipaddr (flow5->nexthop));		// uint32_t  nexthop;
		MT_LOGINFO( "input     = " << flow5->input);					// uint16_t  input;
		MT_LOGINFO( "output    = " << flow5->output);					// uint16_t  output;
		MT_LOGINFO( "dPkts     = " << flow5->dPkts);					// uint32_t  dPkts;
		MT_LOGINFO( "dOctets   = " << flow5->dOctets);					// uint32_t  dOctets;
		MT_LOGINFO( "First     = " << flow5->First);					// uint32_t  First;
		MT_LOGINFO( "Last      = " << flow5->Last);						// uint32_t  Last;
		MT_LOGINFO( "srcport   = " << flow5->srcport);					// uint16_t  srcport;
		MT_LOGINFO( "dstport   = " << flow5->dstport);					// uint16_t  dstport;
		MT_LOGINFO( "pad1      = " << (unsigned int)flow5->pad1);		// uint8_t   pad1;
		MT_LOGINFO( "tcp_flags = " << (unsigned int)flow5->tcp_flags);	// uint8_t   tcp_flags;
		MT_LOGINFO( "prot      = " << (unsigned int)flow5->prot);		// uint8_t   prot;
		MT_LOGINFO( "tos       = " << (unsigned int)flow5->tos);		// uint8_t   tos;
		MT_LOGINFO( "src_as    = " << flow5->src_as);					// uint16_t  src_as;
		MT_LOGINFO( "dst_as    = " << flow5->dst_as);					// uint16_t  dst_as;
		MT_LOGINFO( "src_mask  = " << (unsigned int)flow5->src_mask);	// uint8_t   src_mask;
		MT_LOGINFO( "dst_mask  = " << (unsigned int)flow5->dst_mask);	// uint8_t   dst_mask;
		MT_LOGINFO( "pad2      = " << flow5->pad2);						// uint16_t  pad2;
// add info from header
		MT_LOGINFO( "-- timestamp = " << header5->unix_secs);
	}

	// out to datafile
	if(isopt('o'))
	{
		conf->flDatastream
			<< "srcaddr="<<get_ipaddr(flow5->srcaddr)   		// uint32_t  srcaddr;
			<<" dstaddr="<<get_ipaddr (flow5->dstaddr)        	// uint32_t  dstaddr;
			<<" nexthop="<<get_ipaddr (flow5->nexthop)        	// uint32_t  nexthop;
			<<" input="<<flow5->input                        	// uint16_t  input;
			<<" output="<<flow5->output                      	// uint16_t  output;
			<<" dPkts="<<flow5->dPkts				       		// uint32_t  dPkts;
			<<" dOctets="<<flow5->dOctets						// uint32_t  dOctets;
			<<" First="<<flow5->First 					    	// uint32_t  First;
			<<" Last="<<flow5->Last                          	// uint32_t  Last;
			<<" srcport="<<flow5->srcport                     	// uint16_t  srcport;
			<<" dstport="<<flow5->dstport                     	// uint16_t  dstport;
			<<" pad1="<<(unsigned int)flow5->pad1				// uint8_t   pad1;
			<<" tcp_flags="<<(unsigned int)flow5->tcp_flags		// uint8_t   tcp_flags;
			<<" prot="<<(unsigned int)flow5->prot               // uint8_t   prot;
			<<" tos="<<(unsigned int)flow5->tos	  			  	// uint8_t   tos;
			<<" src_as="<<flow5->src_as					    	// uint16_t  src_as;
			<<" dst_as="<<flow5->dst_as					    	// uint16_t  dst_as;
			<<" src_mask="<<(unsigned int)flow5->src_mask       // uint8_t   src_mask;
			<<" dst_mask="<<(unsigned int)flow5->dst_mask       // uint8_t   dst_mask;
			<<" pad2="<<flow5->pad2                           	// uint16_t  pad2;
			<<" timestamp="<<header5->unix_secs << std::endl;	// add info from header
			conf->flDatastream.flush();
	}
}
