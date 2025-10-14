#ifndef __DATA__MYSQL__TESTS__TEST_DB_COLUMN__H
#define __DATA__MYSQL__TESTS__TEST_DB_COLUMN__H

#include <cppunit/extensions/HelperMacros.h>

class TestDbColumn: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestDbColumn);
	CPPUNIT_TEST(testEscapeString);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testEscapeString();
	
public:
	void setUp();
	void tearDown();

};

#endif //__DATA__MYSQL__TESTS__TEST_DB_COLUMN__H
