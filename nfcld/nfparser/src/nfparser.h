/*
 * nfparser.h
 *
 *  Created on: 07.02.2011
 *      Author: wocson
 */

#ifndef NFPARSER_H_
#define NFPARSER_H_

#include <mt/tools/ReloadableConfig.h>
#include <mt/tools/Application.h>
#include <mt/logger/Logable.h>

#include "NfReceiver.h"
#include "VRadiusExch.h"
#include "Filter.h"
#include "FilterProc.h"
#include "TemplateProc.h"
#include "NfStatOut.h"
#include "NflowDump.h"

#define ARRAY_COUNT(x) (sizeof(x)/sizeof(x[0]))

class NfParser : public mt::Application,public mt::Logable{

public:
	NfParser(int argc, char* argv[]);
	virtual ~NfParser();

protected:
	int32_t readArguments(int argc, char* argv[]);
	void usage(uint32_t opt=0);
	void initialize();
	void process();
	std::string m_NfParserId;

private:
	mt::ReloadableConfig m_config;
	std::string m_configFileName;
	bool m_loggerOn;

	NfReceiver *m_nfReceiver;
	FilterProc *m_fltproc;
	VRadiusExch *m_vradius;
	TemplateProc *m_templproc;
	NfStatOut *m_nfstats;
	NflowDump *m_nfdump;
};


#endif /* NFPARSER_H_ */
