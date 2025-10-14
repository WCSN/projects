#ifndef MLOGGER
#define MLOGGER

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

#include "mt/common/Exception.h"
#include "mt/common/SmartPtr.h"
#include "mt/logger/LoggerDataTypes.h"
#include <mt/logger/LoggerFactory.h>
#include "mt/thread/Mutex.h"

namespace mt {

#define MAXLOGSTRLEN 20000
#define LOG_LEVEL_LENGTH 7

class Logger {

	Mutex m_clctrMtx;
	LogCollectorPtr m_collector;

	Mutex m_logLevelMtx;
	LogLevel m_logLevel;

	bool m_connected;
	bool m_writeFileLine;

	std::string m_className;

	bool writable(LogLevel l) const {
		if (l >= m_logLevel) {
			return true;
		}
		return false;
	}

private:
	std::string formatTime(const struct timeval& time);
	std::string pid(int32_t pid, int32_t tid);

protected:

	static char levelVals[][10];

	virtual void common(LogLevel l, char* format, va_list args);

public:

	void setCollector(LogCollectorPtr);

	void setLogLevel(LogLevel l) {
		AutoMutex a(m_logLevelMtx);
		m_logLevel = l;
	}

	LogLevel getLogLevel() {
		AutoMutex a(m_logLevelMtx);
		return m_logLevel;
	}

	Logger(const std::string &str, LogLevel l = DEBUG);

	const std::string& getClassName() const {
		return m_className;
	}

	void Connected(bool b);

	virtual ~Logger();

	void info(char* format, ...) {

		if (!writable(INFO))
			return;
		va_list args;
		va_start(args, format);
		common(INFO, format, args);
		va_end(args);

	}
	;

	void warn(char* format, ...) {

		if (!writable(WARNING))
			return;
		va_list args;
		va_start(args, format);
		common(WARNING, format, args);
		va_end(args);

	}
	;

	void debug(char* format, ...) {

		if (!writable(DEBUG))
			return;
		va_list args;
		va_start(args, format);
		common(DEBUG, format, args);
		va_end(args);

	}
	;

	void trace(char* format, ...) {

		if (!writable(TRACE))
			return;
		va_list args;
		va_start(args, format);
		common(TRACE, format, args);
		va_end(args);

	}

	void error(char* format, ...) {

		if (!writable(ERROR))
			return;
		va_list args;
		va_start(args, format);
		common(ERROR, format, args);
		va_end(args);

	}

};

}

/*
 #define MT_LOGINIT(classname)																					\
	MT_LOGINIT(classname, "");

 */

#define MT_LOGINIT_STATIC(classname)																						\
	LoggerPtr m_logger = mt::LoggerFactory::instance()->getLogger<classname>(#classname); \
	std::string m_loggerPrefix;

#define MT_LOG_SETPREFIX_STATIC(prefix)\
	std::stringstream str;\
	str << prefix;\
	m_loggerPrefix = str.str();

#define MT_LOGINIT(classname)																						\
	m_logger = mt::LoggerFactory::instance()->getLogger<classname>(#classname); \

#define MT_LOG_SETPREFIX(prefix)\
	std::stringstream str; \
	str << prefix;	\
	m_loggerPrefix = str.str();

#define MT_LOGDEBUG(message)																					\
	{																																		\
		if(!m_logger)																				\
			MT_EXCEPTION(mt::LoggerException, "No logger implementation");	\
		std::stringstream str;\
		if(m_loggerPrefix.length()) 	\
		str<<m_loggerPrefix<<": ";		\
		str << message;																	\
		if(mt::LoggerFactory::instance()->writeFileLine())							\
			str << " | " << __FILE__ << ":" << __LINE__;										\
		m_logger->debug(const_cast<char*>("%s"), str.str().c_str());			\
	}

#define MT_LOGWARNING(message)																				\
	{																																		\
		if(!m_logger)																											\
			MT_EXCEPTION(mt::LoggerException, "No logger implementation");	\
		std::stringstream str;																						\
		if(m_loggerPrefix.length()) 	\
		str<<m_loggerPrefix<<": ";		\
		str << message;																										\
		if(mt::LoggerFactory::instance()->writeFileLine())							\
			str << " | " << __FILE__ << ":" << __LINE__;										\
		m_logger->warn(const_cast<char*>("%s"), str.str().c_str());				\
	}

#define MT_LOGTRACE(message)																					\
	{																																		\
		if(!m_logger)																											\
			MT_EXCEPTION(mt::LoggerException, "No logger implementation");	\
		std::stringstream str;																						\
		if(m_loggerPrefix.length()) 	\
		str<<m_loggerPrefix<<": ";		\
	str << message;																										\
		if(mt::LoggerFactory::instance()->writeFileLine())							\
			str << " | " << __FILE__ << ":" << __LINE__;										\
		m_logger->trace(const_cast<char*>("%s"), str.str().c_str());			\
	}

#define MT_LOGINFO(message)																						\
	{																																		\
		if(!m_logger)																											\
			MT_EXCEPTION(mt::LoggerException, "No logger implementation");	\
		std::stringstream str;																						\
		if(m_loggerPrefix.length()) 	\
		str<<m_loggerPrefix<<": ";		\
		str << message;																										\
		if(mt::LoggerFactory::instance()->writeFileLine())							\
			str << " | " << __FILE__ << ":" << __LINE__;										\
		m_logger->info(const_cast<char*>("%s"), str.str().c_str());				\
	}

#define MT_LOGERROR(message)																					\
	{																																		\
		if(!m_logger)																											\
			MT_EXCEPTION(mt::LoggerException, "No logger implementation");	\
		std::stringstream str;																						\
		if(m_loggerPrefix.length()) 	\
		str<<m_loggerPrefix<<": ";		\
		str << message;\
		if(mt::LoggerFactory::instance()->writeFileLine()) \
			str << " | " << __FILE__ << ":" << __LINE__;\
		m_logger->error(const_cast<char*>("%s"), str.str().c_str()); \
}

#endif
