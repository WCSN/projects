#ifndef __COMMON__TESTS__TEST_EXCEPTION__H
#define __COMMON__TESTS__TEST_EXCEPTION__H

#include <cppunit/extensions/HelperMacros.h>

class TestException: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestException);
	CPPUNIT_TEST(testSetValueFromStream);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testSetValueFromStream();
	
public:
	void setUp();
	void tearDown();

};

#endif //__COMMON__TESTS__TEST_EXCEPTION__H
