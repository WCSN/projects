#include "TestXmlConfig.h"
#include <mt/common/XmlConfig.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TestXmlConfig);

const std::string PARAM1_VALUE = "test_param1_value";
const std::string PARAM2_VALUE = "test_param2_value";

std::string TestXmlConfig::createConfigString() {
	return "<TEST_CFG test_param1=\"" + PARAM1_VALUE + "\">\
<TEST_CFG2 test_param2=\"" + PARAM2_VALUE + "\"/>\
</TEST_CFG>";
}

void TestXmlConfig::setUp() {
}

void TestXmlConfig::tearDown() {
}

void TestXmlConfig::testInitFromString() {
	mt::XmlConfig config(createConfigString().c_str());
	CPPUNIT_ASSERT_EQUAL(PARAM1_VALUE, config.get("test_param1"));
	CPPUNIT_ASSERT_EQUAL(PARAM2_VALUE, config.get("TEST_CFG2", "test_param2"));
}

void TestXmlConfig::testSetValue() {
	mt::XmlConfig config(createConfigString().c_str());
	const std::string NEW_VALUE1 = "new_value1";
	const std::string NEW_VALUE2 = "new_value2";
	config.put("test_param1", NEW_VALUE1);
	config.put("TEST_CFG2", "test_param2", NEW_VALUE2);

	CPPUNIT_ASSERT_EQUAL(NEW_VALUE1, config.get("test_param1"));
	CPPUNIT_ASSERT_EQUAL(NEW_VALUE2, config.get("TEST_CFG2", "test_param2"));
}

void TestXmlConfig::testGetName() {
	mt::XmlConfig config(createConfigString().c_str());
	CPPUNIT_ASSERT_EQUAL(std::string("TEST_CFG"), config.getName()); 
}

void TestXmlConfig::testGetMandatory() {
	mt::XmlConfig config(createConfigString().c_str());
	try {
		config.getMandatory("NoSuchParameter");
		CPPUNIT_FAIL("Exception no throws");
	} catch (const mt::Exception& me) {
		CPPUNIT_ASSERT_EQUAL(std::string("No mandatory parameter at configuration: No parameter 'NoSuchParameter'"), me.displayText());
	}
}
