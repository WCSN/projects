/*
 * main.cpp
 *
 *  Created on: 06.12.2012
 *      Author: wocson
 */


#include <iostream>
#include "n_df.h"

using namespace std;


int main(int argc, char *argv[])
{

	if (argc < 2)
	{
		cout << "Usage << n_df DEVICE0 ..... DEVICEX\n" << endl;
		return (2);
	}
	else
	{
		n_df(argc, argv);
		cout << "------------------"<< endl;

		char *asdf[] = {"n_df","-Th"};

	//	diskfree(2,asdf);
	}

return 0;
}


