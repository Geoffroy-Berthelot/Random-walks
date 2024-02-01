 /*==========================================================
  * Lattice_spiral class, derived from Lattice class
  * The 'lattice' is a n-dimensional array,
  * and we move into this array.
  * using the indexes i,j... \in [0, ..., L_dim_size-1]
  * In this class, we use a spiral indexation method for dimension 2 only
  *========================================================*/
#include <cmath>
#include "Lattice_spiral.hpp"

/*==========================================================
 * Lattice spiral (derived class, see Lattice for base class)
 *========================================================*/
//------------------------------------------------------------------------------
// Constructor:
Lattice_spiral::Lattice_spiral(const bigInt n_preys, const bigInt N0, const double lattice_size, const unsigned dim, const double percol_value, const unsigned x_max)
:Lattice(n_preys, N0, lattice_size, dim, percol_value, x_max) {}

//------------------------------------------------------------------------------
// Check if node is already visited (2D only), if not, mark it as visited.
// We also check the node is inside the lattice, otherwise we increase its size by 2 layers (fixed value).
bool Lattice_spiral::check_new_node(const PointND * pt) {

    unsigned idx = 0;

    // converts [i,j] (2-dimensional point) to spiral index:
    idx = (bigInt) get_index( *pt );
    
    if( idx >= nodes ) {
        // then index is out of the array, containing all nodes.
        // we need reallocation of "is_visited":

        const bigInt old_nodes = nodes; // save old values of nodes before increasing.
        // increases node values by 2 'rings' or 'layers',
        // where LDS is the number of nodes in one size (i.e. L_DIM_SIZE).
        // thus total increase (2 layers) is:
        const unsigned add_ = L_dim_size * 8 + 16;
        // for generalization we can use:
        // number of elements to add = 4 * n_layer * ( L_dim_size + n_layer )
        // const unsigned add_ = 4 * n_layer * ( L_dim_size + n_layer )
        // where 'n_layer' is the number of 'layers' we want to add.
        // (a 'layer' corresponds to the nodes surrounding the current lattice, or rings).
        
        nodes += (bigInt) add_;
        is_visited  = (char *) realloc( is_visited, nodes * sizeof(char) );
        memset( &is_visited[old_nodes], '\0', (nodes - old_nodes) * sizeof(char) );
        
        L_dim_size += 4;
        //or L_dim_size += n_layer*2;
    }
    
    // Check whether the node at index 'idx' was previously visited or not:
    if(is_visited[idx] == '\0') {
        is_visited[idx] = '1';
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
// Check if node is already visited (2D only), if not, mark it as visited.
// We also check the node is inside the lattice, otherwise we increase its size by 2 layers (fixed value).
// (Percolation version)
bool Lattice_spiral::check_new_node(const PointND * pt, std::mt19937_64 &engine) {

    unsigned idx = 0;

    //convert [i,j] (2-dimensional point) to spiral index:
    idx = (bigInt) get_index( *pt );
    
    if( idx >= nodes ) {
        // The index is out of the array, containing all nodes.
        // reallocation of "is_visited":
        const bigInt old_nodes = nodes; //save old values of nodes before increasing.
        // increases node values by 2 'rings' or 'layers',
        // where LDS is the number of nodes in one size (ie. L_DIM_SIZE).
        // thus total increase (2 layers) is:
        const unsigned add_ = L_dim_size * 8 + 16;
        // for generalization we can use:
        // number of elements to add = 4 * n_layer * ( L_dim_size + n_layer )
        // const unsigned add_ = 4 * n_layer * ( L_dim_size + n_layer )
        // where 'n_layer' is the number of 'layers' we want to add.
        // (a 'layer' corresponds to the nodes surrounding the current lattice, or rings).
        
        nodes += (bigInt) add_;
        is_visited  = (char *) realloc( is_visited, nodes * sizeof(char) );

        // We initialize (new) "empty" nodes during the percolation:
        percolate_new( old_nodes, nodes, engine );

        L_dim_size += 4;
    } 
    
    // Check whether the node at index 'idx' was previously visited or not.
    // In the case of percolation a new site can already be marked as 'visited'
    if(is_visited[idx] == '\0') {
        is_visited[idx] = '1';
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
// Check if node is already visited (2D only), if not, mark it as visited.
// We also check the node is inside the lattice, 
// otherwise we increase its size by the required number of layers (depending on the targeted site).
// (Jump version)
bool Lattice_spiral::check_new_node_jump(const PointND * pt) {

    unsigned idx = 0;
    // Sets a 'margin' variable which allows for some 'room' (i.e. additional layers of nodes) for latter jumps,
    // this prevents entering the idx >= nodes conditions 'too often'.
    const unsigned margin = 4; 
    unsigned n_layers = 0;

    // convert [i,j] (2-dimensional point) to spiral index:
    idx = (bigInt) get_index( *pt );

    if( idx >= nodes ) {
        // then index is out of the array, containing all nodes.
        // we need reallocation of "is_visited":

        // compute the number of new nodes to add to the lattice,
        // that is the number of new elements to add to the 'is_visited' array.
        // simply increase the number of layers by : max_jump_size * margin
        n_layers = max_jump_size * margin;
        const unsigned n_elements = 4 * n_layers * ( L_dim_size + n_layers );
        // or, alternatively, the following function can be used:
        // const unsigned n_elements = get_n_nodes(idx);
        
        // thus resize the array accordingly:
        const bigInt old_nodes = nodes; //copy the value of nodes to old_nodes
        nodes += n_elements;
        
        is_visited  = (char *) realloc( is_visited, nodes * sizeof(char) );
        memset( &is_visited[old_nodes], '\0', (nodes - old_nodes) * sizeof(char) );
        
        // then accordingly increase 'L_dim_size':
        L_dim_size = (unsigned) std::sqrt(nodes);
    }
    
    // Check whether the node at index 'idx' was previously visited or not:
    if(is_visited[idx] == '\0') {
        is_visited[idx] = '1';
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
// Estimate the number of new nodes to append to the 'visited' array
// thus we directly returns the number of elements (new nodes) to add to the lattice:
bigInt Lattice_spiral::get_n_nodes(const bigInt idx) const {
    
    bigInt Lds_0 = L_dim_size;
    
    // increment by two (because the number of nodes for each new layer is L_dim_size + 2;
    while( Lds_0*Lds_0 < idx )
        Lds_0 += 2;
    
    // then returns the number of new elements to add to the array:
    return Lds_0 * Lds_0 - L_dim_size * L_dim_size;
}

//------------------------------------------------------------------------------
// returns index from a spiral (2D only):
// adapted from: https://stackoverflow.com/questions/9970134/get-spiral-index-from-location
bigInt Lattice_spiral::get_index(const PointND &pt) const {

    unsigned long long int p = 0;

     if( pt[1] * pt[1] >= pt[0] * pt[0] ) {
        p = 4 * pt[1] * pt[1] - pt[1] - pt[0];
        if ( pt[1] < pt[0] )
            p = p - 2 * ( pt[1] - pt[0] );
    } else {
        p = 4 * pt[0] * pt[0] - pt[1] - pt[0];
        if ( pt[1] < pt[0] )
            p = p + 2 *( pt[1] - pt[0] );
    }
    
    // checks whether 'p' can fit in a 'bigInt' type:
    check_limits( p );
    
    return (bigInt) p;
}
