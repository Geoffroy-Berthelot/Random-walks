#ifndef POINTND_HELP				// avoid repeated expansion
#define POINTND_HELP

#include<array>
#include<iostream>
#include<limits>
#include "config_types.hpp"

//**************** PointND class ****************
class PointND
{
    public:

    //------------------------------------------------------------------------------
    // Empty constructor
    PointND()
    {
        //std::cout << "=> je suis crée aléatoirement avec 0 dimensions\n";
    }

    //------------------------------------------------------------------------------
    // Specific constructor for all dimensions with infinity.
    // we use infinity to show that the walker has not yet be initialized in the simulation (see FRsim).
    PointND( const unsigned int dim )
    :nDimensions{ dim }
    {
        elements_ = (cInt*) malloc(dim * sizeof(cInt));
        std::fill_n(elements_, nDimensions, std::numeric_limits<cInt>::infinity());
        //std::cout << "=> je suis crée avec 2 dimensions\n";
    }

    //------------------------------------------------------------------------------
    // Destructor
    ~PointND() {
        //std::cout << "=> je suis detruit\n";
        free( elements_ );
    }

    //------------------------------------------------------------------------------
    // returns the number of dimensions
    unsigned int n_dim() const {
        return nDimensions;
    }
    
    //------------------------------------------------------------------------------
    // Overloading operators [], +=, -=, +, -, == and <<
    cInt& operator [] ( unsigned const i )
    {
        return elements_[i];
    }

    cInt const& operator [] ( unsigned const i ) const
    {
        return elements_[i];
    }

     void operator += ( PointND const& other )
    {
        for( unsigned i = 0; i < nDimensions; ++i )
            elements_[i] += other.elements_[i];
    }

    void operator -= ( PointND const& other )
    {
        for( unsigned i = 0; i < nDimensions; ++i )
            elements_[i] -= other.elements_[i];
    }

    bool operator == ( PointND const& other )
    {
        for( unsigned i = 0; i < nDimensions; ++i )
            if( elements_[i] != other.elements_[i] )
                return false;
        return true;
    }
    
    bool operator == ( const PointND * other )
    {
        for( unsigned i = 0; i < nDimensions; ++i )
            if( elements_[i] != other->elements_[i] )
                return false;
        return true;
    }    
    
    friend PointND operator + ( PointND const& a, PointND const& b )
    {
        PointND ret( a );
        ret += b;
        return ret;
    }

    friend PointND operator - ( PointND const& a, PointND const& b )
    {
        PointND ret( a );
        ret -= b;
        return ret;
    }
    
    friend std::ostream &operator<<( std::ostream &out, const PointND * nd ) {
        for(unsigned i = 0; i < nd->nDimensions-1; i++)
            out << nd->elements_[i] << ",";
        out << nd->elements_[nd->nDimensions-1];
        return out;
    }
    
    friend std::ostream &operator<<( std::ostream &out, const PointND & nd ) {
        for(unsigned i = 0; i < nd.nDimensions-1; i++)
            out << nd.elements_[i] << ",";
        out << nd.elements_[nd.nDimensions-1];
        return out;
    }
    
    private:
    const unsigned int nDimensions = 2; //default number of Dimensions is 2
    cInt * elements_ = NULL;
};

#endif

