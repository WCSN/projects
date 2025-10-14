/*
 * TemplateProcSender.h
 *
 *  Created on: 11.02.2011
 *      Author: wocson
 */

#ifndef TEMPLATEPROC_H_
#define TEMPLATEPROC_H_

#include <mt/tools/ReloadableConfig.h>
#include <mt/logger/Logable.h>

//#include "../../command/src/Command.h"
//#include "../../command/src/CommandSender.h"


class TemplateProc: public mt::Logable {
public:
	TemplateProc(const std::string& nfId,const mt::XmlConfigPtr& cfg);
	virtual ~TemplateProc();

	void SendCmd(const std::string& MSISDN,const std::string& CalledStationId);

private:

	//CommandSender m_cmdSender;
	std::string m_nfId;
	std::string m_searchbase;
};


#endif /* TEMPLATEPROC_H_ */
