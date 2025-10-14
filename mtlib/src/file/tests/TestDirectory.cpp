/*
 * Title       : TestDirectory.cpp
 * Date        : 16.07.2010
 * Author      : Andrei Borodaenko
 * Description : ***
 */

#include <mt/file/Directory.h>
#include <mt/common/Exception.h>
#include "TestDirectory.h"

#include  <algorithm>

CPPUNIT_TEST_SUITE_REGISTRATION(TestDirectory);

void TestDirectory::setUp() {

}

void TestDirectory::tearDown() {

}

void TestDirectory::testCreation() {
    CPPUNIT_ASSERT_NO_THROW(mt::Directory("/usr/local"));
    CPPUNIT_ASSERT_NO_THROW(mt::Directory("/usr/local/"));
}

void TestDirectory::testCreationFailure() {
	CPPUNIT_ASSERT_THROW(mt::Directory("/aaa/bbb/ccc"), mt::Exception);
}

void TestDirectory::testGetEntries() {
    mt::Directory dir("../src/tests/data");
    const std::vector<std::string>& entries = dir.getEntries();
    CPPUNIT_ASSERT_EQUAL(3 + 1/*.svn*/, (int)entries.size());
    CPPUNIT_ASSERT(std::find(entries.begin(), entries.end(), std::string("entry0.test")) != entries.end());
    CPPUNIT_ASSERT(std::find(entries.begin(), entries.end(), std::string("entry1.test")) != entries.end());
    CPPUNIT_ASSERT(std::find(entries.begin(), entries.end(), std::string("entry2.test")) != entries.end());
}
