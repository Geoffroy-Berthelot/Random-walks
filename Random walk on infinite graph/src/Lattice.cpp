 /*==========================================================
  * Lattice base class
  * The 'lattice' is a n-dimensional array
  *========================================================*/
#include "Lattice.hpp"

/*==========================================================
 * Lattice (abstract, pure virtual  class)
 *
 *========================================================*/

//------------------------------------------------------------------------------
// Constructor
// L_dim_space is the number of nodes on one side (one dimension) of the lattice
// L_space is 'delta'
// warning: no type-cast verification here.
Lattice::Lattice(const bigInt n_preys, const bigInt N0, const double lattice_size, const unsigned dim, const double percolation_value, const unsigned x_max)
    :nodes{N0}, L_size{lattice_size}, L_dim{dim},
    //Let's use a ternary operator for dim 1, as we don't need the std::pow() machinery for this dimension:
    L_dim_size{ 
        dim == 1 ? N0
        : (bigInt) round( std::pow( N0, 1.0/dim ) )
    },
    L_space { 
        dim == 1 ? lattice_size / ( n_preys - 1 ) 
        : lattice_size / ( std::pow( n_preys, 1.0/dim ) - 1 ) 
    },
    p_perco{ percolation_value },
    max_jump_size{ x_max }
{
    is_visited = (char *) malloc(N0 * sizeof(char));
    memset(is_visited, '\0', N0 * sizeof(char) );  //all elements initialized to '\0'
}
    
//------------------------------------------------------------------------------
// Print information (for information purposes)
void Lattice::print_info() const {
    std::cout << "\n___LATTICE INFO___\n";
    std::cout << "nodes = " << nodes << " (total number of nodes)" << std::endl;
    std::cout << "L_size = " << L_size << " (size of lattice)" << std::endl;
    std::cout << "L_dim = " << L_dim << " (dimension of lattice)" << std::endl;
    std::cout << "L_dim_size = " << L_dim_size << " (number of nodes per side)" << std::endl;
    std::cout << "L_space = " << L_space << " (spacing between nodes)" << std::endl;
}

//------------------------------------------------------------------------------
// Percolate the lattice according to 'p_perco' (between [0, 1])
// This function percolates the initial lattice (ie. at the start of the simulation)
// Another function is provided (see below) for the percolation of new, added sites 
// (when the walker is outside the current lattice)
void Lattice::percolate_init( std::mt19937_64 &engine ) {

    std::uniform_real_distribution<double> rand_p(0.0, 1.0);

    // This loop can be time consuming at start, 
    // depending on the value of N0_1D, N0_2D or N0_3D (see 'include/Config_types.hpp').
    // Recall that all values of is_visited[] were previously set to '0' using memset(),
    // such that we don't need to push '0' values (i.e. else is_visited[p++] = '\0';).
    // Note that this loop can be costly in terms of running-time, depending on 'N0' values
    // (see 'Config_types.hpp')
    for( bigInt p=0; p < nodes; p++ ) {
        if( rand_p(engine) < p_perco )
            is_visited[p] = '1'; //set to '1', consider it as already been visited (=empty)
    }
}

//------------------------------------------------------------------------------
// Percolates new (added) nodes to the original lattice.
void Lattice::percolate_new( const bigInt index_start, const bigInt index_end, std::mt19937_64 &engine ) {
    
    std::uniform_real_distribution<double> rand_p(0.0, 1.0);

    bigInt p = index_start;
    while( p < index_end )
        if( rand_p(engine) < p_perco )
            is_visited[p++] = '1'; //already visited (=empty)
        else
            is_visited[p++] = '\0'; //not visited
}

//------------------------------------------------------------------------------
// Return L_space (spacing between nodes)
double Lattice::get_L_space() const {
    return L_space;
}

//------------------------------------------------------------------------------
// Return L_dim (spacing between nodes)
unsigned Lattice::get_L_dim() const {
    return L_dim;
}

//------------------------------------------------------------------------------
// Sets the center position of the lattice (central node):
void Lattice::set_center_pos( PointND * center_pos ) const {

    //start at the center of the lattice: [0,0] for a n-dimensional lattice:
    for(unsigned i = 0; i < L_dim; i++)
        (*center_pos)[i] = 0;
}

//------------------------------------------------------------------------------
// Sets the center position of the lattice (central node):
void Lattice::set_center_pos( PointND & center_pos ) const {

    //start at the center of the lattice: [0,0] for a n-dimensional lattice:
    for(unsigned i = 0; i < L_dim; i++)
        center_pos[i] = 0;
}

//------------------------------------------------------------------------------
// Destructor
Lattice::~Lattice() {
    free(is_visited);
}
