#include "TestSingleton.h"
#include <mt/common/Singleton.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TestSingleton);

class SingletonTest {
	friend class mt::Singleton<SingletonTest>;

private:
	SingletonTest() {m_constructed = true;}
	virtual ~SingletonTest() {m_desstructed = true;}

public:
	int m_count;
	static bool m_constructed;
	static bool m_desstructed;

};

bool SingletonTest::m_constructed = false;
bool SingletonTest::m_desstructed = false;


typedef mt::Singleton<SingletonTest> SingletonTest_Singleton;
typedef mt::Singleton<SingletonTest> SingletonTest_SingletonSecond;

void TestSingleton::setUp() {
	SingletonTest::m_constructed = false;
	SingletonTest::m_desstructed = false;
}

void TestSingleton::tearDown() {
	SingletonTest_Singleton::release();
}

void TestSingleton::testCreate() {
	SingletonTest_Singleton::instance()->m_count = 10;
	CPPUNIT_ASSERT(SingletonTest::m_constructed);
	CPPUNIT_ASSERT_EQUAL(10, SingletonTest_Singleton::instance()->m_count);

//	CPPUNIT_ASSERT_EQUAL((uint32_t)SingletonTest_Singleton::instance(), (uint32_t)SingletonTest_SingletonSecond::instance());

}

void TestSingleton::testReleaseSingleton() {
	CPPUNIT_ASSERT(!SingletonTest::m_constructed);
	SingletonTest_Singleton::instance()->m_count = 10;
	CPPUNIT_ASSERT(SingletonTest::m_constructed);

	CPPUNIT_ASSERT(!SingletonTest::m_desstructed);
	SingletonTest_Singleton::release();
	CPPUNIT_ASSERT(SingletonTest::m_desstructed);
}

