#ifndef __LOGGER__TESTS__TEST_LOGGER__H
#define __LOGGER__TESTS__TEST_LOGGER__H

#include <cppunit/extensions/HelperMacros.h>

class TestingLogger;

class TestLogger: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestLogger);
	CPPUNIT_TEST(testLogDebugMacros);
	CPPUNIT_TEST(testLogWarningMacros);
	CPPUNIT_TEST(testLogTraceMacros);
	CPPUNIT_TEST(testLogInfoMacros);
	CPPUNIT_TEST(testLogErrorMacros);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testLogDebugMacros();
	void testLogWarningMacros();
	void testLogTraceMacros();
	void testLogInfoMacros();
	void testLogErrorMacros();

public:
	void setUp();
	void tearDown();

private:
	TestingLogger* m_logger;
	std::string m_loggerPrefix;
};

#endif //__LOGGER__TESTS__TEST_LOGGER__H
