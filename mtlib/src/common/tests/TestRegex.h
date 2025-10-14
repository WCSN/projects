#ifndef __COMMON__TESTS__TEST_REGEX__H
#define __COMMON__TESTS__TEST_REGEX__H

#include <cppunit/extensions/HelperMacros.h>
#include <mt/common/Regex.h>

class TestRegex: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestRegex);
	CPPUNIT_TEST(testMatchIfValueMatched);
	CPPUNIT_TEST(testInitRegexIfMaskIsIncorrect);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testMatchIfValueMatched();
	void testMatchIfValueNotMatched();
	void testInitRegexIfMaskIsIncorrect();
	
public:
	void setUp();
	void tearDown();

private:	
	mt::RegexPtr m_testingRegex;
};

#endif //__COMMON__TESTS__TEST_REGEX__H
