/*
 * Title       : TestDirectory.h
 * Date        : 16.07.2010
 * Author      : Andrei Borodaenko
 * Description :
 */

#ifndef TESTDIRECTORY_H_
#define TESTDIRECTORY_H_

#include <cppunit/extensions/HelperMacros.h>

class TestDirectory : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TestDirectory);
    CPPUNIT_TEST(testCreation);
    CPPUNIT_TEST(testGetEntries);
    CPPUNIT_TEST_SUITE_END();

protected:
    void testCreation();
    void testCreationFailure();
    void testGetEntries();

public:
    void setUp();
    void tearDown();
};

#endif /* TESTDIRECTORY_H_ */
