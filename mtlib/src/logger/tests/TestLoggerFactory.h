/*
 * TestLoggerFactory.h
 *
 *  Created on: 12.08.2010
 *      Author: hakamasy
 */

#ifndef TESTLOGGERFACTORY_H_
#define TESTLOGGERFACTORY_H_


#include <cppunit/extensions/HelperMacros.h>
#include "mt/logger/LoggerFactory.h"
#include "mt/common/XmlConfig.h"

//class TestingLoggerFactory;

using namespace mt;

class TestLoggerFactory: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TestLoggerFactory);
	CPPUNIT_TEST(testUniqueLoggerGarantee);
	CPPUNIT_TEST(testInitByXml);
	CPPUNIT_TEST(testParseXmlConfig);
	CPPUNIT_TEST(testReinit);

	CPPUNIT_TEST_SUITE_END();

protected:

	XmlConfigPtr m_testInitByXmlCfg;
	void testUniqueLoggerGarantee();
	void testReinit();
	void testParseXmlConfig();
	void testInitByXml();


public:
	void setUp();
	void tearDown();

};


#endif /* TESTLOGGERFACTORY_H_ */
