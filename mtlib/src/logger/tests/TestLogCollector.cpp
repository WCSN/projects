#include "TestLogCollector.h"
#include "mt/logger/Logger.h"
#include "mt/logger/LogCollector.h"
#include "mt/logger/Logable.h"

#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION(TestLogCollector);
const std::string TEST_MESSAGE = "Test log message";

using namespace mt;

void TestLogCollector::setUp() {

	std::string testInitByXmlCfgStr = "<logfile path='test_pathB' level='DEBUG'>"
				"<class name='A'/>"
				"<class name='B'/>"
			"</logfile>";

	m_testInitByXmlCfg = new XmlConfig(testInitByXmlCfgStr.c_str());

}

void TestLogCollector::tearDown(){
}

void TestLogCollector::testInit() {

	LogCollectorPtr ptr = new LogCollector();
	int32_t ret = ptr->init(m_testInitByXmlCfg);

	CPPUNIT_ASSERT(ret == 0);

	std::vector<std::string> v = ptr->getClassNames();

	CPPUNIT_ASSERT_EQUAL(std::string("A"), v[0]);
	CPPUNIT_ASSERT_EQUAL(std::string("B"), v[1]);


}

