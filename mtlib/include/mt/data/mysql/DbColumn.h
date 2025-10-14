#ifndef __DATA__MYSQL__DB_COLUMN__H
#define __DATA__MYSQL__DB_COLUMN__H

#include <string>
#include <vector>

#include <mt/common/DynamicBuffer.h>

namespace mt {

	typedef std::vector<std::string> DB_COLUMN;

	/**
	 * @class DbColumn mt/data/mysql/DbColumn.h
	 * @brief DbColumn contain data from the DB data's column.  
	 */

	class DbColumn : public DB_COLUMN {
	public:
//@{
		DbColumn();
		DbColumn(const DB_COLUMN& column);
		virtual ~DbColumn();
//@}
//@{
		/** 
		 * Get value as long.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		long getLong(int32_t row) const;

		/** 
		 * Get value as int.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		int32_t getInt(int32_t row) const;

		/** 
		 * Get value as unsigned long.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		unsigned long getUnsignedLong(int32_t row) const;

		/** 
		 * Get value as unsigned int.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		uint32_t getUnsignedInt(int32_t row) const;

		/** 
		 * Get value as double.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		double getUnsignedDouble(int32_t row) const;

		/** 
		 * Get value as double.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		double getDouble(int32_t row) const;

		/** 
		 * Get value as float.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		float getUnsignedFloat(int32_t row) const;

		/** 
		 * Get value as float.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		float getFloat(int32_t row) const;

		/** 
		 * Get value as char string.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		const char* getCharString(int32_t row) const;

		/** 
		 * Get value as std::string.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		std::string getString(int32_t row) const;

		/** 
		 * Get value as time_t.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		time_t getTime(int32_t row) const;

		/** 
		 * Get value as long. If value is 'NULL' than will returned -1.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		long getIdentity(int32_t row) const;

		/** 
		 * Get value as bool.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		bool getBool(int32_t row) const;

		/** 
		 * Get binary value.
		 * 
		 * @param row Row's number.
		 * 
		 * @return Cell's value.
		 */
		mt::ByteBuffer getBinary(int32_t row) const;
		
		/** 
		 * Check for NULL.
		 * 
		 * @param row Row's number.
		 * 
		 * @return TRUE if value is 'NULL' or 'null'. Otherwise, will return FALSE.
		 */
		bool isNull(int32_t row) const;

		/** 
		 * Append new value to the column.
		 * 
		 * @param value appended value.
		 * 
		 * @return new size of column.
		 */
		int32_t append(const std::string& value );
	
		/** 
		 * Convert value to string at MYSQL format.
		 * 
		 * @param id Value to convert.
		 * 
		 * @return String at MYSQL format.
		 */
		static std::string identity(int32_t id);

		/** 
		 * Convert value to string at MYSQL format.
		 * 
		 * @param buf Value to convert.
		 * @param len length of buf.
		 * 
		 * @return String at MYSQL format.
		 */
		static std::string binary( char* buf, int32_t len);

		/** 
		 * Convert value to string at MYSQL format.
		 * 
		 * @param t Value to convert.
		 * @param quoted if TRUE than string will be quoted.
		 * 
		 * @return String at MYSQL format.
		 */
		static std::string stringFromDate(time_t t, bool quoted=true);

		/** 
		 * Convert value to string at MYSQL format.
		 * 
		 * @param f Value to convert.
		 * 
		 * @return String at MYSQL format.
		 */
		static std::string unsignedFloat( float f );

		/** 
		 * Convert value to string at MYSQL format.
		 * 
		 * @param d Value to convert.
		 * 
		 * @return String at MYSQL format.
		 */
		static std::string unsignedDouble( double d );

		/** 
		 * Convert value to string at MYSQL format.
		 * 
		 * @param l Value to convert.
		 * 
		 * @return String at MYSQL format.
		 */
		static std::string unsignedLong(long l);

		/** 
		 * Convert value to string at MYSQL format.
		 * 
		 * @param i Value to convert.
		 * 
		 * @return String at MYSQL format.
		 */
		static std::string unsignedInt( uint32_t i );

		/** 
		 * Convert value to string at MYSQL format.
		 * 
		 * @param str Value to convert.
		 * 
		 * @return String at MYSQL format.
		 */
		static std::string mysqlString(const std::string& str);
		
		/// If string "in" include symbol "\" than it will be escaped.
		static std::string escapeString(const std::string& in);
		//@}	
	private:
		static std::string dateToString(time_t time);
		static time_t dateFromString(const std::string& time);
	};

	inline long DbColumn::getLong(int32_t row) const { 
		return atol(operator[](row).c_str());
	}

	inline int32_t DbColumn::getInt(int32_t row) const {
		return (int32_t)getLong(row);
	}

	inline unsigned long DbColumn::getUnsignedLong(int32_t row) const { 
		return ("NULL"==operator[](row) || ""==operator[](row))?-1L:atol(operator[](row).c_str());
	}

	inline uint32_t DbColumn::getUnsignedInt(int32_t row) const {
		return (uint32_t)getUnsignedLong(row);
	}

	inline double DbColumn::getUnsignedDouble(int32_t row) const {
		return ("NULL"==operator[](row) || ""==operator[](row))?-1.0:atof(operator[](row).c_str());
	}

	inline double DbColumn::getDouble(int32_t row) const { 
		return atof(operator[](row).c_str());
	}

	inline float DbColumn::getUnsignedFloat(int32_t row) const {
		return (float)getUnsignedDouble(row);
	}

	inline float DbColumn::getFloat(int32_t row) const {
		return (float)getDouble(row);
	}

	inline const char* DbColumn::getCharString(int32_t row) const {
		return getString(row).c_str();
	}
 
	inline std::string DbColumn::getString(int32_t row) const {
		std::string v = operator[](row); 
		return (v=="NULL") ? std::string() : v;
	}

	inline time_t DbColumn::getTime(int32_t row) const { 
		return dateFromString(operator[](row));
	}
	
	inline long DbColumn::getIdentity(int32_t row) const {
		std::string v=operator[](row); 
		return ((v=="NULL"||v=="")?-1:atol(v.c_str()));
	}

	inline bool DbColumn::getBool(int32_t row) const {
		return ("0"!=operator[](row));
	}
 
	inline bool DbColumn::isNull(int32_t row) const { 
		return "NULL"==operator[](row) || "null"==operator[](row);
	}

	inline int32_t DbColumn::append(const std::string& value ){ 
		push_back(value); 
		return size();
	}
}

#endif /* __DATA__MYSQL__DB_COLUMN__H */
