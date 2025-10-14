//============================================================================
// Name        : nfparser.cpp
// Author      : Wocson
// Version     :
// Copyright   : Mobitecnologies
// Description :
//============================================================================

#include <mt/utils/System.h>
#include <mt/logger/LoggerFactory.h>

#include "nfparser.h"
#include "NflowProc.h"
#include <wl/utils/Srvfunc.h>

NfParser::NfParser(int argc, char* argv[]) :
	mt::Application(argc, argv), m_loggerOn(true) {

}

NfParser::~NfParser() {

	delete m_nfdump;
	delete m_nfstats;
	delete m_templproc;
	delete m_vradius;
	delete m_fltproc;
	delete m_nfReceiver;
}

void NfParser::initialize() {

	m_config.init(m_configFileName);

	mt::XmlConfigPtr cfg, cfgsub;

	cfg = m_config.getConfig("logger");
	mt::LoggerFactory::instance()->init(cfg);

	MT_LOGINIT(NfParser);

   //Get Options
	MT_LOGDEBUG("Get <Options> from config");
	cfg = m_config.getConfig("Options");
	MT_EXCEPTION_ASSERT(cfg, "No section <Options> at configuration");

	//Set Id nfparser
	cfgsub = cfg->getConfig("NfParser");
	m_NfParserId = cfgsub->get("id");
	MT_LOGTRACE("Start nfparser (id " << m_NfParserId <<")");

	//Init Statistics
	MT_LOGDEBUG("Init Statistics");
	m_nfstats = new NfStatOut(cfg);

	MT_LOGDEBUG("Init Nflow Proccessor");
	m_nfdump = new NflowDump(cfg);

	//Get Connections
	MT_LOGDEBUG("Get <Connection> from config");
	//mt::XmlConfigPtr cfg = m_config.getConfig("Connections");
	cfg = m_config.getConfig("Connections");
	MT_EXCEPTION_ASSERT(cfg, "No section <Connections> at configuration");

	// Init Filters
	MT_LOGDEBUG("Get <Filters> from config.");
	cfgsub = m_config.getConfig("Filters");
	MT_EXCEPTION_ASSERT(cfgsub, "No section <Filters> at configuration");
	MT_LOGDEBUG("Init FilterProcesor.");
	m_fltproc = new FilterProc(cfgsub);

	// Init recv netflows
	MT_LOGDEBUG("Get <NflowSource> from config");
	cfgsub = cfg->getConfig("NflowSource");
	MT_EXCEPTION_ASSERT(cfgsub, "No section <NflowSource> at configuration");
	MT_LOGDEBUG("Init Netflow Receiver.");
	m_nfReceiver = new NfReceiver(cfgsub);

	// Init VRadius
	MT_LOGDEBUG("Get <VRadius> from config");
	cfgsub = cfg->getConfig("VRadius");
	MT_EXCEPTION_ASSERT(cfgsub, "No section <VRadius> at configuration");
	MT_LOGDEBUG("Init VRadius Exchanger.");
	m_vradius = new VRadiusExch(cfgsub);

	//Init Template Processor
	MT_LOGDEBUG("Get <TemplateProcessor> from config");
	cfgsub = cfg->getConfig("TemplateProcessor");
	MT_EXCEPTION_ASSERT(cfgsub, "No section <TemplateProcessor> at configuration");
	MT_LOGDEBUG("Init Template Processor");
	m_templproc = new TemplateProc(m_NfParserId,cfgsub);
}

int32_t NfParser::readArguments(int argc, char* argv[]) {
	int32_t argval = -1;
	m_loggerOn = true;
	m_configFileName = "none";
	std::string fullfname;

	if (argc > 1) {
		while ((argval = getopt(argc, argv, "c:s")) != -1) {
			switch (argval) {
			case 'c':
				if (0 == optarg)
					return argval;
				fullfname = std::string(optarg);
				if ((access(fullfname.c_str(), 0) != -1))
					m_configFileName = fullfname;
				else
					return argval;
				break;
			case 's':
				m_loggerOn = false;
				break;
			default:
				return 0;
			}
		}
	} else {
		//std::string path[] = {"./etc/"};
		std::string path[] = { "./etc/", "../etc/" }; //for Debug.
		std::string fullfname;
		uint size = ARRAY_COUNT(path);

		for (uint i = 0; i < size; i++) {
			fullfname = path[i] + "nfparser.config.xml";
			if ((access(fullfname.c_str(), 0) != -1)) {
				m_configFileName = fullfname;
				break;
			} else
				return -1;
		}
	}
	return argval;
}

void NfParser::usage(uint32_t opt) {
	if (opt)
		std::cerr
				<< "\n To start nfnparser use:\n nfparser [-c <config file>] [-s]\n"
					"\t -c \tset configuration file name. Default is './etc/nfparser.config.xml'. \n"
					"\t -s \tset silence mode. No loggs to stdout . Default is off.\n"
				<< std::endl;
}

void NfParser::process() {

	try {
		/*		if (m_config.needReinitConfiguration()) {
		 MT_LOGINFO("Reload configuration from " << m_configFileName);
		 }
		 */

		mt::ByteBuffer nflowbuff = m_nfReceiver->recvNflow();
		MT_LOGINFO("Recv packet.");

		if (nflowbuff)
		{
			m_nfstats->regPackets(nflowbuff.getSize()); //Statistics

			NflowProc nfproc;

			NetFlowPack_v5 nflowpack = nfproc.nfParser(nflowbuff);

			m_nfdump->DropData(nflowpack);

			m_nfstats->regFlows(nfproc.getCountFlows()); //Statistics

			std::vector<Nfv5_Flow>::const_iterator itflow;

			for (itflow = nflowpack.flows.begin(); itflow	!= nflowpack.flows.end(); itflow++)
			{
			if (m_fltproc->check(*itflow))
				{
					std::string ipaddr = Srvfunc().getIpAddrDotOct(itflow->srcaddr);
					MT_LOGINFO("Send request to VRadius by srcIP: "<< ipaddr);
					m_nfstats->regVradiusReq(); //Statistics

					if (m_vradius->sendVRadiusRequiest(ipaddr.c_str()))
					{
						m_nfstats->regVradiusSussedResp(); //Statistics
						m_nfstats->regTemplCmdSend(); //Statistics
						m_templproc->SendCmd(m_vradius->getMSISDN(),m_vradius->getCalledStationId());
					} else
						m_nfstats->regVradiusErrorResp(); //Statistics
				}
				m_nfstats->outStats(); //Statistics
			}
		}

		nflowbuff.clear();

	} catch (const mt::Exception& me) {
		MT_LOGERROR("Exception in process: " << me.displayText());
	}
}
