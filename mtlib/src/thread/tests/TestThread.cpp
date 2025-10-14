#include "TestThread.h"
#include <mt/thread/Thread.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TestThread);

const std::string THREAD_NAME = "Testing thread";

class TestingThread : public mt::Thread {
public:
	TestingThread() : m_threadWasRunned(false) {
	}

	virtual void run() {
		mt::Thread::setName(THREAD_NAME);
		m_threadName = mt::Thread::name();
		m_threadWasRunned = true;
	}
	
	bool m_threadWasRunned;
	std::string m_threadName;
};

void TestThread::setUp() {
}

void TestThread::tearDown() {
}

void TestThread::testStartThread() {
	TestingThread thread;
	thread.start();
	thread.wait();
	CPPUNIT_ASSERT(thread.m_threadWasRunned);
	CPPUNIT_ASSERT(thread.m_threadName.find(THREAD_NAME) != std::string::npos);
}

void TestThread::testGetName() {
	CPPUNIT_ASSERT_EQUAL(std::string("main_thread"), mt::Thread::name());
}


