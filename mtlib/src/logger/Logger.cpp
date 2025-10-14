#include <mt/logger/Logger.h>
#include "mt/logger/LoggerFactory.h"
#include <mt/utils/System.h>
#include <mt/utils/StrUtils.h>
#include "mt/logger/LogCollector.h"
#include "mt/thread/AutoMutex.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#define DIGIT_FORMAT(digit) std::setw(digit) << std::setfill('0')
#define STR_FORMAT(digit) std::left << std::setw(digit)

namespace mt {


char Logger::levelVals[][10] = { "TRACE  ", "DEBUG  ", "INFO   ", "WARNING",
		"ERROR  ", "REWRITE" };


void Logger::setCollector(LogCollectorPtr p) {

	AutoMutex a(m_clctrMtx);
	m_collector = p;
}

Logger::Logger(const std::string &str, LogLevel l) {
	m_className = str;
	m_logLevel = l;
	m_writeFileLine = false;
	m_connected = false;

}

void Logger::Connected(bool b) {
	m_connected = b;
}

Logger::~Logger() {
}

void Logger::common(LogLevel l, const char* format, va_list args) {

	struct timeval time;
	struct timezone tz;

	gettimeofday(&time, &tz);

	char buf[2048];
	vsnprintf(buf, 2048, format, args);
	std::string body = m_className + ": ";
	body.append(buf);

	std::string log_string = formatTime(time) + " " + pid(mt::System::getPid(),
			mt::System::getTid()) + " [" + levelVals[l] + "] " + body;

	AutoMutex a(m_clctrMtx);
	if (m_collector) {
		m_collector->push(log_string);
	}

}

std::string Logger::formatTime(const struct timeval& time) {

	struct tm *local_tm = localtime(&time.tv_sec);
	std::stringstream date_time;

	date_time << DIGIT_FORMAT(2) << local_tm->tm_year - 100 << "-"
			<< DIGIT_FORMAT(2) << local_tm->tm_mon + 1 << "-"
			<< DIGIT_FORMAT(2) << local_tm->tm_mday << " " << DIGIT_FORMAT(2)
			<< local_tm->tm_hour << ":" << DIGIT_FORMAT(2) << local_tm->tm_min
			<< ":" << DIGIT_FORMAT(2) << local_tm->tm_sec << "."
			<< DIGIT_FORMAT(3) << (uint32_t) time.tv_usec / 1000;
	return date_time.str();
}

std::string Logger::pid(int32_t pid, int32_t tid) {
	std::stringstream stream;
	stream << STR_FORMAT(5) << pid << ":" << STR_FORMAT(5) << tid;
	return stream.str();
}

}
