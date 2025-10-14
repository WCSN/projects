//============================================================================
// Name        : fibbo.cpp
// Author      : Wocson
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <math.h>

using namespace std;

long double int_pow(int64_t base, int64_t pw)
{
	long double res = (long double)base;
	long double ld_base = (long double)base;

if(pw == 0)
		res = 0;
	else if(pw > 0)
	{
		for(int64_t i = 2; i <= pw; i++)
			res *= ld_base;
	}
	else
	{
		for(int64_t i = 2; i <= (-1)*pw; i++)
			res *= ld_base;

		res = 1 / res;
	}

	return res;
}

int main(void)
{
 	long double m = 0;

 	cout << "Числа Фибоначчи:\n";

	m = int_pow(2,sizeof(int64_t)*8)-1;

	long double frst = 0;
	long double scnd = 1;
	long double thrt = 0;

	cout << "┌"; for(int i=0; i<=67;i++) cout << "─"; cout << "┐\n";  		//┌──────┐

	cout << "│";
	cout.width(3);	cout << right << "#"; 	cout.width(2); cout << right << "│";
	cout.width(12);	cout << right << "DEC"; cout.width(11); cout << right << "│";
	cout.width(11);	cout << right << "HEX"; cout.width(10); cout << right << "│";
	cout.width(14);	cout << right << "OCT"; cout.width(14); cout << right << "│\n";

	cout << "├"; for(int i=0; i<=67;i++) cout << "─"; cout << "┤\n";

	uint64_t n=0;

	do
	{	thrt = frst + scnd;
		frst = scnd;
		scnd = thrt;

		cout << "│"; cout.width(3); cout << right << dec << n + 1 << "│";
        cout.width(20); cout << right << dec << (uint64_t) thrt << "│";
        cout.width(18); cout << uppercase << right << hex << (uint64_t) thrt << "│";
        cout.width(24); cout << right << oct << (uint64_t) thrt << "│\n";
        n++;

	} while ( (frst + scnd) < (uint64_t)m-1 && (frst + scnd) > 0 );

	cout << "└"; for(int i=0; i <= 67; i++) cout << "─"; cout << "┘\n"; 	//└──────┘
	cout << endl;

	cout << "Целые числа:\n";

		cout << "┌"; for(int i=0; i<=67;i++) cout << "─"; cout << "┐\n";  		//┌──────┐

		cout << "│";
		cout.width(3);	cout << right << "#"; 	cout.width(2); cout << right << "│";
		cout.width(12);	cout << right << "DEC"; cout.width(11); cout << right << "│";
		cout.width(11);	cout << right << "HEX"; cout.width(10); cout << right << "│";
		cout.width(14);	cout << right << "OCT"; cout.width(14); cout << right << "│\n";

		cout << "├"; for(int i=0; i<=67;i++) cout << "─"; cout << "┤\n";

		n=0;

		do
		{
			cout << "│"; cout.width(3); cout << right << dec << n + 1 << "│";
	        cout.width(20); cout << right << dec << n << "│";
	        cout.width(18); cout << uppercase << right << hex << n << "│";
	        cout.width(24); cout << right << oct << n << "│\n";
	        n++;

		} while ( n <= 100 );

		cout << "└"; for(int i=0; i <= 67; i++) cout << "─"; cout << "┘\n"; 	//└──────┘
		cout << endl;


    return 0;
}
