#ifndef __NETWORK__TESTS__TEST_DATA_PACKET__H
#define __NETWORK__TESTS__TEST_DATA_PACKET__H

#include <cppunit/extensions/HelperMacros.h>

class TestDataPacket: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestDataPacket);
	CPPUNIT_TEST(testSetHostsAndPortsByInteger);
	CPPUNIT_TEST(testSetHostsAndPortsByString);
	CPPUNIT_TEST(testSetHostsAndPortsByDefault);
	CPPUNIT_TEST(testSetHostsAndPortsByStringIfStringIncorrect);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testSetHostsAndPortsByInteger();
	void testSetHostsAndPortsByString();
	void testSetHostsAndPortsByDefault();
	void testSetHostsAndPortsByStringIfStringIncorrect();

public:
	void setUp();
	void tearDown();
};

#endif //__NETWORK__TESTS__TEST_DATA_PACKET__H
