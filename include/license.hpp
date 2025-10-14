/***
 *
 * license.hpp
 *
 * Work with license keys
 *
 ***/
#include <map>
#include <MT_string.h>

#ifndef LICENSE_H_INCLUDED
#define LICENSE_H_INCLUDED

using namespace std;

typedef map<MT_string, MT_string> KeyMap;

//==============================================================================
class License {
private:
	KeyMap keys;
	// expiration date as 0xYYYYMMDD
	time_t exp_date;
	int version;
	MT_string module;
public:
/**
 * Read license data from a file 
**/
	License( const char* fileName );	
	bool isExpired(time_t date) const ;
	const MT_string* getStrValue( const MT_string& key ) const;
	int getVersion() const { return version;}
	MT_string getModule() const {return module;}
};

//==============================================================================
#endif
