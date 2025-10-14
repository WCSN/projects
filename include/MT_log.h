#ifndef MT_LOG_H
#define MT_LOG_H

#include <time.h>
#include <stdio.h>
#include <stdarg.h>

#include <exception>
#include <MT_string.h>
#include <strstream>

#include <MT_common.h>
#include <MT_smart_ptr.h>

class ChangingLog;

#ifdef WIN32
	using namespace std;
	#pragma warning( disable : 4786 4503 )
#endif

typedef enum
  {
  	MT_STAT=0,
  	MT_ERROR=1,
  	MT_WARNING=2,
  	MT_INFO=3,
  	MT_INFO_2=4,
  	MT_TRACE=5,
  	MT_TRACE_2=6,
  	MT_TRACE_3=7,
  	MT_DEBUG=8,
  	MT_DEBUG_2=9,
  	MT_DEBUG_3=10,
  } MT_errlevel;

extern char* CURRENT_LOG_LEVEL;
MT_EXTERNAL_SYMBOL char* get_errormask(void);
MT_EXTERNAL_SYMBOL void set_errorlevel(MT_errlevel a);
MT_EXTERNAL_SYMBOL void set_errorlevel(const char* a);

#define LOG_ENABLED( level ) get_errormask()[(int)level]=='1'
#define IF_LOG_ENABLED( level ) if(get_errormask()[(int)level]=='1')

extern FILE* LOG_FILE_FD;

struct LogCacheData
{
	LogCacheData();
	LogCacheData(const time_t& t);

	time_t time;
	int count;
};

MT_EXTERNAL_SYMBOL FILE* get_log_file_fd(void);
MT_EXTERNAL_SYMBOL void set_log_file_fd(FILE* fl);
MT_EXTERNAL_SYMBOL void set_log_file(MT_smart_ptr<ChangingLog> log);
MT_EXTERNAL_SYMBOL void set_log_cache(std::map<MT_string, LogCacheData>* cache);
MT_EXTERNAL_SYMBOL void drop_cache();
MT_EXTERNAL_SYMBOL MT_string formatTime(int mon, int day, int year, int hour, int min, int sec, int msec = -1);

MT_EXTERNAL_SYMBOL bool needOutLog(MT_string& log);

void MT_EXTERNAL_SYMBOL MT_exception(const char* Message,...);
void MT_EXTERNAL_SYMBOL MT_print2log(MT_errlevel level, strstream& s, bool cache);


class MT_exception_t
{
public:
	MT_exception_t(const MT_string& Message) : Msg(Message){};
	~MT_exception_t(){};
	MT_EXTERNAL_SYMBOL const MT_string& what (){return Msg;};
private:
	MT_string Msg;
};

MT_string MT_binaryView( unsigned char* data, unsigned int len);

void MT_EXTERNAL_SYMBOL MT_exception(const char* cpzMessage, va_list& ap);
void MT_EXTERNAL_SYMBOL MT_exception(const char* Message,...);

void MT_EXTERNAL_SYMBOL MT_exception_assert(bool condition, const char* Message,...);

MT_EXTERNAL_SYMBOL const char* get_log_message(MT_errlevel a);

/********************************************************
*   MT_log - print message to log if CURRENT_LOG_LEVEL variable greater or equal to first parameter
*   parameters are:
*        
*  level            - value of log level
* expression - stream expression should be printed to log
*
* Example of usage:   MT_log(MT_DEBUG, "Current value is:" << 5 << "big and " << getval() << "small chickens");
*/

#define MT_log(level,expression) {char* str=get_errormask(); \
if(str[(int)level]=='1')\
 {std::strstream s; \
 s <<  expression ; \
 IF_LOG_ENABLED( MT_DEBUG ) s << " ||"<<__FILE__<<":"<<__LINE__;\
 s <<'\0' ; \
 MT_print2log(level,s, false);												\
 }}

#define MT_log_cache(level,expression) {char* str=get_errormask(); \
if(str[(int)level]=='1')\
 {std::strstream s; \
 s <<  expression ; \
 IF_LOG_ENABLED( MT_DEBUG ) s << " ||"<<__FILE__<<":"<<__LINE__;\
 s <<'\0' ; \
 MT_print2log(level,s, true);												\
 }}

typedef MT_map<int,MT_string> MT_threadNamesOFPID;

class MT_autoproc_reg {
public:
	MT_autoproc_reg(MT_string pname);
	~MT_autoproc_reg();
};
//macros to print copyright information about product 'product' const char* to 'out' FILE*
#define WRITE_COPYRIGHT( product, out ) {\
	fprintf(out, "==============================================================\n\r\
  %s\n\r\
  \xA9 Mobile Informative Technologies, Ltd. (Mobitech) 2003-2007\n\r\
  e-mail: info@mobitechnologies.com\n\r\
  www: http://www.mobitechnologies.com\n\r\
  \n\r\
  Compiled: %s %s\n\r\
==============================================================\n\r",product,__DATE__,__TIME__);\
};

	 
#endif

