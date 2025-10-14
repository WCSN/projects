#include "TestUdpSocket.h"
#include <mt/network/UdpSocket.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TestUdpSocket);

void TestUdpSocket::setUp() {
}

void TestUdpSocket::tearDown() {
}

void TestUdpSocket::testSendReceiveData() {
	mt::UdpSocket sender("127.0.0.1", "5555");
	mt::UdpSocket receiver("127.0.0.1", "6666");

	const std::string sended_data = "Test socket data";
	sender.sendTo(sended_data, "127.0.0.1", 6666);
	mt::DataPacketPtr data;
	CPPUNIT_ASSERT(receiver.recv(&data, 10));
	CPPUNIT_ASSERT(data);
	CPPUNIT_ASSERT_EQUAL((int)sended_data.size(), (int)data->data().getSize());

//add string null terminating character
	char buf[data->data().getSize()+1];
	memcpy(buf, data->data(), data->data().getSize());
	buf[data->data().getSize()] = '\0';

	CPPUNIT_ASSERT_EQUAL(sended_data, std::string((const char*)buf));
}
