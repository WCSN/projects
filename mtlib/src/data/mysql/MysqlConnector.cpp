/*
 * MysqlConnector.cpp
 *
 *  Created on: 21.01.2010
 *      Author: hakamasy
 */

#include "mt/data/mysql/MysqlConnector.h"

namespace mt {

MysqlConnector::~MysqlConnector() {

	if (m_mysqlConnection != NULL)
		mysql_close(m_mysqlConnection);

}

MysqlConnector::MysqlConnector(const std::string& dbHost,
		const std::string& login, const std::string& password,
		const std::string &dbName) :
	m_mysqlConnection(NULL), m_dbHost(dbHost), m_login(login), m_password(
			password), m_dbName(dbName) {
	MT_LOGINIT(MysqlConnector);
}

MysqlConnector::MysqlConnector(const XmlConfigPtr& cfg) :
	m_mysqlConnection(NULL) {
	MT_LOGINIT(MysqlConnector);
	m_login = cfg->getMandatory("user");
	m_dbHost = cfg->getMandatory("host");
	m_password = cfg->get("password");
	m_dbName = cfg->getMandatory("database");
}

int32_t MysqlConnector::init(bool useDatabase ) {

	m_mysqlConnection = mysql_init(NULL);
	MT_LOGDEBUG("MySQL connect to " << m_dbHost << " with login '" << m_login << "' and pass: '" << m_password << "'. Database '" << m_dbName << "'");

	const char *dbName = NULL;
	if(useDatabase){
		dbName = m_dbName.c_str();
	}

	if (!mysql_real_connect(m_mysqlConnection, m_dbHost.c_str(),
			m_login.c_str(), m_password.c_str(), dbName, 0, NULL, 0)) {
		MT_LOGERROR(mysql_error(m_mysqlConnection));
		return -1;
	}

	if (mysql_query(m_mysqlConnection, "SET NAMES 'utf8';")) {
		return -1;
	}
	return 0;

}


int32_t MysqlConnector::execCmd(const std::string& query) {

	int32_t rez = mysql_query(m_mysqlConnection, query.c_str());
	if (rez) {
		MT_LOGERROR("cannot perform the query '" << query << "' THe reason is: " << mysql_error(m_mysqlConnection) << "errno: " << mysql_errno(m_mysqlConnection));
		uint32_t retError = mysql_errno(m_mysqlConnection);
		switch (retError) {

		case 2006: //Error: 2006  (CR_SERVER_GONE_ERROR) Message: MySQL server has gone away
			throw mt::ServerGoneDbException();

		case 2051: //Error: 2006  (CR_SERVER_GONE_ERROR) Message: MySQL server has gone away
			throw mt::NoDataDbException();

		case 2053: //Error: 2006  (CR_SERVER_GONE_ERROR) Message: MySQL server has gone away
			throw mt::NoResultSetDbException();

		case 1146:
			throw mt::NoSuchTableDbException();

		case 1007:
			throw mt::DbExistDbException();

		default:
			throw mt::DbException(mysql_error(m_mysqlConnection), retError);

		}

	}

	//query success
	return mysql_affected_rows(m_mysqlConnection);;
}

DbDataPtr MysqlConnector::exec(const std::string& query) {
	std::string logged_query = (query.length() <= LOGGED_CMD_LEN) ? query
			: query.substr(0, LOGGED_CMD_LEN) + "...";
	MT_LOGTRACE("sql query: " << logged_query);

	int execRet = mysql_query(m_mysqlConnection, query.c_str());

	if (execRet) {

		unsigned int retError = mysql_errno(m_mysqlConnection);
		switch (retError) {

		case 2006: //Error: 2006  (CR_SERVER_GONE_ERROR) Message: MySQL server has gone away
			throw mt::ServerGoneDbException();

		case 2051: //Error: 2006  (CR_SERVER_GONE_ERROR) Message: MySQL server has gone away
			throw mt::NoDataDbException();

		case 2053: //Error: 2006  (CR_SERVER_GONE_ERROR) Message: MySQL server has gone away
			throw mt::NoResultSetDbException();

		case 1146:
			throw mt::NoSuchTableDbException();

		default:
			std::string errorIs = "uncatched error. ";
			errorIs += mysql_error(m_mysqlConnection);

//			if (retError)
				throw mt::DbException(errorIs, retError);

//			throw mt::DbException("unknown error", execRet);

		}
	}
	MYSQL_RES *res = mysql_store_result(m_mysqlConnection);
	if (res) {
		return createDbData(res);
	} else if (mysql_errno(m_mysqlConnection)) {
		MT_LOGERROR("MySQL error: cannot get the results of query '" << logged_query <<"'. The reason is: " << mysql_error(m_mysqlConnection));
		return DbDataPtr();
	}
	return new DbData();

}
DbDataPtr MysqlConnector::createDbData(MYSQL_RES* result) {
	DbDataPtr rslt = new DbData;
	uint32_t num_fields = mysql_num_fields(result);
	MYSQL_FIELD *fields;
	fields = mysql_fetch_fields(result);

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result))) {
		unsigned long* flenths = mysql_fetch_lengths(result);
		for (uint32_t fldn = 0; fldn < num_fields; fldn++) {
			std::string fname = fields[fldn].name;
			std::string value = "UNKNOWN";
			unsigned long flength = flenths[fldn];

			//process the value of the fields
			switch (fields[fldn].type) {
			//printable cases
			case FIELD_TYPE_TINY: // 	TINYINT field
			case FIELD_TYPE_SHORT: // 	SMALLINT field
			case FIELD_TYPE_LONG: // 	INTEGER field
			case FIELD_TYPE_INT24: // 	MEDIUMINT field
			case FIELD_TYPE_LONGLONG: // 	BIGINT field
			case FIELD_TYPE_DECIMAL: // 	DECIMAL or NUMERIC field
			case FIELD_TYPE_FLOAT: // 	FLOAT field
			case FIELD_TYPE_DOUBLE: // 	DOUBLE or REAL field
			case FIELD_TYPE_TIMESTAMP: // 	TIMESTAMP field
			case FIELD_TYPE_DATE: // 	DATE field
			case FIELD_TYPE_TIME: // 	TIME field
			case FIELD_TYPE_DATETIME: // 	DATETIME field
			case FIELD_TYPE_YEAR: // 	YEAR field
			case FIELD_TYPE_STRING: // 	CHAR field
			case FIELD_TYPE_VAR_STRING: // 	VARCHAR field
			case FIELD_TYPE_BLOB: // 	BLOB or TEXT field (use max_length to determine the maximum length)
			case FIELD_TYPE_SET: // 	SET field
			case FIELD_TYPE_ENUM: // 	ENUM field
			case FIELD_TYPE_NULL: // 	NULL-type field
				if (row[fldn] && flength) {
					value = std::string(row[fldn], flength);
				} else {
					value = "NULL";
				}
				break;
			default:
				MT_LOGERROR("Unknown type of field '" << value << "' at mysql query")
				;
				continue;
			}
			(rslt->operator[](fname)).append(value);
		}
	}
	mysql_free_result(result);
	return rslt;
}
}
