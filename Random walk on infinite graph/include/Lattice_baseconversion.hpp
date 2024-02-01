/*==========================================================
 * Lattice_baseconversion (derived classes)
 *========================================================*/ 

#ifndef LATTICE_BASE 				// avoid repeated expansion
#define LATTICE_BASE

#include <cmath>
#include <vector>
#include "Lattice.hpp"

//**************** Lattice_baseconversion Class (derived) ****************
class Lattice_baseconversion : public Lattice {
public:
    Lattice_baseconversion(const bigInt, const bigInt, const double, const unsigned, const double, const unsigned);   //Default Constructor
    bool check_new_node(const PointND *);
    bool check_new_node(const PointND *, std::mt19937_64 &);
    bool check_new_node_jump(const PointND *);

private:
    cInt k = 0; //translation parameter when switching bases (offset)
    bigInt get_index( const PointND & ) const;
    bigInt get_n_nodes( const unsigned ) const;
    void get_n_nodes_after_jump( const PointND &, unsigned & ) const;
    void get_Point_from_index( const bigInt n, PointND & ) const;
    void resize_lattice_vector( const unsigned, const unsigned );
    void resize_lattice_carray( const unsigned, const unsigned );
};

#endif
