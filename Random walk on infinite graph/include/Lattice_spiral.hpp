/*==========================================================
 * Lattice (base and derived classes)
 * Architecture is:
 * Lattice (Base, pure virtual) --> Lattice_spiral (derived, spiral indexation)
 *========================================================*/ 

#ifndef LATTICE_SPIRAL					// avoid repeated expansion
#define LATTICE_SPIRAL

#include "Lattice.hpp"

//**************** Lattice_spiral Class (derived) ****************
class Lattice_spiral : public Lattice {
public:
    Lattice_spiral(const bigInt, const bigInt, const double, const unsigned, const double, const unsigned);   //Default Constructor
    bool check_new_node(const PointND *); //default search for a 'new' node (= is this site already visited?)
    bool check_new_node(const PointND *, std::mt19937_64 &); //alternative seach including percolation
    bool check_new_node_jump(const PointND *); //alternative seach including a jumping walker, which requires on-the-fly relallocation      

private:
    bigInt get_n_nodes(const bigInt) const;
    bigInt get_index(const PointND &) const;
};

#endif

