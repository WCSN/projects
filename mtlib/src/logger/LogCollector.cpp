/*
 * LogCollector.cpp
 *
 *  Created on: 12.08.2010
 *      Author: hakamasy
 */

#include "mt/logger/LogCollector.h"
#include "mt/network/UdpSocket.h"
#include <ios>
#include <pthread.h>
#include <signal.h>

namespace mt {

LogCollector::LogCollector() {
	m_out = 0;
	m_default = false;
	sock = NULL;

}

LogCollector::~LogCollector(){

	stop();
}


void LogCollector::stop(){

	m_queue.push(" ");
	InfiniteThread::stop();
}


int32_t LogCollector::initLocal(const std::string &path) {

	m_filePath = path;
	return 0;
}
int32_t LogCollector::initNetwork() {

	return 0;
}

void LogCollector::runLoop() {

	std::string outStr = m_queue.pop();

	if(outStr.length() == 1 && outStr[0] == ' '){
		usleep(100);
		return;
	}

	if (m_out & STDOUT)
		std::cout << outStr << std::endl;

	if (m_out & FILE) {
		m_fileStream << outStr << std::endl;
	}

	if (m_out & NETWORK)
		std::cout << "send to m.logger: " << outStr;

}

int32_t LogCollector::init(XmlConfigPtr cfg) {


	std::string onStdOut_str = cfg->get("stdout");
	if ((onStdOut_str.find("true") != std::string::npos)) {
		m_out |= LogCollector::STDOUT;
	}

	//get path
	m_filePath = cfg->get("path");
	if (m_filePath.length()) {
		m_fileStream.open(m_filePath.c_str(), std::ios::out | std::ios::app);
		if (m_fileStream.fail()) {
			std::cerr << "can't open file " << m_filePath;
			return -1;
		}
		m_out |= LogCollector::FILE;
	}

	//get level
	std::string logLevel = cfg->get("level");
	if (!logLevel.compare("INFO")) {
		m_level = INFO;
	} else if (!logLevel.compare("WARNING")) {
		m_level = WARNING;
	} else if (!logLevel.compare("ERROR")) {
		m_level = ERROR;
	} else if (!logLevel.compare("CRITICAL")) {
		m_level = CRITICAL;
	} else if (!logLevel.compare("DEBUG")) {
		m_level = DEBUG;
	} else {
		m_level = TRACE;
	}

	std::string def = cfg->get("default");
	if (!def.compare("true")) {
		m_default = true;
	}

	ConfigList classNames = cfg->getConfigs(std::string("class"));
	for (size_t m = 0; m < classNames.size(); m++) {
		std::string className = classNames[m]->get("name");
		m_classNames.push_back(className);
	}

	return 0;
}

}

