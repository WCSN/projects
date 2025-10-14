#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
#include <time.h>
#include <syslog.h>
#include <sys/timeb.h>

#include <iostream>
#include <iomanip>

#include "helper.h"

char date[256];

char *signalsmsg[]={
"SIGHUP Hang up",                       					//	SIGHUP				1
"SIGINT Interrupt",                     					//	SIGINT				2
"SIGQUIT Quit",                         					//	SIGQUIT				3
"SIGILL Illegal instruction",           					//	SIGILL				4
"SIGTRAP Trace trap",                   					//	SIGTRAP				5
"SIGABRT Abort or SIGIOT IOT trap",							//	SIGABRT	SIGIOT		6
"SIGBUS BUS error",                     					//	SIGBUS				7
"SIGFPE Floating-point exception",      					//	SIGFPE				8
"SIGKILL Kill, unblockable",            					//	SIGKILL				9
"SIGUSR1 User-defined signal 1",        					//	SIGUSR1				10
"SIGSEGV Segmentation violation",       					//	SIGSEGV				11
"SIGUSR2 User-defined signal 2",        					//	SIGUSR2				12
"SIGPIPE Broken pipe",                  					//	SIGPIPE				13
"SIGALRM Alarm clock",                  					//	SIGALRM				14
"SIGTERM Termination",                  					//	SIGTERM				15
"SIGSTKFLT Stack fault",                					//	SIGSTKFLT			16
"SIGCHLD Child status has changed",     					//	SIGCLD | SIGCHLD	17
"SIGCONT Continue",                     					//	SIGCONT				18
"SIGSTOP Stop, unblockable",            					//	SIGSTOP				19
"SIGTSTP Keyboard stop",                					//	SIGTSTP				20
"SIGTTIN Background read from tty",     					//	SIGTTIN				21
"SIGTTOU Background write to tty",      					//	SIGTTOU				22
"SIGURG	Urgent condition on socket",    					//	SIGURG				23
"SIGXCPU CPU limit exceeded",           					//	SIGXCPU				24
"SIGXFSZ File size limit exceeded",     					//	SIGXFSZ				25
"SIGVTALRM Virtual alarm clock",        					//	SIGVTALRM			26
"SIGPROF Profiling alarm clock",        					//	SIGPROF				27
"SIGWINCH Window size change",          					//	SIGWINCH			28
"SIGPOLL | SIGIO Pollable event occurred I/O now possible", //	SIGPOLL | SIGIO		29
"SIGPWR	Power failure restart",         					//	SIGPWR				30
"SIGSYS	Bad system call"                					//	SIGSYS				31
};

char *usagetxt[]={
"\nusage: nfparser [options]\n",
"options:\n"
" -D\t\t- daemoniZed",
" -d\t\t- debug output info to stdout",
" -o\t\t- save nflow data to file",
" -s\t\t- add stats info data file",
" -c conf\t- other conffile",
" -w\t\t- out all warnings",
" -h\t\t- this help",
" -v\t\t- version",
0};

char defkeys[] = "Ddoshvcw";

char vers[128];

char *name = "nfparser v.";
char *ver="0.2 beta";
char *copyright = "Copyright (c) Mobitech (W.)";

char conffilename[512];
string home;

#define MAXKEYS 64
#define MAXBUFFOUTSZ 16384

#define MAXKEYS_E 001
#define MAXBUFFOUTSZ_E 002

char keys[MAXKEYS][2];

int nKEYS = 0;
int nBytes = 0;

void usage(void) //Usage
{
        version(LRGVER);
        for(int i=0; usagetxt[i]!=0; i++)
                printf("%s\n",usagetxt[i]);
}


void version(int var)
{
  printf("%s %s %s\n", name,ver,copyright);
}


char *version(void)
{
    sprintf(vers,"%s %s %s\n", name,ver,copyright);
    return vers;
}

