#ifndef __DATA__MYSQL__TEST__FUNC__TEST_EXEC_MYSQL_QUERY__H
#define __DATA__MYSQL__TEST__FUNC__TEST_EXEC_MYSQL_QUERY__H

#include <cppunit/extensions/HelperMacros.h>

#include <mt/data/mysql/MysqlConnector.h>

class TestExecMysqlQuery: public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE(TestExecMysqlQuery);
	CPPUNIT_TEST(testCreateTable);
	CPPUNIT_TEST(testInsertQuery);
	CPPUNIT_TEST(testUpdateTable);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testCreateTable();
	void testInsertQuery();
	void testUpdateTable();
public:
	void setUp();
	void tearDown();

private:
	void createTable();

private:
	mt::MysqlConnectorPtr m_connector;
};

#endif //__DATA__MYSQL__TEST__FUNC__TEST_EXEC_MYSQL_QUERY__H
