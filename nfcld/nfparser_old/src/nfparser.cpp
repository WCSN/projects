#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
#include <syslog.h>
#include <signal.h>
#include <iostream>
#include <iomanip>
#include <exception>
#include <fstream>

// Mobitech inc.

#include <mt/utils/System.h>
#include <mt/logger/LoggerFactory.h>
#include <mt/common/XmlConfig.h>
#include <mt/common/Exception.h>
#include <mt/logger/Logable.h>
#include <mt/tools/ReloadableConfig.h>
#include <mt/tools/Application.h>

#include "helper.h"
#include "nfparser.h"
#include "netflowprc.h"
#include "conf.h"

NfparserApp *nfparser_app;
Conf *conf;
NetflowPr *nfpcsr;

mt::XmlConfigPtr logger;
std::string initStr;

void fsignal(int sig)
{
	syslog(LOG_NOTICE,"Recv signal %s(%d)",signalsmsg[sig-1],sig);

	switch(sig)
	{
		case SIGTERM:
		case SIGINT:
		case SIGQUIT:
			syslog(LOG_NOTICE,"Close daemon. End working.");
			closelog();
			delete nfparser_app;
			exit(0);
			break;
		case SIGHUP:
			syslog(LOG_NOTICE,"Reread config");
			// Nfparser reread config // only daemoniZed // in debug mode close application
			conf->ReReadConf();
			syslog(LOG_NOTICE,"Timeout %lu",conf->m_timeoutstat);
			break;
		default:
			syslog(LOG_NOTICE,"Ignore...");
			break;
	}
}

NfparserApp::NfparserApp(void)
{
	if(isopt('D'))// default -D //daemonize
			initStr = "<logger><logfile path='/home/wocson/log/nfparserd.log' level='TRACE' default='true' stdout='true'><class name='*'/></logfile></logger>";
	else
	{
		if(isopt('d'))
			initStr = "<logger><logfile path='/home/wocson/log/nfparser_debug.log' level='DEBUG' default='true' stdout='true'><class name='*'/></logfile></logger>";
		else
			initStr = "<logger><logfile path='/home/wocson/log/nfparser_trace.log' level='TRACE' default='true' stdout='true'><class name='*'/></logfile></logger>";
	}

	logger = new mt::XmlConfig(initStr.c_str());
	mt::LoggerFactory::instance()->init(logger);

	MT_LOGINIT(NfparserApp);
}

int NfparserApp::DataProcess(void)
{
	return ws->Listener();
}

void NfparserApp::Daemonized(void)
{
	std::cout << "nfparser daemon started";
	//chdir("/");
	setsid();

	for (int j = 1; j < 32; j++)
		signal(j, fsignal);

	char msg[1024];
	memset(msg, 0, sizeof(msg));

	int nkeys = getnKEYS();
	for (int i = 0; i < nkeys; i++)
		strcat(msg, getkeys(i));

	syslog(LOG_INFO, "daemon started %s pid: %d", msg, getpid());
	std::cout << " " << getpid() << "\n";

	conf->m_pidfilestream.open(conf->m_pidFilename.c_str(),	std::fstream::out);

	if (!conf->m_pidfilestream.is_open())
		syslog(LOG_ERR, "pid file create failed... %s", strerror(errno));
	else
	{
		conf->m_pidfilestream << getpid();
		conf->m_pidfilestream.close();
	}

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
}

int NfparserApp::Run(void)
{
	if(isopt('D'))// default -D //daemonize
	{
		int retd = fork();

		if(retd < 0)
			return EXIT_FAILURE;
		else if (retd > 0 ) // Parents
			return EXIT_SUCCESS; // Close parents
		else
			Daemonized();
	}

	MT_LOGINFO("nfparser started (pid:" << getpid() << ")" );


	if(isopt('c'))
   		conf = new Conf(conffilename); // conffilename external
   	else
   		conf = new Conf();

	nfpcsr = new NetflowPr();
	ws = new WC_Socket();

return DataProcess();
}

void NfparserApp::Close(void)
{
	remove(conf->m_pidFilename.c_str());
	delete nfpcsr;
	delete ws;
	delete conf;
}

NfparserApp::~NfparserApp()
{
	Close();
}

int main(int argc, char **argv, char **env)
{
	try
	{
		if (argc < 2)
		{
			char *defarg[] ={ "/", "-D" };
			ParceCmd(2, defarg,env);
		}
		else if (ParceCmd(argc, argv, env) == ERROR_CMD)
			return ERROR_CMD; // int -101

		if (isopt('v'))
			version(SMLVER);
		else if (isopt('V'))
			version(LRGVER);
		else if (isopt('h') || isopt('H'))
			usage();
		else
		{
			nfparser_app = new NfparserApp();
			nfparser_app->Run();
		}
	}
	catch(const mt::Exception& my)
	{
		std::cout << my.displayText();
	}

return 0;
}
