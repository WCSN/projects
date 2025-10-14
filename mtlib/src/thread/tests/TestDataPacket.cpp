#include "TestDataPacket.h"
#include <mt/network/DataPacket.h>
#include <mt/common/Exception.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TestDataPacket);

void TestDataPacket::setUp(){
}

void TestDataPacket::tearDown(){
}

void TestDataPacket::testSetHostsAndPortsByInteger() {
	mt::DataPacket packet(0x0A0A0A0A, 
												100, 
												0xFFFFFFFF, 
												200, 
												mt::ByteBuffer());
	CPPUNIT_ASSERT_EQUAL(std::string("10.10.10.10"), packet.srcHostAsString());
	CPPUNIT_ASSERT_EQUAL((int)0x0A0A0A0A, (int)packet.srcHost());
	CPPUNIT_ASSERT_EQUAL(std::string("255.255.255.255"), packet.dstHostAsString());
	CPPUNIT_ASSERT_EQUAL((int)0xFFFFFFFF, (int)packet.dstHost());
	CPPUNIT_ASSERT_EQUAL(100, (int)packet.srcPort());
	CPPUNIT_ASSERT_EQUAL(200, (int)packet.dstPort());
}

void TestDataPacket::testSetHostsAndPortsByString() {
	mt::DataPacket packet("10.10.10.10", 
												100, 
												"255.255.255.255", 
												200, 
												mt::ByteBuffer());
	CPPUNIT_ASSERT_EQUAL((int)0x0A0A0A0A, (int)packet.srcHost());
	CPPUNIT_ASSERT_EQUAL((unsigned int)0xFFFFFFFF, (unsigned int)packet.dstHost());
	CPPUNIT_ASSERT_EQUAL(100, (int)packet.srcPort());
	CPPUNIT_ASSERT_EQUAL(200, (int)packet.dstPort());
}

void TestDataPacket::testSetHostsAndPortsByDefault() {
	mt::DataPacket packet;
	CPPUNIT_ASSERT_EQUAL(0, (int)packet.srcHost());
	CPPUNIT_ASSERT_EQUAL(0, (int)packet.dstHost());
	CPPUNIT_ASSERT_EQUAL(0, (int)packet.srcPort());
	CPPUNIT_ASSERT_EQUAL(0, (int)packet.dstPort());
}

void TestDataPacket::testSetHostsAndPortsByStringIfStringIncorrect() {
	CPPUNIT_ASSERT_THROW(mt::DataPacket ("Incorrect ip string", 100, "255.255.255.255", 200, mt::ByteBuffer()), mt::Exception);
}

