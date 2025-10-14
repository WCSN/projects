/*
 * LogCollector.h
 *
 *  Created on: 12.08.2010
 *      Author: hakamasy
 */

#ifndef LOGCOLLECTOR_H_
#define LOGCOLLECTOR_H_

#include "mt/common/Queue.h"
#include "mt/thread/InfiniteThread.h"
#include "mt/common/SmartPtr.h"
#include "mt/logger/LoggerDataTypes.h"
#include "mt/common/XmlConfig.h"
#include <fstream>
namespace mt {

class UdpSocket;

class LogCollector: public InfiniteThread {

	std::ofstream m_fileStream;
	std::string m_filePath;
	Queue<std::string> m_queue;
	UdpSocket* sock;
	uint8_t m_out;
	bool m_default;

	LogLevel m_level;
	std::vector<std::string> m_classNames;

	enum {
		NETWORK = 0x01, FILE = 0x02, STDOUT = 0x04
	};

public:


	virtual void stop();

	virtual ~LogCollector();
	void setStdOut(){
		m_out |= STDOUT;
	};

	int32_t init(XmlConfigPtr);

	LogLevel getLogLevel() {
		return m_level;
	}

	std::vector<std::string> &getClassNames() {
		return m_classNames;
	}

	bool isDefault() {
		return m_default;
	}

	void push(const std::string &str) {
		m_queue.push(str);
	}

	LogCollector();

	int32_t initLocal(const std::string &);
	int32_t initNetwork();

	void runLoop();

};

}

#endif /* LOGCOLLECTOR_H_ */
