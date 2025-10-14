#ifndef __COMMON__TESTS__SMART_PTR__H
#define __COMMON__TESTS__SMART_PTR__H

#include <cppunit/extensions/HelperMacros.h>

class TestSmartPtr: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestSmartPtr);
	CPPUNIT_TEST(testInitSmartPtr);
	CPPUNIT_TEST(testDecrementCount);
	CPPUNIT_TEST(testCast);
	CPPUNIT_TEST(testBadCast);
	CPPUNIT_TEST(testSmartPtrInVector);
	CPPUNIT_TEST(testSmartPtrInMap);
	CPPUNIT_TEST(testCastFromDerivedToBase);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testSmartPtrInVector();
	void testSmartPtrInMap();
	void testInitSmartPtr();
	void testDecrementCount();
	void testCast();
	void testBadCast();
	void testCastFromDerivedToBase();

public:
	void setUp();
	void tearDown();
};

#endif /* __COMMON__TESTS__SMART_PTR__H */
