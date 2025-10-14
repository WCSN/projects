/*
 * LogProcessor.cpp
 *
 *  Created on: 18.07.2010
 *      Author: hakamasy
 */

#include "mt/logger/LoggerFactory.h"
#include "mt/logger/Logger.h"
#include "mt/common/XmlConfig.h"
#include "mt/logger/LogCollector.h"

using namespace mt;

/*LoggerFactoryInterface* LoggerFactoryInterface::m_instance = NULL;*/

void LoggerFactoryInterface::reinitLoggers() {

	AutoMutex a(m_allLoggersMtx);

	LogLevel level;
	for (size_t i = 0; i < m_allLoggers.size(); i++) {

		LoggerPtr logger = m_allLoggers[i];
		LogCollectorPtr clctr = getCollector(logger->getClassName());
		if (clctr) {
			level = clctr->getLogLevel();
		}else{
			level = DEBUG;
		}

		logger->setCollector(clctr);
		logger->setLogLevel(level);

	}

}

void LoggerFactoryInterface::setFactoryMap(std::map<std::string,
		LogCollectorPtr>* m) {
	AutoMutex a(m_factoryMapMutex);
	if (m_factoryMap != NULL)
		delete m_factoryMap;
	m_factoryMap = m;
}

bool LoggerFactoryInterface::hasFactoryMap(){
	AutoMutex a(m_factoryMapMutex);
	if (m_factoryMap != NULL)
		return true;
	return false;
}


LoggerFactoryInterface::LoggerFactoryInterface() :
	m_connected(false), m_writeFileLine(false), m_level(DEBUG) {
	m_factoryMap = NULL;
}

int LoggerFactoryInterface::init(const std::string &addr,
		const unsigned short port, const std::string &strLevel,
		const bool isStdOut) {

	m_sockFd = socket(AF_INET, SOCK_DGRAM, 0);
	m_procAddr.sin_family = PF_INET;
	m_procAddr.sin_port = htons(port);
	m_procAddr.sin_addr.s_addr = inet_addr(addr.c_str());
	m_stdout = isStdOut;

	if (!strLevel.compare("DEBUG")) {
		m_level = DEBUG;
	} else if (!strLevel.compare("TRACE")) {
		m_level = TRACE;
	} else if (!strLevel.compare("INFO")) {
		m_level = INFO;
	} else if (!strLevel.compare("WARNING")) {
		m_level = WARNING;
	} else if (!strLevel.compare("ERROR")) {
		m_level = ERROR;
	} else {
		std::cout << "can't determinate log level " << strLevel
				<< ". Use default 'DEBUG'";
		m_level = DEBUG;
	}

	if (isStdOut) {
		m_defaultCollector = new LogCollector();
		m_defaultCollector->setStdOut();
		m_defaultCollector->start();
	}
	return 0;
}

LogCollectorPtr LoggerFactoryInterface::getCollector(
		const std::string &className) {

	if (!m_factoryMap) {
		return m_defaultCollector;
	}

	LogCollectorPtr collector;
	AutoMutex a(m_factoryMapMutex);
	std::map<std::string, LogCollectorPtr>::iterator it = m_factoryMap->find(
			className);
	if (it != m_factoryMap->end()) {
		collector = it->second;
	} else {
		collector = m_defaultCollector;
	}

	return collector;
}

int LoggerFactoryInterface::init(const XmlConfigPtr cfg) {

	ConfigList cfgList = cfg->getConfigs(std::string("logfile"));
	std::vector<LogCollectorPtr> collectors;

	for (size_t i = 0; i < cfgList.size(); i++) {
		LogCollectorPtr collector = new LogCollector();
		if (!collector->init(cfgList[i])) {
			collectors.push_back(collector);
			collector->start();
		} else {
			return -1;
		}
	}

	std::map<std::string, LogCollectorPtr>* newMap = createFactoryMap(
			collectors);

	bool needReinitLogger = hasFactoryMap();
	setFactoryMap(newMap);

	if(needReinitLogger){
		reinitLoggers();
	}

	return 0;
}

std::map<std::string, LogCollectorPtr> *LoggerFactoryInterface::createFactoryMap(
		const std::vector<LogCollectorPtr> &v) {

	std::map<std::string, LogCollectorPtr> *map = new std::map<std::string,
			LogCollectorPtr>;

	for (size_t i = 0; i < v.size(); i++) {
		LogCollectorPtr clctr = v[i];
		std::vector<std::string> &classes = clctr->getClassNames();
		for (size_t j = 0; j < classes.size(); j++) {
			(*map)[classes[j]] = clctr;
		}

		if (clctr->isDefault()) {
			m_defaultCollector = clctr;
		}
	}

	return map;
}

LoggerPtr LoggerFactoryInterface::createLogger(const std::string &className) {

	LogLevel level = INFO;

	LogCollectorPtr clctr = getCollector(className);
	if (clctr) {
		level = clctr->getLogLevel();
	}
	LoggerPtr logger = new Logger(className, level);
	logger->setCollector(clctr);

	AutoMutex a(m_allLoggersMtx);
	m_allLoggers.push_back(logger);

	return logger;
}

