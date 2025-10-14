/*
 * Title       : TestDynamicBuffer.h
 * Date        : 26.07.2010
 * Author      : Andrei Borodaenko
 * Description :
 */

#ifndef TESTDYNAMICBUFFER_H_
#define TESTDYNAMICBUFFER_H_

#include <cppunit/extensions/HelperMacros.h>

class TestDynamicBuffer : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TestDynamicBuffer);
    CPPUNIT_TEST(testCreation);
    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST(testFunctions);
    CPPUNIT_TEST_SUITE_END();

protected:
    void testCreation();
    void testOperators();
    void testFunctions();

public:
    void setUp();
    void tearDown();
};

#endif /* TESTDYNAMICBUFFER_H_ */
