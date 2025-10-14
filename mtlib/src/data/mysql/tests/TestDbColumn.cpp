#include "TestDbColumn.h"
#include <mt/data/mysql/DbColumn.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TestDbColumn);


void TestDbColumn::setUp() {
}

void TestDbColumn::tearDown() {
}

void TestDbColumn::testEscapeString() {
	std::string test_string = "Test \\string\\";
	CPPUNIT_ASSERT_EQUAL(std::string("Test \\\\string\\\\"), mt::DbColumn::escapeString(test_string));
}



