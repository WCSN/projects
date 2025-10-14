readme.txt

 Created on: 25.11.2010
     Author: wocson
     

format cisco nflow:





Typical flow (version 5):
--HEAD--
 Netflow_ver       = 5              NetFlow export format version number
 Flows_counts      = 1              Number of flows exported in this packet (1-30)
 SysUptime         = 274775924      Current time in milliseconds since the export device booted
 Unix_secs         = 1015215127     Current count of seconds since 0000 UTC 1970
 Unix_nsecs        = 792698874      Residual nanoseconds since 0000 UTC 1970
 Flow_sequence     = 41578          Sequence counter of total flows seen
 Engine_tag        = 0              Type of flow-switching engine
 Engine_id         = 0              Slot number of the flow-switching engine
 Sampling_interval = 0              First two bits hold the sampling mode; remaining 14 bits hold value of sampling interval
 --FLOW--
srcaddr   = 10.255.2.184        Source IP address
dstaddr   = 10.255.2.153        Destination IP address
nexthop   = 0.0.0.0     		IP address of next hop router
input     = 2                   SNMP index of input interface
output    = 0                   SNMP index of output interface
dPkts     = 1                   Packets in the flow
dOctets   = 224                 Total number of Layer 3 bytes in the packets of the flow
First     = 274750923           SysUptime at start of flow
Last      = 274750923           SysUptime at the time the last packet of the flow was received
srcport   = 0                   TCP/UDP source port number or equivalent
dstport   = 771                 TCP/UDP destination port number or equivalent
pad1      = 0                   pad1 Unused (zero) bytes
tcp_flags = 16                  tcp_flags Cumulative OR of TCP flags
prot      = 1                   prot IP protocol type (for example, TCP = 6; UDP = 17)
tos       = 0                   tos IP type of service (ToS)
src_as    = 0                   src_as Autonomous system number of the source, either origin or peer
dst_as    = 0                   dst_as Autonomous system number of the destination, either origin or peer
src_mask  = 24                  src_mask Source address prefix mask bits
dst_mask  = 24                  dst_mask Destination address prefix mask bits
pad2      = 0                   pad2 Unused (zero) bytes

Vraduis request:

Get info by src_IP 10.255.2.254 :
          MSISDN: 89211234567


Config file example:

<?xml version="1.0" encoding="UTF-8"?>
<config>
	<Options>
		<Datafile dfname="/home/wocson/nfparser.dat"/>
		<Pidfile pfile="/var/run/nfparser.pid"/>
	</Options>

	<ConnectPar>
	
		<VRadIP lhost="10.255.2.99" dhost="10.255.2.233" dport="3813" username="nfparser" sharedSecret="secret"/>
		<TemplIP lhost="0.0.0.0" lport="0" dhost="10.255.2.99" dport="7777" />	
		<NflsrcIP dhost="0.0.0.0" dport="9993"/>
		
	</ConnectPar>

	<Filters>	
    	<srcNet net="10.255.2.0/24" operation="included"/> <!-- operation="included" / operation="excluded"-->  
    	<srcNet net="10.255.2.0/24" operation="included"/>  
    
    	<dstNet net="10.255.2.0/24" operation="included"/>  
    	<dstNet net="10.255.2.0/24" operation="included"/>
    
    	<dstport port="137" operation="included"/> 
    	<dstport port="138" operation="included"/>
    	<dstport port="771" operation="included"/>
    	<dstport port="1947" operation="included"/> 
    	       
    	<protocol prot="17" operation="included"/> 
    	<protocol prot="6" operation="included"/>
    	<protocol prot="1" operation="excluded"/>
    
    	<typeoS tos="0" operation="included"/> 
    	<typeoS tos="192" operation="included"/>
  </Filters>
</config>

