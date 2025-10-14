/*
 * help.cpp
 *
 *  Created on: Aug 22, 2014
 *      Author: wocson
 */

#include "rmssapp.h"

void help(void)
{
	cout << "rmss " << version() << " - remove extra spaces and tabs at the ends of lines in text files.\n"
 << "Update: correct line zero del\n"
 << "Usage:\n\trmss [-option] source_file [target_file]\n"
 << "\toptions:\n"
 << "\t-d delete all space lines\n"
 << "\t-h help\n"
 << "\t-v version"
 << endl;
}

const char *version(void)
{
    return "1.7.1";
}