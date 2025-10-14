/*
 * TestOrderSafeProcessor.cpp
 *
 *  Created on: 26.10.2010
 *      Author: hakamasy
 */

#include "TestOrderSafeProcessor.h"

#include <mt/common/OrderSafeProcessor.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TestOrderSafeProcessor);

class DataIn {

public:

	DataIn(const std::string& str) {
		data = str;
	}
	DataIn(uint32_t i) {
		m_i = i;
	}

	std::string data;
	uint32_t m_i;
};

class DataOut {

public:

	std::string field1;
	std::string field2;
	std::string field3;
	uint32_t i;

};

class Parser: public mt::OrderSafeProcessorInterface<DataIn*, DataOut*> {

public:

	virtual ~Parser(){};
	DataOut* run(DataIn* data) {
		DataOut* ret = new DataOut();
		ret->field1 = data->data;
		ret->i = data->m_i;
		int32_t rnd = random();
		usleep(rnd % 500);
		return ret;
	}
};

void TestOrderSafeProcessor::setUp() {

}

void TestOrderSafeProcessor::testProcessor() {

	uint32_t threadCnt = 2;
	Parser* parser = new Parser();

	char buf[256];

	for (uint32_t i = 0; i < 10; i++) {
		sprintf(buf, "1");
	}
	mt::OrderSafeProcessor<DataIn*, DataOut*> proc(parser, threadCnt);
	proc.start();

	DataIn dataIn("test1");
	proc.push(&dataIn);

	DataOut *dataOut = proc.pop();

	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorect parsing", 0,
			dataOut->field1.compare("test1"));

}

void TestOrderSafeProcessor::testOrder() {

	uint32_t threadCnt = 10;
	Parser* parser = new Parser();

	mt::OrderSafeProcessor<DataIn*, DataOut*> proc(parser, threadCnt);
	proc.start();

	for (uint32_t i = 0; i < 1000; i++) {
		DataIn *dataIn = new DataIn(i);
		proc.push(dataIn);
	}

	for (uint32_t i = 0; i < 1000; i++) {
		DataOut *dataOut = proc.pop();
		CPPUNIT_ASSERT_EQUAL_MESSAGE("incorect order", i, dataOut->i);

	}

}

void TestOrderSafeProcessor::testLiteResize() {

	uint32_t threadCnt = 10;
	Parser* parser = new Parser();

	mt::OrderSafeProcessor<DataIn*, DataOut*> proc(parser, threadCnt);
	proc.start();

	for (uint32_t i = 0; i < 256; i++) {
		DataIn *dataIn = new DataIn(i);
		proc.push(dataIn);
	}

	proc.enlargeOutputQueue(512);

	for (uint32_t i = 256; i < 500; i++) {
		DataIn *dataIn = new DataIn(i);
		proc.push(dataIn);
	}

	for (uint32_t i = 0; i < 500; i++) {
		DataOut *dataOut = proc.pop();
		CPPUNIT_ASSERT_EQUAL_MESSAGE("incorect order", i, dataOut->i);
	}

}

void TestOrderSafeProcessor::testHardResize() {
	uint32_t threadCnt = 10;
	Parser* parser = new Parser();

	mt::OrderSafeProcessor<DataIn*, DataOut*> proc(parser, threadCnt);
	proc.start();

	for (uint32_t i = 0; i < 256; i++) {
		DataIn *dataIn = new DataIn(i);
		proc.push(dataIn);
	}

	for (uint32_t i = 0; i < 100; i++) {
		DataOut *dataOut = proc.pop();
		CPPUNIT_ASSERT_EQUAL_MESSAGE("incorect order first", i, dataOut->i);
	}

	for (uint32_t i = 256; i < 300; i++) {
		DataIn *dataIn = new DataIn(i);
		proc.push(dataIn);
	}

	proc.enlargeOutputQueue(512);

	for (uint32_t i = 100; i < 300; i++) {
		DataOut *dataOut = proc.pop();
		CPPUNIT_ASSERT_EQUAL_MESSAGE("incorect order", i, dataOut->i);
	}

}

void TestOrderSafeProcessor::tearDown() {

}
