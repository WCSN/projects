#include "TestNonBlockingQueue.h"
#include <mt/common/NonBlockingQueue.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TestNonBlockingQueue);

void TestNonBlockingQueue::testPushPop() {
	mt::NonBlockingQueue<int> queue;
	queue.push(1);
	queue.push(2);
	CPPUNIT_ASSERT_EQUAL(1, queue.pop());
	CPPUNIT_ASSERT_EQUAL(2, queue.pop());
}

void TestNonBlockingQueue::testPopEmptyQueue() {
	mt::NonBlockingQueue<int> queue;
	CPPUNIT_ASSERT_THROW(queue.pop(), mt::Exception);
}



