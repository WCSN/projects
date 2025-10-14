#ifndef __CONFIG__TESTS__TEST_SINGLETON__H
#define __CONFIG__TESTS__TEST_SINGLETON__H

#include <cppunit/extensions/HelperMacros.h>

class TestSingleton: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestSingleton);
	CPPUNIT_TEST(testCreate);
	CPPUNIT_TEST(testReleaseSingleton);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testCreate();
	void testReleaseSingleton();
	
public:
	void setUp();
	void tearDown();
	
private:
};

#endif //__CONFIG__TESTS__TEST_SINGLETON__H
