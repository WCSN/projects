/*
 * NflowProc.h
 *
 *  Created on: 11.02.2011
 *      Author: wocson
 */

#ifndef NFLOWPROC_H_
#define NFLOWPROC_H_

#include <mt/logger/Logable.h>
#include <mt/common/DynamicBuffer.h>
#include <mt/tools/ReloadableConfig.h>

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

struct Nfv5_Header{
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

struct Nfv5_Flow{
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

#pragma pack()

struct NetFlowPack_v5
{
	Nfv5_Header *header;
	std::vector <Nfv5_Flow> flows;

	NetFlowPack_v5(void):header(NULL){}

	void clear(void)
	{
		header = NULL;
		flows.clear();
	}
};

class FilterProc;

class NflowProc: public mt::Logable {
public:
	NflowProc(void);
	virtual ~NflowProc();

	NetFlowPack_v5 nfParser(const mt::ByteBuffer& nflowbuff);
	uint32_t getCountFlows(void);

private:

	uint16_t m_count;
	void ntoh_hdr5(Nfv5_Header *header);
	void ntoh_flow5(Nfv5_Flow *flow);
};


#endif /* NFLOWPROC_H_ */
