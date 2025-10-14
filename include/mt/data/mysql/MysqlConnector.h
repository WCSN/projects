/*
 * MysqlConnector.h
 *
 *  Created on: 21.01.2010
 *      Author: hakamasy
 */

#ifndef MYSQLCONNECTOR_H_
#define MYSQLCONNECTOR_H_

#include <mysql/mysql.h>
#include <string>

#include <mt/logger/Logger.h>
#include <mt/common/XmlConfig.h>
#include <mt/data/mysql/DbData.h>

namespace mt {

	/**
	 * \class MysqlConnector
	 *
	 * \brief Simple Connector to MYSQL
	 */

	class MysqlConnector : public Logable{

		MYSQL* m_mysqlConnection;

		std::string m_dbHost;
		std::string m_login;
		std::string m_password;
		std::string m_dbName;

	public:
		//@{
		MysqlConnector(const std::string& dbHost, const std::string& login,
									 const std::string& password, const std::string &dbName);
		MysqlConnector(const XmlConfigPtr& cfg);
		virtual ~MysqlConnector();
		//@}
		//@{
		void setDbName(std::string &dbName) {
			m_dbName = dbName;
		}

		void setLogger(const LoggerPtr &logger) {
			m_logger = logger;
		}

		/**
		 * Execute SQL query and return command-execute status. (Used for queries, which doesn't return data, e.g. INSERT, DROP, UPDATE etc.)
		 *
		 * @param query sql query.
		 * @return count of affected rows.
		 */
		int32_t execCmd(const std::string& query);

		/**
		 * Execute SQL query and return result.
		 *
		 * @param query sql query.
		 * @return TODO return SmartPointer to data struct.
		 */
		virtual DbDataPtr exec(const std::string& query);

		/**
		 * Initialize database connection.
		 * @param if useDatabase = true - MysqlConnector will use <DbName> database as default.
		 * if useDatabase = false, no default database will be setted. You should exec "Use database <dbname>" query at first.
		 * @return 0 if success and -1 if error occured.
		 */
		int32_t init(bool useDatabase = true);
		//@}

	private:
		enum {
			LOGGED_CMD_LEN = 1024
		};
		DbDataPtr createDbData(MYSQL_RES* result);
	};

	typedef mt::SmartPtr<MysqlConnector> MysqlConnectorPtr;
}

#endif /* MYSQLCONNECTOR_H_ */
