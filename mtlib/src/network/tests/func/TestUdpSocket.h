#ifndef __NETWORK__TESTS__FUNC__TEST_UDP_SOCKET__H
#define __NETWORK__TESTS__FUNC__TEST_UDP_SOCKET__H

#include <cppunit/extensions/HelperMacros.h>

class TestUdpSocket: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestUdpSocket);
	CPPUNIT_TEST(testSendReceiveData);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testSendReceiveData();
	
public:
	void setUp();
	void tearDown();
	
};

#endif //__NETWORK__TESTS__FUNC__TEST_UDP_SOCKET__H
