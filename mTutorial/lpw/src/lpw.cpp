/*
 * lpw.cpp
 *
 *  Created on: 13.02.2012
 *      Author: wocson
 */

#include "lpw.h"

using namespace std;

char *jobst[] =
	{
	"PENDING",		/* Job is waiting to be printed */
	"HELD",			/* Job is held for printing */
	"PROCESSING",	/* Job is currently printing */
	"STOPPED",		/* Job has been stopped */
	"CANCELED",		/* Job has been canceled */
	"ABORTED",		/* Job has aborted due to error */
	"COMPLETED"		/* Job has completed successfully */
	};


void usage(void)
{
	cout
	<<"Usage: lpw [OPTIONS]\n"
	<<" Show printer(s) info...\n"
	<<" -a\tall info\n"
	<<" -P\tname printer\n"
	<<" -j\t[job type] PENDING | HELD | PROCESSING | STOPPED | CANCELED | ABORTED | COMPLETED\n"
	<<" -h\tHelp\n"
	<< endl;
}


string JobStatus(const ipp_jstate_t state)
{
	return string(jobst[state - 3]);
}

//"%Y-%m-%d %H:%M:%S"       //2010-01-13 00:00:01
string ConvTime(const time_t tt, const std::string& format = "%Y%m%d-%H:%M:%S")
{
	struct tm *ts;
	char timeL[128];
	ts = localtime(&tt);
	strftime(timeL, 512, format.c_str(), ts);

	return std::string(timeL);
}


void Def_Info(void)
{
	cups_lang_t *langs;

	cout << "Default printer: " << cupsGetDefault() << endl;
	langs = cupsLangDefault ();

	for (; langs!=NULL; langs=langs->next )
		cout <<"Language: " << langs->language << endl;
}


void ShowPrinterJobs(char const* nameprinter,int jtype)
{
	cups_job_t *jobs, *job;
	int i;

	int num_jobs = cupsGetJobs(&jobs, nameprinter, 0, CUPS_WHICHJOBS_ALL);

	if (num_jobs)
	{
		for (i = num_jobs, job = jobs; i > 0; i--, job++)
		{
			if(jtype==job->state || jtype==0)
			cout
			/* The job ID */
			<< " " << job->id << "."
			/* Printer or class name */
			<< " " << job->dest
			/* Title/job name */
			<< " [" << job->title << "]"
			/* User the submitted the job */
			<< " " << job->user
			/* Document format */
			<< " " << job->format
			/* Job state */
			<< " [" << JobStatus(job->state) << "]"
			/* Size in kilobytes */
			<< " " << job->size << "kB"
			/* Priority (1-100) */
			<< " " << job->priority
			/* Time the job was completed */
			<< " [" << ConvTime(job->completed_time)
			/* Time the job was created */
			<< " " << ConvTime(job->creation_time)
			/* Time the job was processed */
			<< " " << ConvTime(job->processing_time) << "]" << endl;
		}
	}
	else
		cout << "    None" << endl;
}


void ShowPrintersList(void)
{
	int i = 0;

	cups_dest_t *dests, *dest;

	int num_dests = cupsGetDests(&dests);

	for (i = num_dests, dest = dests; i > 0; i--, dest++)
	{
		if (dest->instance)
			cout <<" " << dest->name << "/" << dest->instance << endl;
		else
			cout <<" " << dest->name << endl;
	}
}


int CheckJobType(char const *typejob)
{
	string jst(typejob);

	if(jst.compare("all")==0)
		return 0;
	else
	{
		for (int i = 0; i <= 6; i++ )
		{
			jst.empty();
			jst.assign(jobst[i]);
			if(jst.compare(typejob)==0)
				return i+3;
		}
	}
}

int lpw(char const* typejob)
{
	int i = 0;

	int jtype = CheckJobType(typejob);

	cups_dest_t *dests, *dest;

	int num_dests = cupsGetDests(&dests);

	for (i = num_dests, dest = dests; i > 0; i--, dest++)
	{
		if (dest->instance)
			cout << dest->name << "/" << dest->instance << ":" << endl;
		else
			cout << dest->name << ":" << endl;

		cout << " PPD: " << cupsGetPPD(dest->name) << endl << " Jobs:" << endl
				<< " ID, prnnm, [jbnm], user, frmt, [state], Size kB, Prio, [tcmp, tcr, tproc] (YYYYMMDD-HH:MM:SS)"
				<< endl;

		ShowPrinterJobs(dest->name,jtype);
		cout << "---------------------------------------------" << endl;
	}

	return (0);
}
