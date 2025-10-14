/*
 * TestLogable.h
 *
 *  Created on: 18.07.2010
 *      Author: hakamasy
 */

#ifndef TESTLOGABLE_H_
#define TESTLOGABLE_H_

#include <cppunit/extensions/HelperMacros.h>
#include "mt/logger/Logable.h"
#include "mt/common/XmlConfig.h"


#define TEST_MESSAGE "Test log message"

using namespace mt;

class LogableClass : public Logable {


	public:
		LogableClass() {

			MT_LOGINIT(LogableClass);

		};

		void logDebug(){
			MT_LOGDEBUG(TEST_MESSAGE);
		};
};


class TestLoggerFunc: public CPPUNIT_NS::TestFixture {
	LogableClass* m_testLogable;
	XmlConfigPtr m_testInitByXmlCfg;


	CPPUNIT_TEST_SUITE(TestLoggerFunc);
	CPPUNIT_TEST(testLogDebugMacros);

	/*
	CPPUNIT_TEST(testLogWarningMacros);
	CPPUNIT_TEST(testLogTraceMacros);
	CPPUNIT_TEST(testLogInfoMacros);
	CPPUNIT_TEST(testLogErrorMacros);
*/
	CPPUNIT_TEST_SUITE_END();

protected:

	void testLogDebugMacros();

	/*
	void testLogWarningMacros();
	void testLogTraceMacros();
	void testLogInfoMacros();
	void testLogErrorMacros();

*/
public:
	TestLoggerFunc(){
		m_testLogable = NULL;
	}

	void setUp();
	void tearDown();

private:


};



#endif /* TESTLOGABLE_H_ */
