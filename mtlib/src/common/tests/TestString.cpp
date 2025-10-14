#include "TestString.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestString);

void TestString::setUp() {
}

void TestString::tearDown() {
}

void TestString::testcreationByCharString() {
	char* str = "test string";
	mt::String test(str);
	CPPUNIT_ASSERT(strcmp(str, test.data()) == 0);
}

void TestString::testcreateDefault() {
	mt::String string;
	CPPUNIT_ASSERT(strcmp("", string.data()) == 0);
}

void TestString::testequalOperatorWithString() {
	char* str = "test string";
	CPPUNIT_ASSERT(mt::String(str) == mt::String(str));
	CPPUNIT_ASSERT(!(mt::String(str) == mt::String()));
	CPPUNIT_ASSERT(mt::String() == mt::String());
}

void TestString::testnotEqualOperatorWithString() {
	char* str = "test string";
	CPPUNIT_ASSERT(!(mt::String(str) != mt::String(str)));
	CPPUNIT_ASSERT(mt::String(str) != mt::String("not equal string"));
}

void TestString::testequalOperatorWithCharString() {
	char* str = "test string";
	CPPUNIT_ASSERT(mt::String(str) == str);
	CPPUNIT_ASSERT(str == mt::String(str));
	CPPUNIT_ASSERT(!("" == mt::String(str)));
	CPPUNIT_ASSERT(!(mt::String(str) == "not equal string"));

	mt::String test = "";
	CPPUNIT_ASSERT(!(test == "test"));
}

void TestString::testnotEqualOperatorWithCharString() {
	char* str = "test string";
	CPPUNIT_ASSERT(!(mt::String(str) != str));
	CPPUNIT_ASSERT(!(str != mt::String(str)));
	CPPUNIT_ASSERT("" != mt::String(str));
	CPPUNIT_ASSERT(mt::String(str) != "not equal string");
}

void TestString::testinitBySubString() {
	char* str = "test string";
	CPPUNIT_ASSERT(mt::String(str, 4) == "test");
	CPPUNIT_ASSERT(mt::String(str, 11) == str);
	CPPUNIT_ASSERT(mt::String(str, 0) == "");

	mt::String tested_str(str, 4);
	CPPUNIT_ASSERT_EQUAL(4, (int)tested_str.size());
	CPPUNIT_ASSERT('\0' == *(tested_str.data() + 4));

	mt::String tested_str2(str, 11);
	CPPUNIT_ASSERT_EQUAL(11, (int)tested_str2.size());
	CPPUNIT_ASSERT('\0' == *(tested_str2.data() + 11));
}

void TestString::testinitByChar() {
	CPPUNIT_ASSERT(mt::String(5, 'a') == "aaaaa");
	CPPUNIT_ASSERT(mt::String(0, 'a') == "");
	CPPUNIT_ASSERT(mt::String(1, 'a') == "a");
}

void TestString::testinitBySubStr() {
	mt::String str("test string");
	CPPUNIT_ASSERT(mt::String(str, 5, 6) == "string");
	CPPUNIT_ASSERT(mt::String(str, 10, 1) == "g");
	CPPUNIT_ASSERT(mt::String(str, 10) == "g");
	CPPUNIT_ASSERT(mt::String(str, 0) == str);
}

void TestString::testtestLength() {
	CPPUNIT_ASSERT_EQUAL(11, (int)mt::String("test string").length());
	CPPUNIT_ASSERT_EQUAL(4, (int)mt::String("test").length());
	CPPUNIT_ASSERT_EQUAL(0, (int)mt::String("").length());
	CPPUNIT_ASSERT_EQUAL(2, (int)mt::String("\0\0", 2).length());
}

void TestString::testisEmpty() {
	CPPUNIT_ASSERT(mt::String().empty());
	CPPUNIT_ASSERT(!mt::String("test").empty());
}

void TestString::testtestAppendCharOperator() {
	mt::String string("test");
	string += '2';
	CPPUNIT_ASSERT(string == "test2");
	
	mt::String string2;
	string2 += 'a';
	CPPUNIT_ASSERT(string2 == "a");
}

void TestString::testtestAppendCharStringOperator() {
	mt::String string("test");
	string += " string";
	CPPUNIT_ASSERT(string == "test string");

	mt::String string2;
	string2 += "test";
	CPPUNIT_ASSERT(string2 == "test");
}

void TestString::testtestAppendStringOperator() {
	mt::String string("test");
	string += mt::String(" string\0\0", 9);
	CPPUNIT_ASSERT_EQUAL(13, (int)string.length());
	CPPUNIT_ASSERT(string == mt::String("test string\0\0", 13));

	mt::String string2;
	string2 += mt::String("test");
	CPPUNIT_ASSERT(string2 == "test");
}

void TestString::testfindCharStringFromPos() {
	mt::String string("testing string");
	CPPUNIT_ASSERT_EQUAL(0, (int)string.find("test"));
	CPPUNIT_ASSERT_EQUAL(-1, (int)string.find("a"));
	CPPUNIT_ASSERT_EQUAL(2, (int)string.find("st"));
	CPPUNIT_ASSERT_EQUAL(8, (int)string.find("st", 3));
}

