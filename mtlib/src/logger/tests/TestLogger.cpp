#include "TestLogger.h"
#include "mt/logger/Logger.h"
#include "mt/logger/Logable.h"


#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION(TestLogger);
const std::string TEST_MESSAGE = "Test log message";

using namespace mt;

class TestingLogger : public Logger{

public:
	TestingLogger() : Logger("TestingLogger", TRACE){};
	virtual ~TestingLogger(){}

protected:
	virtual void common(LogLevel l, char* format, va_list args) {
		char buf[MAXLOGSTRLEN];
		vsprintf(buf, format, args);
		m_message = buf;
		m_currentLogLevel = l;
	}

public:
	std::string m_message;
	LogLevel m_currentLogLevel;
};

void TestLogger::setUp() {
	m_logger = new TestingLogger;
}

void TestLogger::tearDown(){
	delete m_logger;
}

void TestLogger::testLogDebugMacros() {
	MT_LOGDEBUG(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(TEST_MESSAGE, m_logger->m_message);
	CPPUNIT_ASSERT_EQUAL(DEBUG, m_logger->m_currentLogLevel);
}

void TestLogger::testLogWarningMacros() {
	MT_LOGWARNING(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(TEST_MESSAGE, m_logger->m_message);
	CPPUNIT_ASSERT_EQUAL(WARNING, m_logger->m_currentLogLevel);
}

void TestLogger::testLogTraceMacros() {
	MT_LOGTRACE(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(TEST_MESSAGE, m_logger->m_message);
	CPPUNIT_ASSERT_EQUAL(TRACE, m_logger->m_currentLogLevel);
}

void TestLogger::testLogInfoMacros() {
	MT_LOGINFO(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(TEST_MESSAGE, m_logger->m_message);
	CPPUNIT_ASSERT_EQUAL(INFO, m_logger->m_currentLogLevel);
}

void TestLogger::testLogErrorMacros() {
	MT_LOGERROR(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(TEST_MESSAGE, m_logger->m_message);
	CPPUNIT_ASSERT_EQUAL(ERROR, m_logger->m_currentLogLevel);
}



