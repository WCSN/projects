#include "TestRegex.h"
#include <mt/common/Exception.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TestRegex);

using namespace mt;
const std::string MASK = ".*wap.*";

void TestRegex::setUp() {
	m_testingRegex = new mt::Regex(MASK);
}

void TestRegex::tearDown() {
}

void TestRegex::testMatchIfValueMatched() {
	CPPUNIT_ASSERT(m_testingRegex->match("megafon.wap.spb"));
}

void TestRegex::testMatchIfValueNotMatched() {
	CPPUNIT_ASSERT(m_testingRegex->match("megafon.internet.spb"));
}

void TestRegex::testInitRegexIfMaskIsIncorrect() {
	CPPUNIT_ASSERT_THROW(new Regex("*wap*"), mt::Exception);
}





