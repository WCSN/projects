/*
 * main.cpp
 *
 *  Created on: Feb 3, 2012
 *      Author: wocson
 */

#include "lpw.h"

using namespace std;

int main(int argc, char **argv, char **env)
{
	int32_t opt = -1;

    if (argc > 1)
	{
		while ((opt = getopt(argc, argv, "P:j:avh")) != -1)
		{
			switch (opt)
			{
			case 'P':
				if (0 == optarg)
					cout << "-P required name printer!" << endl;
				else
					ShowPrinterJobs(optarg);
				return 0;
			case 'a':
				cout << "lpw report:" << endl;
				ShowPrintersList();
				Def_Info();
				lpw();
				break;
			case 'j':
				cout << "Listing of job type: ";
				if (0 == optarg)
				{
					cout << "all" << endl;
					lpw();
				}
				else
				{
					cout << optarg << endl;
					lpw(optarg);
				}	
				break;
			case 'h':
				usage();
				break;
			default:
				break;
				//return 0;
			}
		}
	}
	else
	{
		cout << "lpw report:" << endl;
		ShowPrintersList();
		Def_Info();
	}

	return (0);
}
