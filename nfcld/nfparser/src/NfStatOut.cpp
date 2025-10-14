/*
 * NfStatOut.cpp
 *
 *  Created on: 11.02.2011
 *      Author: wocson
 */

#include "NfStatOut.h"
#include <wl/utils/Srvfunc.h>

NfStatOut::NfStatOut(const mt::XmlConfigPtr& options) :
	m_timeOut(0), m_currenttime(0), m_savetime(Srvfunc().getUnixTime()),
			m_countpackets(0), m_countflows(0), m_countVradiusReq(0),
			m_countVradiusSussedResp(0), m_countVradiusErrorResp(0),
			m_countTemplCmdSend(0), m_countrecvBytes(0) {

	MT_LOGINIT(NfStatOut);

	mt::XmlConfigPtr cfgsub = options->getConfig("Statistic");

	m_timeOut = atoi(cfgsub->get("timeout").c_str());

	std::string valpar = cfgsub->get("statfilepath");
	if(valpar.length()){
		m_fileStat.open(valpar.c_str(), std::fstream::ate | std::fstream::out | std::fstream::app);
	if (m_fileStat.is_open()){
		MT_LOGINFO("Stat file: " <<  valpar << " open sussed");}
	else{
		MT_LOGERROR("Stat file: " <<  valpar << " open failed...");}
	}
}

NfStatOut::~NfStatOut() {

}

void NfStatOut::regPackets(uint32_t recvbytes) {
	m_countpackets++;
	m_countrecvBytes += recvbytes;
}

void NfStatOut::regFlows(uint32_t flows) {
	m_countflows++;
}

void NfStatOut::regVradiusReq(void) {
	m_countVradiusReq++;
}

void NfStatOut::regVradiusSussedResp(void) {
	m_countVradiusSussedResp++;
}

void NfStatOut::regVradiusErrorResp(void) {
	m_countVradiusErrorResp++;
}

void NfStatOut::regTemplCmdSend(void) {
	m_countTemplCmdSend++;
}

void NfStatOut::outStats(void) {
	m_currenttime = Srvfunc().getUnixTime();

	if ((m_currenttime - m_savetime) >= m_timeOut) {

		MT_LOGINFO(" VRaduis: Requests: "<< m_countVradiusReq << " Sussed: " << m_countVradiusSussedResp << " Error: "<< m_countVradiusErrorResp);
		MT_LOGINFO(" Request to TemplateProcessor: "<< m_countTemplCmdSend);
		time_t timeinterval = (m_currenttime - m_savetime);
		MT_LOGINFO(" Packets: "<< m_countpackets << " Flows: "<< m_countflows <<" "<< (float)m_countpackets/(float)timeinterval<<" Packets/sec "<< (float)m_countflows/(float)timeinterval << " flows/sec");

		if(m_fileStat.is_open()){
			m_fileStat << Srvfunc().getTime2Log("%Y-%m-%d %H:%M:%S");
			m_fileStat << " VRaduis: Requests: "<< m_countVradiusReq << " Sussed: " << m_countVradiusSussedResp << " Error: "<< m_countVradiusErrorResp;
			m_fileStat << " Request to TemplateProcessor: "<< m_countTemplCmdSend;
			m_fileStat << " Packets: "<< m_countpackets << " Flows: "<< m_countflows <<" "<< (float)m_countpackets/(float)timeinterval<<" Packets/sec "<< (float)m_countflows/(float)timeinterval << " flows/sec";
			m_fileStat << std::endl;
		}

		resetCounts();
	}
}

void NfStatOut::resetCounts() {

	m_savetime = m_currenttime = Srvfunc().getUnixTime();

	//m_countpackets;
	//m_countflows;
	//m_countVradiusReq;
	//m_countVradiusSussedResp;
	//m_countVradiusErrorResp;
	//m_countTemplCmdSend;
	//m_countrecvBytes;
}
