/*
 * TemplateProcSender.cpp
 *
 *  Created on: 11.02.2011
 *      Author: wocson
 */

#include "TemplateProc.h"

//<TemplateProcessor lhost="0.0.0.0" lport="0" dhost="10.255.2.99" dport="7777" id="NFPARSER1" searchbase="wap,mms;wap"/>

TemplateProc::TemplateProc(const std::string& nfId, const mt::XmlConfigPtr& cfg) /*:
	m_cmdSender(CommandSender(cfg->get("lhost").c_str(),
			atoi(cfg->get("lport").c_str()), cfg->get("dhost").c_str(),
			atoi(cfg->get("dport").c_str()))), m_nfId(nfId), m_searchbase(cfg->get("searchbase"))*/
{

	MT_LOGINIT(TemplateProc);
}

TemplateProc::~TemplateProc()
{

}

void TemplateProc::SendCmd(const std::string& MSISDN,
		const std::string& CalledStationId)
{
	MT_LOGINFO("Send command to Template Processor: " << m_nfId << "\n\t MSISDN: " << MSISDN <<"\n\t CalledStationId" << CalledStationId);
	MT_LOGINFO("Searching for: "<< m_searchbase);
/*
	CommandPtr command = new Command(Command::GET_SETTING_WITH_SEARCH_BASE,
			m_nfId, MSISDN, CalledStationId, m_searchbase);

	m_cmdSender.sendCommand(command);
	*/
}

