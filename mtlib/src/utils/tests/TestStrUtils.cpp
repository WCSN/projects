#include <mt/utils/StrUtils.h>

#include "TestStrUtils.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestStrUtils);

void TestStrUtils::setUp() {
}

void TestStrUtils::tearDown() {
}

void TestStrUtils::testDetokenize() {
	std::vector<std::string> v;
	v.push_back("1");
	v.push_back("2");
	v.push_back("3");

	CPPUNIT_ASSERT_EQUAL(std::string("1,2,3"), mt::StrUtils::detokenize(v, ","));
}



