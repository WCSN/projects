/*
 * filter.cpp
 *
 *  Created on: 28.12.2010
 *      Author: wocson
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
//#include <UdpSocket.h>

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#include "netflowprc.h"
#include "nfparser.h"
#include "helper.h"
#include "conf.h"
#include "filter.h"

Filter::Filter(void)
{
    MT_LOGINIT(Filter);
}

bool Filter::ipSrc_ok(uint32_t src_addr)
{
	bool check_st = true;
	char buff[512];

//check srcNet
	for(std::vector<NetAttr>::iterator it =  vsrcNets.begin(); it!= vsrcNets.end(); it++ )
	{
		if(isopt('d'))
			sprintf(buff,"srcNet: %08X & %08X = %08X op: %u ",	it->network,
																src_addr,
																src_addr & it->netmask,
																it->op );

		check_st = ((it->network == (src_addr & it->netmask)) == it->op);

		if(isopt('d'))
			MT_LOGINFO(buff << ((check_st) ? LTGREEN "TRUE" CLSTD : LTRED "FALSE" CLSTD));
	}

	if(isopt('d'))
		MT_LOGINFO("Check srcNet: " << ((check_st) ? LTGREEN "TRUE" CLSTD : LTRED "FALSE" CLSTD));

return check_st;
}

bool Filter::ipDst_ok(uint32_t dst_addr)
{
	bool check_st = true;
	char buff[512];

//check dstNet
	for(std::vector<NetAttr>::iterator it =  vdstNets.begin(); it!= vdstNets.end(); it++ )
	{
		if(isopt('d'))
			sprintf(buff,"dstNet: %08X & %08X = %08X op: %u ",	it->network,
																dst_addr,
																dst_addr & it->netmask,
																it->op );

		check_st = ((it->network == (dst_addr & it->netmask)) == it->op);

/*
		if((dst_addr & it->netmask) == it->network)
		{
			if(it->op)
				check_st = true;
			else
				check_st = false;
		}
		else
		{
			if(it->op)
				check_st = false;
			else
				check_st = true;
		}
*/

		if(isopt('d'))
			MT_LOGINFO(buff << ((check_st) ? LTGREEN "TRUE" CLSTD : LTRED "FALSE" CLSTD));
	}

	if(isopt('d'))
		MT_LOGINFO("Check dstNet: " << ((check_st) ? LTGREEN "TRUE" CLSTD : LTRED "FALSE" CLSTD));

return check_st;
}

bool Filter::port_ok(uint16_t dstport)
{
	char buff[512];
	bool check_st = true;

	for(std::vector<NetAddPar>::iterator it =  vDstPorts.begin(); it != vDstPorts.end(); it++)
	{
		if(isopt('d'))
			sprintf(buff,"dstPort: %u <==> %u op: %d ", dstport, it->val, it->op);

		check_st = ((dstport==(uint16_t)it->val) == it->op);

		if(isopt('d'))
			MT_LOGINFO(buff << ((check_st) ? LTGREEN "TRUE" CLSTD : LTRED "FALSE" CLSTD));
	}

	if(isopt('d'))
		MT_LOGINFO("Check dstPort: " << ((check_st) ? LTGREEN "TRUE" CLSTD : LTRED "FALSE" CLSTD));

return check_st;
}

bool Filter::tos_ok(uint8_t tos)
{
	char buff[512];
	bool check_st = true;

	for(std::vector<NetAddPar>::iterator it =  vToss.begin(); it != vToss.end(); it++)
	{
		if(isopt('d'))
			sprintf(buff,"tos: %u <==> %u op: %d ", tos, it->val, it->op);

		check_st = ((tos ==	(uint8_t)it->val ) == it->op);

		if(isopt('d'))
			MT_LOGINFO(buff << ((check_st) ? LTGREEN "TRUE" CLSTD : LTRED "FALSE" CLSTD));
	}

	if(isopt('d'))
		MT_LOGINFO("Check TOS: " << ((check_st) ? LTGREEN "TRUE" CLSTD : LTRED "FALSE" CLSTD));

return check_st;
}

bool Filter::prot_ok(uint8_t prot)
{
	char buff[512];
	bool check_st = true;

	for(std::vector<NetAddPar>::iterator it =  vProts.begin(); it != vProts.end(); it++)
	{
		if(isopt('d'))
			sprintf(buff,"prot: %u <==> %u op: %d ", prot, it->val, it->op);

		check_st = ((prot == (uint8_t)it->val) == it->op);

		if(isopt('d'))
			MT_LOGINFO(buff << ((check_st) ? LTGREEN "TRUE" CLSTD : LTRED "FALSE" CLSTD));
	}

	if(isopt('d'))
		MT_LOGINFO("Check Prot: " << ((check_st) ? LTGREEN "TRUE" CLSTD : LTRED "FALSE" CLSTD));

return check_st;
}

bool Filter::Check(uint32_t srcaddr, uint32_t  dstaddr, uint16_t dstport, uint8_t tos, uint8_t prot)
{
	bool check_st = false;

	if(isopt('d'))
	{
			MT_LOGINFO(LTGREEN "Filter ID: ================= " << iD << CLSTD);
/*
			ipSrc_ok(srcaddr);
			ipDst_ok(dstaddr);
			port_ok(dstport);
			tos_ok(tos);
			prot_ok(prot);
*/
	}

		if(ipSrc_ok(srcaddr))
			if(ipDst_ok(dstaddr))
				if(port_ok(dstport))
					if (tos_ok(tos))
						if (prot_ok(prot))
							  check_st = true;

	if(isopt('d'))
		MT_LOGINFO("Check STATUS: " << ((check_st) ? LTGREEN "TRUE" CLSTD : LTRED "FALSE" CLSTD));

return check_st;
}
