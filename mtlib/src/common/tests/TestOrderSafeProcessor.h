/*
 * TestOrderSafeProcessor.h
 *
 *  Created on: 26.10.2010
 *      Author: hakamasy
 */

#ifndef TESTORDERSAFEPROCESSOR_H_
#define TESTORDERSAFEPROCESSOR_H_

#include <cppunit/extensions/HelperMacros.h>

class TestOrderSafeProcessor : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TestOrderSafeProcessor);
    CPPUNIT_TEST(testProcessor);
    CPPUNIT_TEST(testOrder);
    CPPUNIT_TEST(testLiteResize);
    CPPUNIT_TEST(testHardResize);

    CPPUNIT_TEST_SUITE_END();

protected:
    void testProcessor();
    void testOrder();
    void testLiteResize();
    void testHardResize();

public:
    void setUp();
    void tearDown();
};

#endif /* TESTORDERSAFEPROCESSOR_H_ */
