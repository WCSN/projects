#ifndef __DATA__MYSQL__DB_DATA__H
#define __DATA__MYSQL__DB_DATA__H

#include <string>
#include <map>

#include <mt/data/mysql/DbColumn.h>
#include <mt/common/SmartPtr.h>

namespace mt {


	typedef enum { DB_REJECT=-2, DB_ERROR=-1, DB_OK=0 } DbResult;

	typedef std::map<std::string, DbColumn> DB_DATA;

	/**
	 * @class DbData mt/data/mysql/DbData.h
	 * @brief DbData contain returned data from DB.
	 */

	class DbData : private DB_DATA {

	public:
		DbData();
		DbData(const DB_DATA& draw);
		virtual ~DbData();

		/**
		 * Check data.
		 *
		 * @return TRUE if no data.
		 */
		bool isEmpty() const;

		/// Get row count of the data.
		size_t rowCount()const;

		/// Get column count of the data.
		size_t columnCount() const;

		/**
		 * Get column by name from data.
		 *
		 * @param name Column's name.
		 * @trows mt::Exception if no column with such name.
		 *
		 * @return Reference to column.
		 */
		DbColumn& getColumn(const std::string& name);

		/**
		 * Get column by name from data.
		 * If no such column it will be inserted to date.
		 *
		 * @param name Column's name.
		 *
		 * @return Reference to column.
		 */
		DbColumn& operator[](const std::string& name);

		/**
		 * Get column's name by it's number.
		 *
		 * @param columnNo Number of the column.
		 *
		 * @return Column's name.
		 */
		std::string getColumnName(uint32_t columnNo);

		/**
		 * Get column by number from data.
		 *
		 * @param columnNo Number of the column.
		 *
		 * @return constant reference to column.
		 */
		const DbColumn& getColumn(uint32_t columnNo);
	};

	typedef mt::SmartPtr<DbData> DbDataPtr;

	inline bool DbData::isEmpty() const {
		return 0==DB_DATA::size() || 0==(*DB_DATA::begin()).second.size();
	}

	inline size_t DbData::rowCount() const {
		return 0==DB_DATA::size() ? 0 :(*DB_DATA::begin()).second.size();
	}

	inline size_t DbData::columnCount() const {
		return DB_DATA::size();
	}

}
#endif /* __DATA__MYSQL__DB_DATA__H */
