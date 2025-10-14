/*
 * wsystem.cpp
 *
 * Created on: 07.06.2013
 * Author: wocson
 */

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm> 
#include <vector>
#include "wlsystem.h"

using namespace std;

/*
template< class RandomIt >
void random_shuffle( RandomIt first, RandomIt last )
{
    typename std::iterator_traits<RandomIt>::difference_type i, n;
    n = last - first;
    for (i = n-1; i > 0; --i) {
        using std::swap;
        swap(first[i], first[std::rand() % (i+1)]);
        // rand() % (i+1) isn't actually correct, because the generated number
        // is not uniformly distributed for most values of i. A correct implementation
        // will need to essentially reimplement C++11 std::uniform_int_distribution,
        // which is beyond the scope of this example.
    }
}
*/

int getch(void)
{
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	//ch = getchar();
	ch = 121; // 'y'
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}

vector<uint> randomm(uint minVal, uint maxVal, uint count, uint step, bool srt)
{
	static vector<uint> rgl;

	rgl.clear();

	if (count == 0)
		count = maxVal - minVal + 1;

	for (uint i = minVal; i <= maxVal; i += step)
		rgl.push_back(i);

	random_shuffle(rgl.begin(), rgl.end());

	rgl.erase(rgl.begin() + count, rgl.end());

	if (srt)
		sort(rgl.begin(), rgl.end());

	return rgl;
}

bool YesNo(const string& str)
{
	cout << str << "(y/n): ";
	cout.flush();

	if(getch()=='y')
		return true;

return false;
}
