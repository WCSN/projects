/*
 * main.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: wocson
 */

#include "iostream"

using namespace std;

void f3xp1(void)
{
    unsigned int val;
    unsigned int count = 0;    
    
    cout << "init val= "; cin >> val;
    
    do 
    { 
        count++;    
        
        cout << count << ". " << val;
        
        if( val % 2 )
        {
            val = val * 3 + 1; 
        }
        else
        {
            val = val / 2;
        }
        
        cout << " ( " << val << " )" << endl;
    
    } while ( val != 1 );
    
    cout << "All steps: " << count << endl;
    
}


int main(int argc, char **argv)
{

    f3xp1();


    return 0;
}
