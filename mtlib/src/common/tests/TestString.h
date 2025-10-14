#ifndef __TESTSTRING__H
#define __TESTSTRING__H

#include <cppunit/extensions/HelperMacros.h>
#include "mt/common/String.h"

class TestString: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestString);
	CPPUNIT_TEST(testcreationByCharString);
	CPPUNIT_TEST(testcreateDefault);
	CPPUNIT_TEST(testequalOperatorWithString);
	CPPUNIT_TEST(testnotEqualOperatorWithString);
	CPPUNIT_TEST(testequalOperatorWithCharString);
	CPPUNIT_TEST(testnotEqualOperatorWithCharString);
	CPPUNIT_TEST(testinitBySubString);
	CPPUNIT_TEST(testinitByChar);
	CPPUNIT_TEST(testinitBySubStr);
	CPPUNIT_TEST(testtestLength);
	CPPUNIT_TEST(testisEmpty);
	CPPUNIT_TEST(testtestAppendCharOperator);
	CPPUNIT_TEST(testtestAppendCharStringOperator);
	CPPUNIT_TEST(testtestAppendStringOperator);
	CPPUNIT_TEST(testfindCharStringFromPos);
	CPPUNIT_TEST(testfindCharFromPos);
	CPPUNIT_TEST(testfindStringFromPos);
	CPPUNIT_TEST(testsubstr);
	CPPUNIT_TEST(testreplace);
	CPPUNIT_TEST(testreplaceByString);
	CPPUNIT_TEST(testeraseN);
	CPPUNIT_TEST(testeraseFromPos);
	CPPUNIT_TEST(testerase);
	CPPUNIT_TEST(testinsertMtString);
	CPPUNIT_TEST(testinsertChar);
	CPPUNIT_TEST(testassign);
	CPPUNIT_TEST(testappend);
	CPPUNIT_TEST(testcompare);
	CPPUNIT_TEST(testoperatorGet);
	CPPUNIT_TEST(testclear);
	CPPUNIT_TEST(testtestConcat);
	CPPUNIT_TEST(testoperatorLess);
	CPPUNIT_TEST(testtoUpper);
	CPPUNIT_TEST(testlengthOfString);
	CPPUNIT_TEST_SUITE_END();

 protected:
	void testcreationByCharString();
	void testcreateDefault();
	void testequalOperatorWithString();
	void testnotEqualOperatorWithString();
	void testequalOperatorWithCharString();
	void testnotEqualOperatorWithCharString();
	void testinitBySubString();
	void testinitByChar();
	void testinitBySubStr();
	void testtestLength();
	void testisEmpty();
	void testtestAppendCharOperator();
	void testtestAppendCharStringOperator();
	void testtestAppendStringOperator();
	void testfindCharStringFromPos();
	void testfindCharFromPos();
	void testfindStringFromPos();
	void testsubstr();
	void testreplace();
	void testreplaceByString();
	void testeraseN();
	void testeraseFromPos();
	void testerase();
	void testinsertMtString();
	void testinsertChar();
	void testassign();
	void testappend();
	void testcompare();
	void testoperatorGet();
	void testclear();
	void testtestConcat();
	void testoperatorLess();
	void testtoUpper();
	void testlengthOfString();
	
 public:
	void setUp();
	void tearDown();
};

#endif /* __TESTSTRING__H */
