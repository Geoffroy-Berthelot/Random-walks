/*==========================================================
 * Lattice (base and derived classes)
 * Architecture is:
 * Lattice (Base, pure virtual) --> Lattices_spiral (derived, spiral indexation)
 *========================================================*/ 

#ifndef LATTICE         // avoid repeated expansion
#define LATTICE

#include "config_types.hpp"
#include "PointND.hpp"
#include "Tools.hpp"
#include <cmath>
#include <iostream>
#include <cstring>
#include <algorithm>    // std::remove
#include <random>

//**************** Lattice Class (base, pure virtual) ****************
class Lattice {
public:
    Lattice(const bigInt, const bigInt, const double, const unsigned, const double, const unsigned);
    virtual bool check_new_node(const PointND *) = 0;
    virtual bool check_new_node(const PointND *, std::mt19937_64 & ) = 0;
    virtual bool check_new_node_jump(const PointND *) = 0;
    double get_L_space() const; //getter for L_space
    unsigned get_L_dim() const; //getter for L_dim
    void set_center_pos( PointND * ) const;
    void set_center_pos( PointND & ) const;
    void print_info() const;
    void percolate_init( std::mt19937_64 & );
    void percolate_new( const bigInt, const bigInt, std::mt19937_64 & );
    
    //------------------------------------------------------------------------------
    // Check if 'k' can hold inside the 'bigInt' type limits (i.e. if 'k' is saturated):
    // Warning: causes memory leaks if an error is thrown!
    inline void check_limits( unsigned long long int k ) const {
        
        if( ( k > std::numeric_limits< bigInt >::max() ) == true ) {
            this->~Lattice();
            throw std::runtime_error("The walker is outside of the maximum lattice node. Change the type of ''bigInt'' in ''Config_types.hpp''");
        }
    }
    
    ~Lattice();

protected:
    bigInt nodes;                   //number of nodes. It is not const, as the number of nodes may vary depending on the size of the lattice (see check_new_node())
    const double L_size = 0.0;      //Size of lattice
    const unsigned L_dim = 0;       //dim of lattice
    bigInt L_dim_size = 0;          //number of elements (nodes) in each dimension (should be const after temporary changes)
    const double L_space = 0.0;     //spacing between nodes
    char * is_visited = NULL;       //is position in L_position already visited? ('\0' = no, '1' = yes)
    virtual bigInt get_index(const PointND &) const = 0;
    const double p_perco = 0.0;     //percolation (probability)
    const unsigned max_jump_size = 0;  //corresponds to the max step size when performing a jump
};

#endif
