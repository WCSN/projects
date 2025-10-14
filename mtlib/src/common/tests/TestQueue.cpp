#include "TestQueue.h"
#include <mt/common/Exception.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TestQueue);

using namespace mt;

void TestQueue::setUp() {

}

void TestQueue::tearDown() {
}


void TestQueue::testPushPop() {

	m_testingQueue.push(1);
	uint32_t i = m_testingQueue.pop();
	CPPUNIT_ASSERT_EQUAL((uint32_t)1, i);
}





