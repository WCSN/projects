/*
 * main.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: wocson
 */

#include "Thousand.h"
#include "wlsystem.h"

using namespace std;

void usage(void)
{
    cout << "thousand 0.0.5\n Game 1000.\n Usage: thousand -[opt]\nopt:\n\t -v\tversion\n\nHave fun!\n" << endl;
}

int main(int argc, char **argv)
{
	if(argc > 1)
	{
		usage();
		return 1;
	}
	else
	{
		ThousandApp thsapp;
		thsapp.Run();
	}

return 0;
}
