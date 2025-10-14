#ifndef __COMMON__TESTS__TEST_XML_CONFIG__H
#define __COMMON__TESTS__TEST_XML_CONFIG__H

#include <cppunit/extensions/HelperMacros.h>

class TestXmlConfig: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestXmlConfig);
	CPPUNIT_TEST(testInitFromString);
	CPPUNIT_TEST(testSetValue);
	CPPUNIT_TEST(testGetName);
	CPPUNIT_TEST(testGetMandatory);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testInitFromString();
	void testSetValue();
	void testGetName();
	void testGetMandatory();

public:
	void setUp();
	void tearDown();

private:
	std::string createConfigString();
};

#endif //__COMMON__TESTS__TEST_XML_CONFIG__H
