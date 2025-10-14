#ifndef __COMMON__TESTS__TEST_REGEX__H
#define __COMMON__TESTS__TEST_REGEX__H

#include <cppunit/extensions/HelperMacros.h>
#include <mt/common/Queue.h>



class TestQueue: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestQueue);
	CPPUNIT_TEST(testPushPop);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testPushPop();

public:
	void setUp();
	void tearDown();

private:
	mt::Queue<uint32_t> m_testingQueue;
};

#endif //__COMMON__TESTS__TEST_REGEX__H
