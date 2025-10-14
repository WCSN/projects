#include "TestExecMysqlQuery.h"
#include <string>

const std::string DB_HOST = "127.0.0.1";
const std::string DB_USER = "root";
const std::string DB_PASS = "";
const std::string DB_DATABASE = "mtlib_test";
const std::string DB_TABLENAME = "tTestTable";

CPPUNIT_TEST_SUITE_REGISTRATION(TestExecMysqlQuery);

void TestExecMysqlQuery::createTable() {
	mt::DbDataPtr data = m_connector->exec("CREATE TABLE " + DB_TABLENAME + " (TestClass VARCHAR(255), TestMethod VARCHAR(255), RequestId INTEGER)");
	CPPUNIT_ASSERT(data);
}

void TestExecMysqlQuery::setUp() {
	m_connector = new mt::MysqlConnector(DB_HOST, DB_USER, DB_PASS, DB_DATABASE);
	CPPUNIT_ASSERT(m_connector->init() >= 0);
	try{
		m_connector->exec("DROP TABLE " + DB_TABLENAME);
	}catch(const mt::DbException &e){
	}
}

void TestExecMysqlQuery::tearDown() {
	try{
		m_connector->exec("DROP TABLE " + DB_TABLENAME);
	}catch(const mt::DbException &e){
	}
}

void TestExecMysqlQuery::testCreateTable() {
	createTable();
	mt::DbDataPtr tables_data = m_connector->exec("SHOW TABLES");
	CPPUNIT_ASSERT(tables_data);
	CPPUNIT_ASSERT_EQUAL(1, (int)tables_data->columnCount());
	CPPUNIT_ASSERT_EQUAL(1, (int)tables_data->rowCount());
	CPPUNIT_ASSERT_EQUAL(std::string("Tables_in_mtlib_test"), tables_data->getColumnName(0));
	CPPUNIT_ASSERT_EQUAL(DB_TABLENAME, tables_data->getColumn("Tables_in_mtlib_test").getString(0));
}

void TestExecMysqlQuery::testUpdateTable() {
	createTable();

	try{
		int32_t rows = m_connector->execCmd("UPDATE " + DB_TABLENAME + " set TestClass = '10' where RequestId = '1'");
		CPPUNIT_ASSERT_EQUAL(0, (int)rows);

		rows = m_connector->execCmd("INSERT INTO " + DB_TABLENAME + " VALUES ('TestExecMysqlQuery', 'testUpdateTable', 1)");
		CPPUNIT_ASSERT_EQUAL(1, (int)rows);

		rows = m_connector->execCmd("UPDATE " + DB_TABLENAME + " set TestClass = 'TestExecMysqlQuery' where RequestId = '1'");
		CPPUNIT_ASSERT_EQUAL(1, (int)rows);
	} catch(const mt::DbException &e){
		CPPUNIT_FAIL(e.message());
	}
}


void TestExecMysqlQuery::testInsertQuery() {
	createTable();
	mt::DbDataPtr insert_data = m_connector->exec("INSERT INTO " + DB_TABLENAME + " VALUES ('TestExecMysqlQuery', 'testInsertQuery', 1)");
	CPPUNIT_ASSERT(insert_data);
	CPPUNIT_ASSERT_EQUAL(0, (int)insert_data->columnCount());
	CPPUNIT_ASSERT_EQUAL(0, (int)insert_data->rowCount());

	mt::DbDataPtr data = m_connector->exec("SELECT * FROM " + DB_TABLENAME);
	CPPUNIT_ASSERT(data);
	CPPUNIT_ASSERT_EQUAL(3, (int)data->columnCount());
	CPPUNIT_ASSERT_EQUAL(1, (int)data->rowCount());
	CPPUNIT_ASSERT_EQUAL(std::string("TestExecMysqlQuery"), data->getColumn("TestClass").getString(0));
	CPPUNIT_ASSERT_EQUAL(std::string("testInsertQuery"), data->getColumn("TestMethod").getString(0));
	CPPUNIT_ASSERT_EQUAL(1, data->getColumn("RequestId").getInt(0));

	CPPUNIT_ASSERT_THROW(data->getColumn("NoSuchColumn"), mt::Exception);
}