/*
1  fprintf(stdout,"%s",getdate(date,2,"%a %b %d %Y %H:%M:%S"));
2  fprintf(stdout,"%s",getdate(date,2,"%A %d %B %Y %H:%M:%S"));
3  fprintf(stdout,"%s",getdate(date,2,"%Y.%m.%d %H:%M:%S"));    	//2010.01.13 00:00:01
4  fprintf(stdout,"%s",getdate(date,2,"%Y%m%d %H%M%S" ));		//20100113 000001
5  fprintf(stdout,"%s",getdate(date,2,"%d.%m.%Y %H:%M:%S"));
6  fprintf(stdout,"%s",getdate(date,2,"%H:%M:%S"));
*/

#define YYYY_MM_DD_HH_MM_SS  3
#define YYYYMMDD_HHMMSS      4


char *getdate(char *sdt,int size,char *format)
{
   time_t tt;
   struct tm *ts;

   tzset();
   time( &tt );            /* Get datetime in seconds */
   ts = localtime( &tt );  /* Convert datetime to struct */

   strftime( sdt, 512, format, ts );

   return sdt;
}

time_t gettimesec(void)
{
	time_t tt;
	time(&tt); /* Get datetime in seconds */
return tt;
}

char *getdate(char *sdt,int size,int type)
{
   time_t tt;
   struct tm *ts;

   tzset();
   time(&tt);            /* Get datetime in seconds */
   ts = localtime( &tt );  /* Convert datetime to struct */

   switch(type)
   {
	   case 3:
		   strftime( sdt, 512, "%Y.%m.%d %H:%M:%S", ts );
		   break;
	   case 4:
		   strftime( sdt, 512, "%Y%m%d %H%M%S", ts );
		   break;
	   default:
		   strftime( sdt, 512, "%Y.%m.%d %H:%M:%S", ts );
		   break;
   }
   return sdt;
}

string getenvval(char **env, char *val)
{
	static string sval0;

	for(int i=0; env[i]!=0; i++)
	{
//		cout << env[i] << "\n";
		sval0.assign(env[i]);
		if(sval0.find(val) == 0)
			return sval0.substr(sval0.find("=")+1).c_str();
	}
	return "none";
}

int ParceCmd(int argc,char **argv,char **env)
{
	int ik=0;
	int ij=0;

    for(int i=0; i < argc-1; i++)
    {
    	if(argv[i+1][0]=='-')
        {
        	for(int in=0; argv[i+1][in+1]!=0; ik++,in++)
        	{
        		keys[ik][0] = argv[i+1][in+1];

        		nKEYS++;

        		if(nKEYS > MAXKEYS)
        		{
        			std::cerr << "Error:<< MAXKEYS << ParceCmd: Check options settings.\n";
        			return MAXKEYS_E;
        		}
        		else
        		{
        			bool ch_ok = false;

        			for(int idf=0; defkeys[idf]; idf++)
        			{
        				if(keys[ik][0] == defkeys[idf])
        				{
        					ch_ok=true;
        					break;
        				}
        			}
        			if(!ch_ok)
        			{
        				std::cerr << "Unknown options: " << keys[ik][0] << ".\n";
        				return ERROR_CMD;
        			}
        		}

        		if(keys[ik][0]=='c')
        		{
        		  	strcpy(conffilename,argv[i+2]);
        		}
        	}
        }
        else
        {
        	nBytes += (int)strlen(argv[i+1]);

        	if(nBytes >= MAXBUFFOUTSZ)
        	{
        		fprintf(stdout,"Error: %d. ParceCmd: Options buffer overload. (%d)\n",MAXBUFFOUTSZ,nBytes);
        		return MAXBUFFOUTSZ_E;
        	}
          ij++;
        }
    }

home = getenvval(env,"HOME"); //get home dir.

return 0;
}

int getnKEYS(void)
{
	return nKEYS;
}

char *getkeys(int i)
{
	static char ch[4];
	ch[0]=' ';
	ch[1]='-';
	ch[2]=keys[i][0];
	ch[3]=0;

	return ch;
}

bool isopt(char key)
{
 for(int i=0; i < nKEYS; i++)
         if( keys[i][0] == key )
                 return 1;

 return 0;
}

bool is_file(string fname)
{
	return (access(fname.c_str(),0)!=-1);
}
