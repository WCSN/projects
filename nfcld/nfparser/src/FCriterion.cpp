/*
 * FCriterion.cpp
 *
 *  Created on: 10.02.2011
 *      Author: wocson
 */

#include "FCriterion.h"
#include <wl/utils/Srvfunc.h>

FCriterion::FCriterion(const mt::XmlConfigPtr& cfg) :
	m_operation((cfg->get("operation") == "include") ? INCLUDE : EXCLUDE) {
	MT_LOGINIT(FCriterion);
	MT_LOGTRACE("Init Criterion");
}

//#define DEBUG 0

bool FCriterion::check(const Nfv5_Flow& flow) {
	MT_LOGTRACE("Checked criterion...");

#ifdef DEBUG
	bool st_alpcrit = isCriterionApplied(flow);
	bool st_oper = m_operation == INCLUDE;
	bool st_check = st_oper == st_alpcrit;

	if(st_check && DEBUG)
		isCriterionApplied(flow);

return (st_check);

#else

return ((m_operation == INCLUDE) == isCriterionApplied(flow));
#endif
}

// added source network in filter
FCriterionSrcNet::FCriterionSrcNet(const mt::XmlConfigPtr& cfg) :
	FCriterion(cfg) {
	MT_LOGINIT(FCriterionSrcNet);
	std::string valpar = cfg->get("network");
	uint slash_pos = valpar.find("/");
	m_ui32addr = Srvfunc().getIpAddrUint32(valpar.substr(0, slash_pos));
	m_ui32mask = 0xffffffff << (32 - (atoi(valpar.substr(slash_pos + 1).c_str())));
	MT_LOGTRACE("ScrcNet " << valpar);
}

FCriterionSrcNet::~FCriterionSrcNet() {

}

bool FCriterionSrcNet::isCriterionApplied(const Nfv5_Flow& flow) {
	MT_LOGTRACE("Checked srcIP address: "<< Srvfunc().getIpAddrDotOct(flow.srcaddr));
	bool st_check = (m_ui32addr == (flow.srcaddr & m_ui32mask));
	if(st_check)
		MT_LOGTRACE("Check passed " << m_ui32addr << "==" << (flow.srcaddr & m_ui32mask));
	return st_check;
}

// added destination network in filter
FCriterionDstNet::FCriterionDstNet(const mt::XmlConfigPtr& cfg) :
	FCriterion(cfg) {
	MT_LOGINIT(FCriterionDstNet);
	std::string valpar = cfg->get("network");
	uint slash_pos = valpar.find("/");
	m_ui32addr = Srvfunc().getIpAddrUint32(valpar.substr(0, slash_pos));
	m_ui32mask = 0xffffffff << (32 - (atoi(valpar.substr(slash_pos + 1).c_str())));
	MT_LOGTRACE("DstNet " <<valpar);
}

FCriterionDstNet::~FCriterionDstNet() {

}

bool FCriterionDstNet::isCriterionApplied(const Nfv5_Flow& flow) {
	MT_LOGTRACE("Checked dstIP address: "<< Srvfunc().getIpAddrDotOct(flow.dstaddr));
	bool st_check = (m_ui32addr == (flow.dstaddr & m_ui32mask));
	if(st_check)
		MT_LOGTRACE("Check passed " << m_ui32addr << "==" << (flow.dstaddr & m_ui32mask));
return st_check;
}
/////////////////////////////
// added destination port in filter
FCriterionPort::FCriterionPort(const mt::XmlConfigPtr& cfg) :
	FCriterion(cfg) {
	MT_LOGINIT(FCriterionPort);
	m_ui16port = atoi(cfg->get("value").c_str());
	MT_LOGTRACE("dstPort " << m_ui16port);
}

FCriterionPort::~FCriterionPort() {

}

bool FCriterionPort::isCriterionApplied(const Nfv5_Flow& flow) {
	MT_LOGTRACE("Checked dstPort: "<< flow.dstport);
	bool st_check = (m_ui16port == flow.dstport);
	if(st_check)
		MT_LOGTRACE("Check passed " << m_ui16port << "==" << flow.dstport);
return st_check;
}

/////////////////////////////
// added destination protocol in filter
FCriterionProtocol::FCriterionProtocol(const mt::XmlConfigPtr& cfg) :
	FCriterion(cfg) {
	MT_LOGINIT(FCriterionProtocol);
	m_ui8protocol = atoi(cfg->get("value").c_str());
	MT_LOGTRACE("Protocol: " << (uint16_t)m_ui8protocol);
}

FCriterionProtocol::~FCriterionProtocol() {

}

bool FCriterionProtocol::isCriterionApplied(const Nfv5_Flow& flow) {
	MT_LOGTRACE("Checked Protocol: "<< (uint16_t)flow.prot);
	bool st_check = (m_ui8protocol == flow.prot);
	if(st_check)
		MT_LOGTRACE("Check passed " << (uint16_t)m_ui8protocol << "==" << (uint16_t)flow.prot);
return st_check;
}

/////////////////////////////
// added destination tos in filter
FCriterionTos::FCriterionTos(const mt::XmlConfigPtr& cfg) :
	FCriterion(cfg) {
	MT_LOGINIT(FCriterionTos);
	m_ui8tos = atoi(cfg->get("value").c_str());
	MT_LOGTRACE("TOS: " << (uint16_t)m_ui8tos);
}

FCriterionTos::~FCriterionTos() {

}

bool FCriterionTos::isCriterionApplied(const Nfv5_Flow& flow) {
	MT_LOGTRACE("Checked tos: "<< (uint16_t)flow.tos);
	bool st_check = (m_ui8tos == flow.tos);
	if(st_check)
		MT_LOGTRACE("Check passed " << (uint16_t)m_ui8tos << "==" << (uint16_t)flow.tos);
return st_check;
}
