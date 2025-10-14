#ifndef _HELPER_H_
#define _HELPER_H_

#include <iostream>
#include <string>
using namespace std;

#define MAXKEYS_E 001
#define MAXBUFFOUTSZ_E 002

#define SMLVER 001
#define LRGVER 002

#define ERROR_CMD -101
#define ERROR_SCKT -102

void  usage(void);
void  version(int);
char *version(void);

char *getdate(char *sdt,int size,char *format);
char *getdate(char *sdt,int size,int type=0);
time_t gettimesec(void);

int   ParceCmd(int,char **,char **);
bool  isopt(char);

int   getnKEYS(void);
char *getkeys(int i);

extern char conffilename[512];

//1m - intensity
//4m - underscore
//5m - blinking
//7m - reverse

//#define _COLOR_

#ifdef _COLOR_
#define CLSTD		"\033[0m" //all attibutes off
#define BLACK  		"\033[0;30m"
#define RED  		"\033[0;31m"
#define LTRED  		"\033[1;31m"
#define GREEN 		"\033[0;32m"
#define LTGREEN		"\033[1;32m"
#define YELLOW 		"\033[0;33m"
#define LTYELLOW 	"\033[1;33m"
#define BLUE 		"\033[0;34m"
#define LTBLUE 		"\033[1;34m"
#define PURPLE 		"\033[0;35m"
#define LTPURPLE 	"\033[1;35m"
#define CYAN 		"\033[0;36m"
#define LTCYAN 		"\033[1;36m"
#define GRAY 		"\033[0;37m"
#define WHITE 		"\033[1;37m"

#else

#define CLSTD		"" //all attibutes off

#define BLACK  		""
#define RED  		""
#define LTRED  		""
#define GREEN 		""
#define LTGREEN		""
#define YELLOW 		""
#define LTYELLOW 	""
#define BLUE 		""
#define LTBLUE 		""
#define PURPLE 		""
#define LTPURPLE 	""
#define CYAN 		""
#define LTCYAN 		""
#define GRAY 		""
#define WHITE 		""

#endif

#define ARRAY_COUNT(x) (sizeof(x)/sizeof(x[0]))

string getenvval(char **, char *);
bool is_file(string);

extern string home;
extern char date[256];
extern char *signalsmsg[];

#endif  /* _HELPER_H_ */

