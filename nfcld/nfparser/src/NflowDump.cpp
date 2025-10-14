/*
 * NflowDump.cpp
 *
 *  Created on: 18.03.2011
 *      Author: wocson
 */

#include "NflowDump.h"

NflowDump::NflowDump(const mt::XmlConfigPtr& options)
{
	MT_LOGINIT(NflowDump);

	mt::XmlConfigPtr cfgsub = options->getConfig("NfParser");

	std::string valpar = cfgsub->get("dumpfilepath");
	if (valpar.length())
	{
		m_fileDumpNf.open(valpar.c_str(),
				std::fstream::ate | std::fstream::out | std::fstream::app);
		if (m_fileDumpNf.is_open())
		{
			MT_LOGINFO("Dump file: " << valpar << " open sussed");
		}
		else
		{
			MT_LOGERROR("Dump file: " << valpar << " open failed...");
		}
	}
}

NflowDump::~NflowDump()
{
	m_fileDumpNf.close();
}

void NflowDump::DropData(const NetFlowPack_v5& nflowpack)
{

MT_LOGTRACE("Parse flowpacket. header:"
		<< "\nversion          : " << nflowpack.header->version
		<< "\ncount            : " << nflowpack.header->count
		<< "\nSysUptime        : " << nflowpack.header->SysUptime
		<< "\nunix_secs        : " << nflowpack.header->unix_secs
		<< "\nunix_nsecs       : " << nflowpack.header->unix_nsecs
		<< "\nflow_sequence    : " << nflowpack.header->flow_sequence
		<< "\nengine_tag       : " << (uint16_t)nflowpack.header->engine_tag
		<< "\nengine_id        : " << (uint16_t)nflowpack.header->engine_id
		<< "\nsampling_interval: " << nflowpack.header->sampling_interval << "\n");

if (m_fileDumpNf.is_open())
	m_fileDumpNf << nflowpack.header->version << " "
			<< nflowpack.header->count << " "
			<< nflowpack.header->SysUptime << " "
			<< nflowpack.header->unix_secs << " "
			<< nflowpack.header->unix_nsecs << " "
			<< nflowpack.header->flow_sequence << " "
			<< nflowpack.header->engine_tag << " "
			<< nflowpack.header->engine_id << " "
			<< nflowpack.header->sampling_interval << std::endl;

		std::vector<Nfv5_Flow>::const_iterator itflow;
		 uint16_t fcount=0;

		for (itflow = nflowpack.flows.begin(); itflow	!= nflowpack.flows.end(); itflow++,fcount++)
		{
			MT_LOGTRACE("Parse flow ("<<fcount<<"):"
				<<"\nsrcaddr  : " << Srvfunc().getIpAddrDotOct(itflow->srcaddr)
				<<"\ndstaddr  : " << Srvfunc().getIpAddrDotOct(itflow->dstaddr)
				<<"\nnexthop  : " << Srvfunc().getIpAddrDotOct(itflow->nexthop)
				<<"\ninput    : " << itflow->input
				<<"\noutput   : " << itflow->output
				<<"\ndPkts    : " << itflow->dPkts
				<<"\ndOctets  : " << itflow->dOctets
				<<"\nFirst    : " << itflow->First
				<<"\nLast     : " << itflow->Last
				<<"\nsrcport  : " << itflow->srcport
				<<"\ndstport  : " << itflow->dstport
				<<"\npad1     : " << (uint16_t)itflow->pad1
				<<"\ntcp_flags: " << (uint16_t)itflow->tcp_flags
				<<"\nprot     : " << (uint16_t)itflow->prot
				<<"\ntos      : " << (uint16_t)itflow->tos
				<<"\nsrc_as   : " << itflow->src_as
				<<"\ndst_as   : " << itflow->dst_as
				<<"\nsrc_mask : " << (uint16_t)itflow->src_mask
				<<"\ndst_mask : " << (uint16_t)itflow->dst_mask
				<<"\npad2     : " << itflow->pad2 << "\n");

		if (m_fileDumpNf.is_open())
			m_fileDumpNf << Srvfunc().getIpAddrDotOct(itflow->srcaddr) << " "
					<< Srvfunc().getIpAddrDotOct(itflow->dstaddr) << " "
					<< Srvfunc().getIpAddrDotOct(itflow->nexthop) << " "
					<< itflow->input << " " << itflow->output << " "
					<< itflow->dPkts << " " << itflow->dOctets << " "
					<< itflow->First << " " << itflow->Last << " "
					<< itflow->srcport << " " << itflow->dstport << " "
					<< itflow->pad1 << " " << itflow->tcp_flags << " "
					<< itflow->prot << " " << itflow->tos << " "
					<< itflow->src_as << " " << itflow->dst_as << " "
					<< itflow->src_mask << " " << itflow->dst_mask << " "
					<< itflow->pad2 << std::endl;
	}
}
