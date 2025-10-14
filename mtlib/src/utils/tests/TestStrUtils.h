#ifndef __UTILS__TESTS__TEST_STR_UTILS__H
#define __UTILS__TESTS__TEST_STR_UTILS__H

#include <cppunit/extensions/HelperMacros.h>

class TestStrUtils: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestStrUtils);
	CPPUNIT_TEST(testDetokenize);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testDetokenize();
	
public:
	void setUp();
	void tearDown();
	
};

#endif //__UTILS__TESTS__TEST_STR_UTILS__H
