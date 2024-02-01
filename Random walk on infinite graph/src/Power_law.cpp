/*==========================================================
 * Power law (Discrete in interval [a,b])
 *========================================================*/
#include "Power_law.hpp"

//------------------------------------------------------------------------------
// Discrete power law constructor:
Power_law::Power_law(const unsigned a, const unsigned b, const double gamma_)
    :S{b-a+1}, gamma{gamma_}
{
    // definitions:
    unsigned p = a;
    double C = 0.0;
    
    // Allocations and initializations:
    B1 = (double *) malloc( S * sizeof(double) );
    val = (unsigned *) malloc ( S * sizeof(unsigned) );

    // fill B1 array with zeros:
    std::fill(B1, B1 + S, 0);
    
    // fill val array with desired values:
    for (unsigned i = 0; i < S; i++)
        val[i] = p++;

    // 1) Estimate of the normalisation coefficient C:
    // C_{a,b,\gamma}= \sum_{i=a}^b i^{-\gamma}:
    C = 0.0;
    p = a; //reset 'p' to 'a';
    while(p <= b)
        C += std::pow(p++, -gamma);

    // 2) populate B1, array of intervals:
    for( unsigned k = a; k <= b ; k++ ) {
        double sumB1 = 0;
        unsigned i = a;
        while( i <= k-1 )
            sumB1 += std::pow(i++, -gamma);

        B1[k-a] = 1/C * sumB1;
    }
}
    
Power_law::~Power_law(){
    free(B1);
    free(val);
}

//------------------------------------------------------------------------------
// Returns a draw from a discrete power draw distribution truncated in [a, b]:
unsigned Power_law::rand( const double r ) const{
    // Draw one random unsigned integer using U_j ~ U[0,1] that falls into the intervals of B1 array:
    
    // We search for the index of rand(u) in B1 array:
    unsigned idx = 0;
    while( r > B1[idx] ) {
        idx++;
    }
    
    if(idx == 0)
        return val[0];
    else
        if( idx > S )
            return val[S-1];
        else
            return val[idx-1];
}

