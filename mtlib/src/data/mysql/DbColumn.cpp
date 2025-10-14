#include <mt/data/mysql/DbColumn.h>
#include <mt/utils/StrUtils.h>

namespace mt {

	DbColumn::DbColumn() {
	}

	DbColumn::DbColumn(const DB_COLUMN& column) : DB_COLUMN(column) {
	}

	DbColumn::~DbColumn() {
	}

	mt::ByteBuffer DbColumn::getBinary(int32_t row) const {
		std::string binStr = operator[](row);
		mt::ByteBuffer buf(binStr.length());
		buf.append((const uint8_t*)binStr.c_str(), binStr.length());
		return buf;
	};

	std::string DbColumn::identity(int32_t id) {
		if( id<0 )
			return "NULL";
		return mt::StrUtils::intToString(id);
	}

	std::string DbColumn::binary(char* buf, int32_t len) {
		if(!len)
			return "NULL";

		std::string binStr("0x");
		char byteChars[10];
		for(int32_t offset=0; offset<len; offset++){
			sprintf(byteChars,"00%X",(uint32_t)*(buf+offset));
			binStr+= byteChars + strlen(byteChars)-2;
		}
		return binStr;
	}

	std::string DbColumn::unsignedFloat(float f) {
		return unsignedDouble(f);
	}

	std::string DbColumn::unsignedDouble(double d) {
		if( d<0) return "NULL";
		char buf[32];
		sprintf(buf,"%lf",d);
		return buf;
	}

	std::string DbColumn::unsignedLong(long l) {
		if( l<0) return "NULL";
		char buf[32];
		sprintf(buf,"%ld",l);
		return buf;
	}

	std::string DbColumn::unsignedInt(uint32_t i) {
		return unsignedLong(i);
	}

	std::string DbColumn::mysqlString(const std::string& str) {
		if(str.empty())
			return "NULL";
		return "'" + str + "'";
	}

	std::string DbColumn::escapeString(const std::string& in) {
		std::string out(in); //make a copy to modify
		std::string::size_type last_pos = 0;
		while(std::string::npos!=( last_pos=out.find("\\",last_pos))) {
			out.replace(last_pos,1,"\\\\");	
			last_pos+=2;
		}
		last_pos = 0;
		while( std::string::npos!=( last_pos=out.find("'",last_pos))) {
			out.replace(last_pos,1,"\\'");	
			last_pos+=2;
		}
		return out;
	}

	std::string DbColumn::stringFromDate(time_t t, bool quoted) { 
		return (t == -1) ? "NULL" : (quoted ? "'" + dateToString(t) + "'" : dateToString(t));
	}

	std::string DbColumn::dateToString(time_t time){
		if(time==-1) return "";

		struct tm* tmtime = localtime(&time);
		if( !tmtime ){
			char err[64];
			sprintf(err,"INCORRECT TIME VALUE [%X].", (uint32_t)time);
			return err;
		}

		char buf[1024];
		strftime(buf,1024,"%Y-%m-%d %H:%M:%S", tmtime);
	
		return buf;
	}

	time_t DbColumn::dateFromString(const std::string& timeStr){
		if(timeStr=="NULL" || timeStr=="")
			return -1;
		time_t tmpt(0);
		tm* t = localtime(&tmpt);
		float tm_sec;
		int scanned = sscanf(timeStr.c_str(),"%d-%d-%d %d:%d:%f",&t->tm_year,&t->tm_mon,&t->tm_mday,&t->tm_hour,&t->tm_min,&tm_sec);
		t->tm_sec = (int)tm_sec;
		t->tm_year -= 1900;
		t->tm_mon--;
		if( scanned != 6)
			return -1;

		return mktime(t);
	}
}
