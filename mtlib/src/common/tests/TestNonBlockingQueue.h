#ifndef TESTNONBLOCKINGQUEUE__H
#define TESTNONBLOCKINGQUEUE__H

#include <cppunit/extensions/HelperMacros.h>

class TestNonBlockingQueue: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestNonBlockingQueue);
	CPPUNIT_TEST(testPushPop);
	CPPUNIT_TEST(testPopEmptyQueue);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testPushPop();
	void testPopEmptyQueue();
	
};

#endif //TESTNONBLOCKINGQUEUE__H
