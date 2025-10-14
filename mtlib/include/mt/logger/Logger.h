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

//1m - intensity
//4m - underscore
//5m - blinking
//7m - reverse
/*
#define _COLOR_

#ifdef _COLOR_

#define CLSTD		"\033[0m" 		//all attibutes off
#define BLACK  		"\033[0;30m"
#define RED  		"\033[0;31m"
#define GREEN 		"\033[0;32m"
#define YELLOW 		"\033[0;33m"
#define BLUE 		"\033[0;34m"
#define PURPLE 		"\033[0;35m"
#define CYAN 		"\033[0;36m"
#define GRAY 		"\033[0;37m"
#define LTRED  		"\033[1;31m"
#define LTGREEN		"\033[1;32m"
#define LTYELLOW 	"\033[1;33m"
#define LTBLUE 		"\033[1;34m"
#define LTPURPLE 	"\033[1;35m"
#define LTCYAN 		"\033[1;36m"
#define WHITE 		"\033[1;37m"

#else

#define CLSTD		"" 				//all attibutes off

#define BLACK  		""
#define RED  		""
#define LTRED  		""
#define GREEN 		""
#define LTGREEN		""
#define YELLOW 		""
#define LTYELLOW 	""
#define BLUE 		""
#define LTBLUE 		""
#define PURPLE 		""
#define LTPURPLE 	""
#define CYAN 		""
#define LTCYAN 		""
#define GRAY 		""
#define WHITE 		""

#endif
*/

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

	virtual void common(LogLevel l, const char* format, va_list args);

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

	void info(const char* format, ...) {

		if (!writable(INFO))
			return;
		va_list args;
		va_start(args, format);
		common(INFO, format, args);
		va_end(args);
	}

	void warn(const char* format, ...) {

		if (!writable(WARNING))
			return;
		va_list args;
		va_start(args, format);
		common(WARNING, format, args);
		va_end(args);
	}

	void debug(const char* format, ...) {

		if (!writable(DEBUG))
			return;
		va_list args;
		va_start(args, format);
		common(DEBUG, format, args);
		va_end(args);
	}

	void trace(const char* format, ...) {

		if (!writable(TRACE))
			return;
		va_list args;
		va_start(args, format);
		common(TRACE, format, args);
		va_end(args);
	}

	void error(const char* format, ...) {

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