void TestString::testfindCharFromPos() {
	mt::String string("testing string");
	CPPUNIT_ASSERT_EQUAL(0, (int)string.find('t'));
	CPPUNIT_ASSERT_EQUAL(-1, (int)string.find('a'));
	CPPUNIT_ASSERT_EQUAL(8, (int)string.find('s', 3));
}

void TestString::testfindStringFromPos() {
	mt::String string("testing string");
	CPPUNIT_ASSERT_EQUAL(0, (int)string.find(mt::String("test")));
	//CPPUNIT_ASSERT_EQUAL(-1, string.find(mt::String("")));
	CPPUNIT_ASSERT_EQUAL(-1, (int)string.find(mt::String("a")));
	CPPUNIT_ASSERT_EQUAL(2, (int)string.find(mt::String("st")));
	CPPUNIT_ASSERT_EQUAL(8, (int)string.find(mt::String("st"), 3));
}

void TestString::testsubstr() {
	mt::String string("test string");
	CPPUNIT_ASSERT(string.substr(0, 11) == "test string");
	//CPPUNIT_ASSERT(string.substr(0, 2000) == "test string");
	CPPUNIT_ASSERT(string.substr(5, 6) == "string");
}

void TestString::testreplace() {
	mt::String string("test string");
	CPPUNIT_ASSERT(string.replace(0, 4, "tempo") == "tempo string");
	string = "test string";
	CPPUNIT_ASSERT(string.replace(10, 1, "G") == "test strinG");
	string = "test string";
	CPPUNIT_ASSERT(string.replace(4, 1, "") == "teststring");
}

void TestString::testreplaceByString() {
	mt::String string("test string");
	CPPUNIT_ASSERT(string.replace(0, 4, mt::String("tempo")) == "tempo string");
	string = "test string";
	CPPUNIT_ASSERT(string.replace(10, 1, mt::String("G")) == "test strinG");
	string = "test string";
	CPPUNIT_ASSERT(string.replace(4, 1, mt::String()) == "teststring");
}

void TestString::testeraseN() {
	mt::String string("test string");
	CPPUNIT_ASSERT(string.erase(0, 11) == "");
	
	string = "test string";
	CPPUNIT_ASSERT(string.erase(10, 1) == "test strin");
	CPPUNIT_ASSERT(string.erase(10, 1) == "test strin");
	CPPUNIT_ASSERT(string.erase(9, 1000) == "test stri");
}

void TestString::testeraseFromPos() {
	mt::String string("test string");
	CPPUNIT_ASSERT(string.erase(4) == "test");
	CPPUNIT_ASSERT(string.erase(0) == "");
	CPPUNIT_ASSERT(string.erase(0) == "");
}

void TestString::testerase() {
	mt::String string("test string");
	CPPUNIT_ASSERT(string.erase() == "");
}

void TestString::testinsertMtString() {
	mt::String string("test string");
	CPPUNIT_ASSERT(string.insert(5, mt::String("fuck ")) == "test fuck string");
	CPPUNIT_ASSERT(string.insert(5, mt::String("")) == "test fuck string");
}

void TestString::testinsertChar() {
	mt::String string("test string");
	CPPUNIT_ASSERT(string.insert(5, 5, 'a') == "test aaaaastring");
}

void TestString::testassign() {
	mt::String string("test string");
	CPPUNIT_ASSERT(string.assign("test", 4) == "test");
	CPPUNIT_ASSERT(string.assign("test", 0) == "");
}

void TestString::testappend() {
	mt::String string("test string");
	CPPUNIT_ASSERT(string.append("2") == "test string2");

	mt::String appended_str("new string");
	CPPUNIT_ASSERT(string.append(appended_str, 0, 3) == "test string2new");
}

void TestString::testcompare() {
	mt::String string("b");
	CPPUNIT_ASSERT(string.compare(mt::String("b")) == 0);
	CPPUNIT_ASSERT(string.compare("c") < 0);
	CPPUNIT_ASSERT(string.compare("a") > 0);
}

void TestString::testoperatorGet() {
	mt::String string("test string");
	CPPUNIT_ASSERT(string[0] == 't');
}

void TestString::testclear() {
	mt::String string("test string");
	string.clear();
	CPPUNIT_ASSERT(string == "");
}

void TestString::testtestConcat() {
	mt::String string("test");
	CPPUNIT_ASSERT(string + " string" == "test string");
	CPPUNIT_ASSERT("string " + string == "string test");
}

void TestString::testoperatorLess() {
	mt::String a("a"), b("b"), c("c");
	CPPUNIT_ASSERT(a < b);
	CPPUNIT_ASSERT(b < c);
	CPPUNIT_ASSERT(a < c);
}

void TestString::testtoUpper() {
	mt::String string("test string");
	size_t res = string.toUpper();
	CPPUNIT_ASSERT(string == "TEST STRING");
	CPPUNIT_ASSERT_EQUAL(10, (int)res);
	CPPUNIT_ASSERT_EQUAL(0, (int)string.toUpper());
	
	string = "";
	string.toUpper();
	CPPUNIT_ASSERT(string == "");
}

void TestString::testlengthOfString() {
	mt::String str("test");
	str += '\0';
	str += '\0';
	str += '\0';
	CPPUNIT_ASSERT_EQUAL(4 + 3, (int)str.length());
}
