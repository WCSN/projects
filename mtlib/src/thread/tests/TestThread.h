#ifndef __THREAD__TESTS__THREAD__H
#define __THREAD__TESTS__THREAD__H

#include <cppunit/extensions/HelperMacros.h>

class TestThread: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestThread);
	CPPUNIT_TEST(testStartThread);
	CPPUNIT_TEST(testGetName);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testStartThread();	
	void testGetName();
public:
	void setUp();
	void tearDown();
};

#endif //__THREAD__THREAD__H
