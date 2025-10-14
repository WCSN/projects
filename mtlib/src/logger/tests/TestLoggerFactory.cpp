#include "TestLoggerFactory.h"
#include "mt/logger/Logger.h"
#include "mt/logger/LoggerFactory.h"
#include "mt/logger/Logable.h"
#include "mt/logger/LogCollector.h"

#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION( TestLoggerFactory);
const std::string TEST_MESSAGE = "Test log message";

using namespace mt;

class A: public Logable {

public:
	A() {
		MT_LOGINIT(A)
	}

	LoggerPtr getLogger() {
		return m_logger;
	}

};

class B: public Logable {

public:
	B() {
		MT_LOGINIT(B)
	}

	LoggerPtr getLogger() {
		return m_logger;
	}

};


class TestingLoggerFactory : public LoggerFactoryInterface{

public :




};



void TestLoggerFactory::setUp() {

	std::string testInitByXmlCfgStr = "<config>"
			"<logfile path='test_pathB' level='DEBUG' stdout='true' default='true'>"
				"<class name='A'/>"
				"<class name='B'/>"
			"</logfile>"

			"<logfile path='test_pathA' level='INFO' stdout='true'>"
				"<class name='C'/>"
				"<class name='D'/>"
			"</logfile>"

		"</config>";

	m_testInitByXmlCfg = new XmlConfig(testInitByXmlCfgStr.c_str());

}

void TestLoggerFactory::tearDown() {

	//LoggerFactory::release();

}

void TestLoggerFactory::testUniqueLoggerGarantee() {

	B b;
	A a;

	CPPUNIT_ASSERT(a.getLogger() != b.getLogger());
	CPPUNIT_ASSERT_EQUAL(std::string("A"), a.getLogger()->getClassName());
	CPPUNIT_ASSERT_EQUAL(std::string("B"), b.getLogger()->getClassName());

}

void TestLoggerFactory::testInitByXml() {

	int32_t ret = LoggerFactory::instance()->init(m_testInitByXmlCfg);
	CPPUNIT_ASSERT_MESSAGE("Init return error", ret == 0);

	LogCollectorPtr ptr1 = LoggerFactory::instance()->getCollector("A");
	LogCollectorPtr ptr2 = LoggerFactory::instance()->getCollector("B");

	CPPUNIT_ASSERT(ptr1 == ptr2);

	ptr1 = LoggerFactory::instance()->getCollector("C");
	ptr2 = LoggerFactory::instance()->getCollector("D");

	CPPUNIT_ASSERT(ptr1 == ptr2);

	ptr1 = LoggerFactory::instance()->getCollector("A");
	ptr2 = LoggerFactory::instance()->getCollector("C");

	CPPUNIT_ASSERT(ptr1 != ptr2);

}



void TestLoggerFactory::testParseXmlConfig() {

	/*	std::vector<LogCollectorDescPtr> v = LoggerFactory::getInstance()->parseXmlConfig(m_testInitByXmlCfg);*/

	/*
	 CPPUNIT_ASSERT(v.size() == (size_t)2);
	 CPPUNIT_ASSERT( ! v[0]->getMasks()[0].compare("A") );
	 */

}




void TestLoggerFactory::testReinit() {

	TestingLoggerFactory *logFactory = new TestingLoggerFactory();
	logFactory->init(m_testInitByXmlCfg);
	LogCollectorPtr clctr = logFactory->getCollector(std::string("A"));
	logFactory->init(m_testInitByXmlCfg);


	CPPUNIT_ASSERT((int)clctr.count() == (int)1);


}
