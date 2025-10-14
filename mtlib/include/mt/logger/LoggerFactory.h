/*
 * LogProcessor.h
 *
 *  Created on: 18.07.2010
 *      Author: hakamasy
 */

#ifndef LOGGERFACTORY_H_
#define LOGGERFACTORY_H_

#include <string>
#include <sys/time.h>
#include <string>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sstream>
#include <map>
#include <vector>
#include <string>

#include "mt/logger/LoggerDataTypes.h"
#include "mt/common/SmartPtr.h"
#include "mt/thread/Mutex.h"
#include "mt/thread/AutoMutex.h"
#include "mt/common/Singleton.h"

namespace mt {

class XmlConfig;

class LoggerFactoryInterface {

protected :

	int m_sockFd;
	sockaddr_in m_procAddr;

	std::vector<LoggerPtr> m_allLoggers;
	Mutex m_allLoggersMtx;


	bool m_stdout;
	bool m_connected;
	bool m_writeFileLine;
	LogCollectorPtr m_defaultCollector;
	LogLevel m_level;

	std::map<std::string, LogCollectorPtr>* m_factoryMap;
	Mutex m_factoryMapMutex;

	LoggerFactoryInterface();

	std::map<std::string, LogCollectorPtr> *createFactoryMap(const std::vector<
			LogCollectorPtr>&);

	void setFactoryMap(std::map<std::string, LogCollectorPtr>* m);
	bool hasFactoryMap();

	void reinitLoggers();
public:

	LogCollectorPtr getCollector(const std::string &);

	friend class mt::Singleton<LoggerFactoryInterface>;
	SmartPtr<Logger> createLogger(const std::string &className);
/*
	static LoggerFactory* instance() {
		if (m_instance == NULL)
			m_instance = new LoggerFactory();
		return m_instance;
	}
	;
*/

	bool writeFileLine() {
		return m_writeFileLine;
	}


	int init(const std::string &addr, const unsigned short port,
			const std::string &level = "DEBUG", bool b = false);

	int init(const SmartPtr<XmlConfig> );

	template<class T> SmartPtr<Logger> getLogger(const std::string &className);

};

template<class T> SmartPtr<Logger> LoggerFactoryInterface::getLogger(
		const std::string &className) {

	static SmartPtr<Logger> logger;
	if (!logger) {
		logger = createLogger(className);
	}
	return logger;

}
;

typedef mt::Singleton<LoggerFactoryInterface> LoggerFactory;

}

#endif /* LOGPROCESSOR_H_ */
