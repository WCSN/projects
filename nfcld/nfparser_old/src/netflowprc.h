/*
 * netflowprc.h
 *
 *  Created on: 10.11.2010
 *      Author: wocson
 */

#ifndef NETFLOWPRC_H_
#define NETFLOWPRC_H_

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

// Mobitech include.

#include <vradius_client.h>
#include <Command.h>
#include <CommandSender.h>
#include <mt/common/XmlConfig.h>
#include <mt/utils/System.h>
#include <mt/logger/LoggerFactory.h>
#include <mt/logger/Logable.h>
#include <mt/tools/ReloadableConfig.h>
#include <mt/tools/Application.h>

#include "filter.h"
#include "conf.h"

#define NETFLOW_V5_HEADER_LENGTH 24
#define NETFLOW_V5_RECORD_LENGTH 48
#define NETFLOW_V5_MAX_RECORDS	 30

#define NETFLOW_V7_HEADER_LENGTH 24
#define NETFLOW_V7_RECORD_LENGTH 52
#define NETFLOW_V7_MAX_RECORDS   28

/* v5 structures */
/*
http://netflow.caligare.com/netflow_v5.htm
*/
#pragma pack(1)

struct Nfv5_Header
{
  uint16_t  version;				// NetFlow export format version number
  uint16_t  count;					// Number of flows exported in this packet (1-30)
  uint32_t  SysUptime;				// Current time in milliseconds since the export device booted
  uint32_t  unix_secs;				// Current count of seconds since 0000 UTC 1970
  uint32_t  unix_nsecs;				// Residual nanoseconds since 0000 UTC 1970
  uint32_t  flow_sequence;			// Sequence counter of total flows seen
  uint8_t	engine_tag;				// Type of flow-switching engine
  uint8_t	engine_id;				// Slot number of the flow-switching engine
  uint16_t  sampling_interval;		// First two bits hold the sampling mode; remaining 14 bits hold value of sampling interval
};

struct Nfv5_Flow
{
  uint32_t  srcaddr;			// Source IP address
  uint32_t  dstaddr;            // Destination IP address
  uint32_t  nexthop;            // IP address of next hop router
  uint16_t  input;              // SNMP index of input interface
  uint16_t  output;             // SNMP index of output interface
  uint32_t  dPkts;              // Packets in the flow
  uint32_t  dOctets;			// Total number of Layer 3 bytes in the packets of the flow
  uint32_t  First;              // SysUptime at start of flow
  uint32_t  Last;               // SysUptime at the time the last packet of the flow was received
  uint16_t  srcport;            // TCP/UDP source port number or equivalent
  uint16_t  dstport;            // TCP/UDP destination port number or equivalent
  uint8_t   pad1;				//    pad1 Unused (zero) bytes
  uint8_t   tcp_flags;          // tcp_flags Cumulative OR of TCP flags
  uint8_t   prot;               // prot IP protocol type (for example, TCP = 6; UDP = 17)
  uint8_t   tos;                // tos IP type of service (ToS)
  uint16_t  src_as;				// 41 src_as Autonomous system number of the source, either origin or peer
  uint16_t  dst_as;             // 43 dst_as Autonomous system number of the destination, either origin or peer
  uint8_t   src_mask;           // src_mask Source address prefix mask bits
  uint8_t   dst_mask;           // dst_mask Destination address prefix mask bits
  uint16_t  pad2;               //    pad2 Unused (zero) bytes
};


/* v7 structures */
/*
struct Nfv7_Header
{
  uint16_t  version;
  uint16_t  count;
  uint32_t  SysUptime;
  uint32_t  unix_secs;
  uint32_t  unix_nsecs;
  uint32_t  flow_sequence;
  uint32_t  reserved;
};

struct Nfv7_flow
{
  uint32_t  srcaddr;
  uint32_t  dstaddr;
  uint32_t  nexthop;
  uint16_t  input;
  uint16_t  output;
  uint32_t  dPkts;
  uint32_t  dOctets;
  uint32_t  First;
  uint32_t  Last;
  uint16_t  srcport;
  uint16_t  dstport;
  uint8_t   flags;
  uint8_t   tcp_flags;
  uint8_t   prot;
  uint8_t   tos;
  uint16_t  src_as;
  uint16_t  dst_as;
  uint8_t   src_mask;
  uint8_t   dst_mask;
  uint16_t  pad;
  uint32_t  router_sc;
};
*/
#pragma pack()


class NetflowPr: public mt::Logable
{
public:

	NetflowPr(void);
	void Parser(int sock);

	void VRadius_request(void);
	void SendInfo2TemplateProcessor(void);

	~NetflowPr();

private:

	int fl_count;

	VRADIUSCLIENT vrc;

	Nfv5_Header	*header5;
	Nfv5_Flow *flow5;

//	Nfv7_Header	*header7;
//	Nfv7_flow *flow7;

	char ulMSISDN[64];
	char CalledStationId[64];

	CommandSender cmndSender;

	uint64_t VRadReq_countOk;
	uint64_t VRadReq_countErr;

	uint32_t flow_sequence_current;
	uint32_t sys_uptime;
  	uint16_t cur_count;

    uint8_t *m_buffsock;

  	void Parse_v5(void);
  		uint32_t m_SZHeader;
  		uint32_t m_SZFlow;
  	void ntoh_hdr5(void);
  	void ntoh_flow5(void);
  	void out_header5(void);
  	void out_flow5(void);

  	void Parse_v7(void);
  	//
  	void Parse_v9(void);
  	//
	char *get_ipaddr (unsigned long addr);
//	int Init(void);
};

#endif /* NETFLOWPRC_H_ */
