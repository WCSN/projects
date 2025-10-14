/*
 * main.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: wocson
 */

#include "unistd.h"
#include "rmssapp.h"


int main(int argc, char **argv)
{
	string source_file;
	string dest_file;
	bool dline = false;

	if (argc == 1) { help(); return 0; }

    int opt, iopt=0;

    while ((opt = getopt (argc, argv, "dvh")) != -1)
    {
        switch (opt)
        {
            case 'd':
                dline = true;
                if (argc == 2) 
                {
                    cout << "Error: Source file not defined" << endl;                                                      //      |
                    help();
                    return -3;
                }
                break;
            case 'v':
                cout << version() << endl;
                return 1;
            case 'h':
                help();
                return 2;
            default:
                cout << "Unknown option..." << endl;
                return -1;
        }
        ++iopt;
        // в op сохраняем оператор если надо
        // op = optarg[0]; // if have option argv
    }

    source_file.append(argv[1+iopt]);

    if(argc == (2+iopt)) 
        dest_file = source_file;
    else if(argc == (3+iopt)) 
        dest_file.append(argv[2+iopt]);
    else
    {
        cout << "Error: set argument / options";
        help();
        return -4;
    }

    Rmss rmss(source_file, dest_file, dline);

return rmss.Run();
}
