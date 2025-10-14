/*
 * NflowProcc.cpp
 *
 *  Created on: 11.02.2011
 *      Author: wocson
 */

#include "NflowProc.h"
#include "FilterProc.h"
#include <wl/utils/Srvfunc.h>

NflowProc::NflowProc(void)
{
	MT_LOGINIT(NflowProc);
}

NflowProc::~NflowProc()
{

}

void NflowProc::ntoh_hdr5(Nfv5_Header *header5)
{
	header5->count = ntohs(header5->count);
	header5->version = ntohs(header5->version);
	header5->SysUptime = ntohl(header5->SysUptime);
	header5->unix_secs = ntohl(header5->unix_secs);
	header5->unix_nsecs = ntohl(header5->unix_nsecs);
	header5->flow_sequence = ntohl(header5->flow_sequence);
	// header5->engine_tag  //uint8_t
	// header5->engine_id;	//uint8_t				// Slot number of the flow-switching engine
	header5->sampling_interval = ntohs(header5->sampling_interval);
}

void NflowProc::ntoh_flow5(Nfv5_Flow *flow5)
{
	flow5->srcaddr = ntohl(flow5->srcaddr);
	flow5->dstaddr = ntohl(flow5->dstaddr);
	flow5->nexthop = ntohl(flow5->nexthop);
	flow5->input = ntohs(flow5->input);
	flow5->output = ntohs(flow5->output);
	flow5->dPkts = ntohl(flow5->dPkts);
	flow5->dOctets = ntohl(flow5->dOctets);
	flow5->First = ntohl(flow5->First);
	flow5->Last = ntohl(flow5->Last);
	flow5->srcport = ntohs(flow5->srcport);
	flow5->dstport = ntohs(flow5->dstport);
	//flow5->pad1;                       uint8_t   pad1;
	//flow5->tcp_flags;                  uint8_t   tcp_flags;
	//flow5->prot;                       uint8_t   prot;
	//flow5->tos;                        uint8_t   tos;
	flow5->src_as = ntohs(flow5->src_as);
	flow5->dst_as = ntohs(flow5->dst_as);
	//flow5->src_mask;                   uint8_t   src_mask;
	//flow5->dst_mask;                   uint8_t   dst_mask;
	flow5->pad2 = ntohs(flow5->pad2);
}

NetFlowPack_v5 NflowProc::nfParser(const mt::ByteBuffer& nflowbuff)
{
	NetFlowPack_v5 nflowpack;

	nflowpack.header = (Nfv5_Header *) ((uint8_t *) nflowbuff.getDataPtr());

	ntoh_hdr5(nflowpack.header);

	m_count = nflowpack.header->count;

	if (nflowpack.header->version == 5)
	{

		for (uint16_t fcount = 0; fcount < nflowpack.header->count; fcount++)
		{

			Nfv5_Flow *flow = (Nfv5_Flow *) ((uint8_t *) (nflowbuff.getDataPtr()
									+ sizeof(Nfv5_Header)
									+ sizeof(Nfv5_Flow)	* fcount));

			ntoh_flow5(flow);
			nflowpack.flows.push_back(*flow);
		}
	}

	return nflowpack;
}

uint32_t NflowProc::getCountFlows(void)
{
	return m_count;
}
