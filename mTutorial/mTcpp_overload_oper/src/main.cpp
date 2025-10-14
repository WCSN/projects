/*
 * main.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: wocson
 */

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;


class DT
{
private:
      int X;
      int Y;
    float Pce;
    string Sinfo;
    
public:
    DT(int,int,float,string);
    void setsinfo(string si){ Sinfo = si;}
    void addsinfo(string si){ Sinfo += si;}    
    
    friend ostream& operator << (ostream& os, const DT& dt);
    friend const DT operator +  (const DT& lval, const DT& rval);
    
};

DT::DT(int x, int y, float pce, string si) : 
X(x), Y(y), Pce(pce), Sinfo(si)
{
}

ostream& operator << (ostream& os, const DT& m_dt)
{
    os  << setw(4) << right << m_dt.X << " | " 
        << setw(4) << right << m_dt.Y << " | " 
        << fixed << right << setw(9) << setprecision(4) << m_dt.Pce << " | " 
        << left << m_dt.Sinfo << flush;
return os;
}

const DT operator + (const DT& lv, const DT& rv) 
{
    return DT( lv.X + rv.X, lv.Y + rv.Y, lv.Pce + rv.Pce, lv.Sinfo + " " + rv.Sinfo );
}

int main(int argc, char **argv)
{
    cout << "Test C++" << endl;
    
    DT dt1(10,20,0.25,"Data sience");       cout << dt1 << endl;
    DT dt2( 5, 7,0.35,"thiffen british");   cout << dt2 << endl;    
    
    DT dtsum1 = dt1 + dt2;
    dtsum1.addsinfo(" |");
    
    DT dx( 1, 5, 100.5, "1 point");        cout << dx << endl;    
    DT dy( 1, 5, 101.5, "2 point");        cout << dy << endl;    
    DT dz( 1, 5, 102.5, "3 point");        cout << dz << endl;    

    DT dtSum = dtsum1 + dx + dy + dz;
    dtSum.setsinfo("Summa DT's");

    cout << "----------------------------------------" << endl;
    cout << dtSum << endl;
	
	return 0;
}

