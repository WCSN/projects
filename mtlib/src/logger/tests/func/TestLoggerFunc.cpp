/*
 * TestLogable.cpp
 *
 *  Created on: 18.07.2010
 *      Author: hakamasy
 */

#include "TestLoggerFunc.h"

#include <fstream>

CPPUNIT_TEST_SUITE_REGISTRATION(TestLoggerFunc);
//const std::string TEST_MESSAGE = "Test log message";




void TestLoggerFunc::setUp(){

	std::string testInitByXmlCfgStr = "<config>"
			"<logfile path='file1.log' level='DEBUG' stdout='true' default='true'>"
				"<class name='LogableClass'/>"
				"<class name='B'/>"
			"</logfile>"
			"<logfile path='file2.log' level='INFO' stdout='true'>"
				"<class name='C'/>"
				"<class name='D'/>"
			"</logfile>"
		"</config>";

	m_testInitByXmlCfg = new XmlConfig(testInitByXmlCfgStr.c_str());

}

void TestLoggerFunc::tearDown(){

	if( m_testLogable != NULL)
		delete m_testLogable ;

}


void TestLoggerFunc::testLogDebugMacros(){

	LoggerFactory::instance()->init(m_testInitByXmlCfg);

	m_testLogable = new LogableClass();
	m_testLogable->logDebug();

	std::ifstream inpt("file1.log");

	char buf[1024];
	inpt.getline(buf, sizeof(buf));

	std::string strFromLog = buf;

	CPPUNIT_ASSERT(strFromLog.find(TEST_MESSAGE) != std::string::npos);
}
