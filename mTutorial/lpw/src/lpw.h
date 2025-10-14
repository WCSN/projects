/*
 * lpw.h
 *
 *  Created on: 13.02.2012
 *      Author: wocson
 */

#ifndef LPW_H_
#define LPW_H_

#include <iostream>
#include <string>
#include <cups/cups.h>
#include <cups/ppd.h>


std::string JobStatus(ipp_jstate_t state);
std::string ConvTime(time_t tt, const std::string& format);
int lpw(char const* typejob = "all");

void usage(void);
void Def_Info(void);
void ShowPrinterJobs(char const* nameprinter,int jtype = 0);
void ShowPrintersList(void);

#endif /* LPW_H_ */
