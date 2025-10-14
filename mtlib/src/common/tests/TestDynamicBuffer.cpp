/*
 * Title       : TestDynamicBuffer.cpp
 * Date        : 26.07.2010
 * Author      : Andrei Borodaenko
 * Description : ***
 */

#include "TestDynamicBuffer.h"

#include <mt/common/DynamicBuffer.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TestDynamicBuffer);

void TestDynamicBuffer::setUp() {

}

void TestDynamicBuffer::tearDown() {

}

void TestDynamicBuffer::testCreation() {
    mt::DynamicBuffer<char> buffer0;
    CPPUNIT_ASSERT_EQUAL(size_t(0), buffer0.getSize());
    CPPUNIT_ASSERT_EQUAL((char*)(0), buffer0.getDataPtr());
    CPPUNIT_ASSERT_EQUAL(size_t(0), buffer0.getCapacity());
}

void TestDynamicBuffer::testOperators() {
    mt::DynamicBuffer<char> buffer0(3);
    mt::DynamicBuffer<char> buffer1(5);
    buffer1.setSize(4);
    buffer1[0] = 4;
    buffer1[1] = 3;
    buffer1[2] = 2;
    buffer1[3] = 1;
    buffer0 = buffer1;
    CPPUNIT_ASSERT_EQUAL(size_t(4), buffer0.getCapacity());
    CPPUNIT_ASSERT_EQUAL(size_t(4), buffer0.getSize());
    CPPUNIT_ASSERT_EQUAL(char(4), buffer0[0]);
    CPPUNIT_ASSERT_EQUAL(char(3), buffer0[1]);
    CPPUNIT_ASSERT_EQUAL(char(2), buffer0[2]);
    CPPUNIT_ASSERT_EQUAL(char(1), buffer0[3]);
    CPPUNIT_ASSERT(buffer0 == buffer1);
    mt::DynamicBuffer<char> buffer2(5);
    CPPUNIT_ASSERT(buffer0 != buffer2);
}

void TestDynamicBuffer::testFunctions() {
    mt::DynamicBuffer<char> buffer;
    buffer.reallocate(50000);
    CPPUNIT_ASSERT_EQUAL(size_t(50000), buffer.getCapacity());
    CPPUNIT_ASSERT_EQUAL(size_t(0), buffer.getSize());
    buffer.setSize(40000);
    CPPUNIT_ASSERT_EQUAL(size_t(50000), buffer.getCapacity());
    CPPUNIT_ASSERT_EQUAL(size_t(40000), buffer.getSize());
    buffer.clear();
    CPPUNIT_ASSERT_EQUAL(size_t(0), buffer.getSize());
    CPPUNIT_ASSERT_EQUAL((char*)(0), buffer.getDataPtr());
    CPPUNIT_ASSERT_EQUAL(size_t(0), buffer.getCapacity());

    // test append
    mt::DynamicBuffer<char> buffer0(2);
    buffer0.setSize(2);
    buffer0[0] = 10;
    buffer0[1] = 11;
    char* ptr = buffer0.getDataPtr();
    char* data = new char[3];
    data[0] = 20;
    data[1] = 21;
    data[2] = 22;
    buffer0.append(data, 3);
    CPPUNIT_ASSERT_EQUAL(size_t(5), buffer0.getSize());
    CPPUNIT_ASSERT_EQUAL(size_t(5), buffer0.getCapacity());
    CPPUNIT_ASSERT_EQUAL(char(10), buffer0[0]);
    CPPUNIT_ASSERT_EQUAL(char(11), buffer0[1]);
    CPPUNIT_ASSERT_EQUAL(char(20), buffer0[2]);
    CPPUNIT_ASSERT_EQUAL(char(21), buffer0[3]);
    CPPUNIT_ASSERT_EQUAL(char(22), buffer0[4]);
    CPPUNIT_ASSERT(ptr != buffer0.getDataPtr());
}
