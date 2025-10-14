#include "TestException.h"
#include "mt/common/Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestException);

void TestException::setUp() {
}

void TestException::tearDown() {
}

void TestException::testSetValueFromStream()
{
	const int ERROR_CAUSE = 1;
	try {
		MT_EXCEPTION(mt::Exception, "Testing error.Reason: " << ERROR_CAUSE);
		CPPUNIT_FAIL("No exception");
	}
	catch(const mt::Exception& me) {
		CPPUNIT_ASSERT_EQUAL(std::string("Testing error.Reason: 1"), me.message());
	}
}

