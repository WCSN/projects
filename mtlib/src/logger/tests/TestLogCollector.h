/*
 * TestLoggerFactory.h
 *
 *  Created on: 12.08.2010
 *      Author: hakamasy
 */

#ifndef TESTLOGCOLLECTOR_H_
#define TESTLOGCOLLECTOR_H_


#include <cppunit/extensions/HelperMacros.h>
#include "mt/logger/LoggerFactory.h"
#include "mt/common/XmlConfig.h"

//class TestingLoggerFactory;

using namespace mt;

class TestLogCollector: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestLogCollector);

	CPPUNIT_TEST(testInit);

	CPPUNIT_TEST_SUITE_END();

protected:

	XmlConfigPtr m_testInitByXmlCfg;
	void testInit();


public:
	void setUp();
	void tearDown();

};


#endif /* TESTLOGGERFACTORY_H_ */
