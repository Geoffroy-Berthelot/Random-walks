/*==========================================================
 * Power law (Discrete in interval [a,b])
 *========================================================*/ 

#ifndef POWERLAW_					// avoid repeated expansion
#define POWERLAW_

#include<algorithm>
#include<cmath>
#include<iostream>

class Power_law {
public:
    Power_law(const unsigned, const unsigned, const double);   //Default Constructor
    const double Init_C(int, const int );
    unsigned rand( const double ) const;
    ~Power_law();

private:
    const size_t S; //size of B1, val arrays
    double *B1 = NULL;
    unsigned *val = NULL;
    const double gamma = 0.0;
};

#endif

