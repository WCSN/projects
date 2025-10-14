#ifndef _COMMON_DB_CONNECTION_
#define _COMMON_DB_CONNECTION_

#include "MT_common.h"
#include "MT_config.h"
#include "MT_shared.h"
#include <MT_string.h>
#include <vector>
#include <time.h>

#ifdef WIN32
	#define vsnprintf(a,b,c,d) _vsnprintf(a,b,c,d)
	#pragma warning( disable : 4786 4503 )
#endif

#define L_CSTRING 			32
#define MAX_CHAR_BUF	1024

time_t dateFromString(const MT_string& time);
MT_string MT_stringFromDate(time_t time);

typedef enum { DB_REJECT=-2, DB_ERROR=-1, DB_OK=0 } DB_RESULT;
typedef vector<MT_string> __DB_COLUMN;

class DB_COLUMN: private __DB_COLUMN
{
public:
	DB_COLUMN():__DB_COLUMN(){};
	DB_COLUMN(__DB_COLUMN& rowData):__DB_COLUMN(rowData){}; 
	
	virtual long getLong( int row )const{ return atol(operator[](row).c_str());};
	virtual int getInt( int row ) const{return (int)getLong(row);}
	virtual long getUnsignedLong( int row ) const{ return ("NULL" == operator[](row) || operator[](row) == "")?-1L:atol(operator[](row).c_str());};
	virtual int getUnsignedInt( int row )const {return (int)getUnsignedLong(row);}
	virtual double getUnsignedDouble( int row ) const{ return (operator[](row) == "NULL" || operator[](row) == "")?-1.0:atof(operator[](row).c_str());};
	virtual double getDouble( int row ) const{ return atof(operator[](row).c_str());};
	virtual float getUnsignedFloat( int row ) const{return (float)getUnsignedDouble(row);};
	virtual float getFloat( int row ) const{return (float)getDouble(row);};
	virtual const char* getCharString(int row) const{return getString(row).c_str();}; 
	virtual MT_string getString( int row ) const{MT_string v=operator[](row); return (v=="NULL"?MT_string(""):v);};
	virtual time_t getTime( int row ) const{ return dateFromString(operator[](row));};
	virtual long getIdentity( int row ) const{MT_string v=operator[](row); return ((v=="NULL"||v=="")?-1:atol(v.c_str()));};
	virtual bool getBool(int row) const{return (operator[](row) != "0");} 
	virtual MT_memory<unsigned char> getBinary(int row) const;
	virtual bool isNull(int row) const{ return operator[](row) == "NULL" || operator[](row) == "null";}
	
	static MT_string Identity(int id);
	static MT_string Binary( char* buf, int len);
	static MT_string MT_stringFromDate(time_t t, bool quoted=true, int type=121) { 
		if(t == -1)
			return "NULL";
		
		if(quoted)
			return "'" + ::MT_stringFromDate(t) + "'";
		else
			return ::MT_stringFromDate(t);
	}
	static MT_string UnsignedFloat( float f ){return UnsignedDouble(f);};
	static MT_string UnsignedDouble( double d );
	static MT_string UnsignedLong( long l );
	static MT_string UnsignedInt( int i ){return UnsignedLong(i);};
	static MT_string escapeString(const MT_string& _in);


	virtual int append( MT_string value ){ push_back( value ); return size();};

	friend class DB_DATA;
	
	virtual ~DB_COLUMN(){};
};

extern void MT_exception(const char* fromat,...);

typedef MT_map<MT_string, DB_COLUMN> __DB_DATA;
class DB_DATA: protected __DB_DATA
{
public:
	DB_DATA():__DB_DATA(){};
	DB_DATA( __DB_DATA& draw ):__DB_DATA(draw){};
	virtual ~DB_DATA(){};

	bool isEmpty(){ return 0==__DB_DATA::size() || 0==(*__DB_DATA::begin()).second.size();};
	int rowCount(){ return 0==__DB_DATA::size() ? 0 :(*__DB_DATA::begin()).second.size();};
	int columnCount(){ return __DB_DATA::size();};
	virtual DB_COLUMN& getColumn(const MT_string& name); 
	virtual DB_COLUMN& operator[](const MT_string& name ){ return __DB_DATA::operator[](name);};
	
	const MT_string& getColumnName(unsigned int columnNo){ 
		if(__DB_DATA::size()<columnNo) 
			MT_exception("Failed to get column %d NAME. Out of bounds %d",columnNo,__DB_DATA::size());
			__DB_DATA::const_iterator cit = __DB_DATA::begin();
			for(unsigned int cn=0; cn<columnNo; cn++){
				cit++;
			}
			return cit->first;
	};

	const DB_COLUMN& getColumn(unsigned int columnNo){ 
		if(__DB_DATA::size()<columnNo) 
			MT_exception("Failed to get column %d. Out of bounds %d",columnNo,__DB_DATA::size());
			__DB_DATA::const_iterator cit = __DB_DATA::begin();
			
			for(unsigned int cn=0; cn<columnNo; cn++){
				cit++;
			}
			return cit->second;
	};
};

typedef MT_smart_ptr<DB_DATA> DB_DATA_PTR;

inline int* IDPTR_OR_NULL(int& id){return id==-1?0:&id;};

class DBConnection: public MT_reso
{
protected:

    MT_string  server;
    MT_string  database;
    MT_string  login;
    MT_string  password;

    bool	connected;
    bool 	initialized;

public:

		DBConnection(const Config &c, const MT_string& section): MT_reso( c, section )
		{
			connected = false;
	 		initialized = false;
			
			try
			{
	 			server 	= c.Get( section, "DBServer");
	 			login 	= c.Get( section, "Login" 	);
	 			password= c.Get( section, "Password" 	);
	 			database= c.Get( section, "Database");
			}
			catch( MT_exception_t E )
			{
				MT_exception( "Can't initialize DBConnection.[%s]", E.what().c_str());
			};
		};

    virtual ~DBConnection(){};

		virtual void open() = 0;
		virtual void close() = 0;
		virtual void reconnect() = 0;

		// This method sends a language command to the server. It expects no
		// rows or parameters to be returned from the server.
		virtual MT_EXTERNAL_SYMBOL DB_RESULT exec_cmd( MT_string Command, unsigned int *count = NULL ) = 0;
		virtual MT_EXTERNAL_SYMBOL DB_RESULT exec_cmd( const char* Command,... );
		
		// Method returns quantity of rows returned for <Command>,
		// and fills map with key as column name and value as vector of fields.
		virtual MT_EXTERNAL_SYMBOL DB_DATA_PTR get_results( MT_string Command ) = 0;
		virtual MT_EXTERNAL_SYMBOL DB_DATA_PTR get_results( const char* Command,...);
		//the method get statement and parameters to bind 
		virtual MT_EXTERNAL_SYMBOL DB_DATA_PTR get_stmnt_results( const char* prepStatement,...)=0;
};

#endif
