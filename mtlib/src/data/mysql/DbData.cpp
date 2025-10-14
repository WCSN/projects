#include <mt/data/mysql/DbData.h>
#include <mt/common/Exception.h>

namespace mt {
	DbData::DbData() {
	}

	DbData::DbData(const DB_DATA& draw) : DB_DATA(draw) {
	}

	DbData::~DbData() {
	}

	DbColumn& DbData::getColumn(const std::string& name) {
		DB_DATA::iterator column_it = DB_DATA::find(name);
		if( column_it == DB_DATA::end()) {
			std::string columnList;
			for(DB_DATA::const_iterator it = DB_DATA::begin(); it != DB_DATA::end(); it++) {
				columnList += "[" + it->first + "]";
			}
			MT_EXCEPTION(mt::Exception, 
									 "DataResult does no contains column with name '" << name << "'. There is " << columnCount() << " columns: " << columnList);
		}
		return column_it->second;
	}

	DbColumn& DbData::operator[](const std::string& name ) {
		return DB_DATA::operator[](name);
	}

	std::string DbData::getColumnName(uint32_t columnNo) { 
		if(columnNo > columnCount()) 
			MT_EXCEPTION(mt::Exception, 
									 "Failed to get column's " << columnNo << " name. Out of bounds " << columnCount());
		DB_DATA::const_iterator cit = DB_DATA::begin();
		for(uint32_t cn=0;  cn < columnNo; cn++){
			cit++;
		}
		return cit->first;
	}

	const DbColumn& DbData::getColumn(uint32_t columnNo) { 
		if(columnNo > columnCount())
			MT_EXCEPTION(mt::Exception, "Failed to get column " << columnNo << ". Out of bounds " << columnCount());

		DB_DATA::const_iterator cit = DB_DATA::begin();
	
		for(uint32_t cn = 0; cn < columnNo; cn++){
			cit++;
		}
		return cit->second;
	}


}
