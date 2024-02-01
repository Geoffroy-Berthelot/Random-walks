 /*==========================================================
  * Lattice_baseconversion class, derived from Lattice class
  * The 'lattice' is a n-dimensional array, and we move into this array
  * using the indexes i,j... \in [0, ..., L_dim_size-1].
  * In this class, we use a base conversion method for dimensions [1, 3]
  *========================================================*/
#include "Lattice_baseconversion.hpp"

/*==========================================================
 * Lattice base conversion (derived class, see 'Lattice.hpp' for base class)
 *========================================================*/
//------------------------------------------------------------------------------
// Constructor:
Lattice_baseconversion::Lattice_baseconversion(const bigInt n_preys, const bigInt N0, const double lattice_size, const unsigned dim, const double percol_value, const unsigned x_max)
:Lattice(n_preys, N0, lattice_size, dim, percol_value, x_max) {
    
    // Warning: N0 should be 'odd', otherwise it will create an offset error.
    // In particular in the get_n_nodes_after_jump() function.
    k = (cInt) (L_dim_size-1) / 2;
}

//------------------------------------------------------------------------------
// Compute the number of nodes to add to the lattice given:
// 'L_dim_size' : the number of nodes on one 'side' of the lattice (i.e. in one dimension)
// 'n_layers' : the number of nodes we want to add to L_dim_size 
// (i.e. the total number of new 'layers' we want to add to the lattice)
bigInt Lattice_baseconversion::get_n_nodes( const unsigned n_layers ) const {

    bigInt to_add = 0;

    if(L_dim == 1)
        to_add = 2*n_layers;
    else
        if(L_dim == 2)
            //to_add = (L_DIM_SIZE + 2*n_layers) * (L_DIM_SIZE + 2*n_layers) - L^1;
            to_add = 4 * L_dim_size * n_layers + 4 * n_layers * n_layers;
        else
            if(L_dim == 3)
                //to_add = (L_DIM_SIZE + 2*n_layers) * (L_DIM_SIZE + 2*n_layers) - L^2;
                to_add = 3 * L_dim_size * L_dim_size * 2 * n_layers
                        + 3 * L_dim_size * 4 * n_layers * n_layers
                        + 8 * n_layers * n_layers * n_layers;
            else
                if(L_dim > 3)
                    // This can be generalized to:
                    // we can use: (L_DIM_SIZE+2*n_layers)^d - L_DIM_SIZE^d
                    // where L_DIM_SIZE is the number of nodes in one 'side'.
                    to_add = std::pow( L_dim_size + 2 * n_layers, L_dim ) - std::pow( L_dim_size, L_dim );

    return to_add;
}

//------------------------------------------------------------------------------
// The walker is outside the lattice, thus we resize the lattice.
// 'to add' is the number of nodes to add to the lattice.
// This version implements a vector approach.
void Lattice_baseconversion::resize_lattice_vector( const unsigned to_add, const unsigned n_layers ) {
    
    // ----------------------- MEMORY REALLOCATION ('visited' array):
    // In the following section we assume that the sum of visited positions holds in a 'bigInt' type.
    // Otherwise, this should be expanded to another type.
    // The procedure is as follow:
    // step 1 - gather the indexes of visited nodes / positions,
    // step 2 - reset is_visited[],
    // step 3 - recompute the indexes in the new (changed) base,
    // step 4 - refill is_visited[] according to the indexes collected in step 1.
    // A (probably) faster implementation could be achieved with:
    // std::unique_ptr<bigInt[]> index_visited( new bigInt[number_visited] );
    // that is zero-overhead (unlike std::vector).
    // However, since we only use it for reallocation, vector would suffice.
    
    // (A) Declare the array (vector) of visited positions and
    // fill it with the index of positions already visited:
    std::vector<bigInt> index_visited;
    for(bigInt i=0; i < nodes; ++i) {
        if(is_visited[i] == '1')
            index_visited.push_back(i);
    }
    
    // (B) We reset 'visited' array and reallocate the memory accordingly to new number of nodes:
    // (B-1) the total number of nodes in the lattice is now:
    nodes += to_add;
    
    // (B-2) the size of the array (re-allocation):
    is_visited  = (char *) realloc( is_visited, nodes * sizeof(char) );
    memset( &is_visited[0], '\0', nodes * sizeof(char) ); //we reset all the values to '\0'
    
    // We update 'L_DIM_SIZE' and 'k' variables:
    L_dim_size += 2 * n_layers;
    const cInt new_k = (cInt) (L_dim_size-1) / 2; //new value of 'k'
    const cInt old_k = k; //old value of 'k'
    // we switch between the two 'k' in order to change the bases (see next section).
    
    // ----------------------- RE-FILL THE ARRAY WITH PREVIOUS VALUES:
    // We now update the new 'is_visited':
    PointND * pt = new PointND( L_dim );
    bigInt idx2 = 0, n=0;
    while( !index_visited.empty() ) {
        
        // get index 'n' at position index_visited[i] and
        // compute it back to PointND:
        k = old_k;
        n = index_visited.back();    //get last item of 'index_visited'
        index_visited.pop_back();           //clear last item of 'index_visited'
        get_Point_from_index( n, *pt );     //get corresponding position
        
        // compute the new index with new 'k':
        k = new_k;
        idx2 = get_index( *pt );
        
        // this site is marked as (already) visited:
        is_visited[idx2] = '1';
    }
    
    delete pt;
}

//------------------------------------------------------------------------------
// The walker is outside the lattice, thus we resize the lattice.
// 'to add' is the number of nodes to add to the lattice.
// This version implements a C array.
void Lattice_baseconversion::resize_lattice_carray( const unsigned to_add, const unsigned n_layers ) {

    // ----------------------- MEMORY REALLOCATION ('visited' array):
    // (A) Declare the array (vector) of visited positions and
    // fill it with the index of positions already visited:
    bigInt * index_visited = (bigInt *) malloc(nodes * sizeof(bigInt));
    // we fill it with the index of visited positions:
    bigInt n_visited = 0;
    for(bigInt i=0; i < nodes; ++i) {
        if(is_visited[i] == '1')
            index_visited[n_visited++] = i;
    }
    
    // (B) We reset 'visited' array and reallocate the memory accordingly to new number of nodes:
    // (B-1) the total number of nodes in the lattice:
    //const bigInt old_nodes = nodes; // save old values of 'nodes' before increasing 'nodes'
    nodes += to_add; // new N_0

    // (B-2) the size of the array (re-allocation):
    is_visited  = (char *) realloc( is_visited, nodes * sizeof(char) );
    memset( &is_visited[0], '\0', nodes * sizeof(char) ); //we reset all the values to '\0'

    // We update 'L_DIM_SIZE' and 'k' variables:
    L_dim_size += 2 * n_layers;
    const cInt new_k = (cInt) (L_dim_size-1) / 2; //new value of 'k'
    const cInt old_k = k; //old value of 'k'
    //we switch between the two 'k' in order to change the bases (see next section).

    // ----------------------- RE-FILL THE ARRAY WITH PREVIOUS VALUES:
    //std::cout << "STEP 3\n";
    // We now update the new 'is_visited':
    PointND * pt = new PointND( L_dim );
    bigInt idx2 = 0;
    for(bigInt kn = 0; kn < n_visited; kn++) {
        // get index 'n' at position index_visited[i] and
        // compute it back to PointND:
        k = old_k;
        get_Point_from_index( index_visited[kn], *pt );     //get corresponding position
        
        // compute the new index with new 'k':
        k = new_k;
        idx2 = get_index( *pt );
        
        // this site is marked as (already) visited:
        is_visited[idx2] = '1';
    }
    
    free(index_visited);
    delete pt;
}

//------------------------------------------------------------------------------
// Check if node is already visited, if not, mark it as visited.
// We also check the node is inside the lattice, otherwise we increase its size by 'n_layers' layers (hardcoded value).
bool Lattice_baseconversion::check_new_node(const PointND * pt) {

    bigInt idx = 0;
    const unsigned n_layers = 100;   // hardcoded parameter, where one 'layer' is the number of nodes required for k = k+1 (i.e. increasing size of lattice by 1)
    bigInt to_add = 0;               // number of new nodes (or indexes in the array) to add, initialized at 0.

    // we first gather the index of the position 'pt' in the 'visited' array:
    idx = get_index( *pt );
    
    // check whether 'idx' is inside the lattice or not,
    // i.e. inside the array 'visited', containing all the indexes of the 
    // possible positions in the lattice with current size L.
    // Otherwise, we increase the size of the lattice:
    if( idx >= nodes ) {
        // compute the number of nodes to add to the lattice
        to_add = get_n_nodes( n_layers );
        
        // resize the lattice accordingly:
        resize_lattice_carray( to_add, n_layers ); //for c-style array implementation
        //resize_lattice_vector( to_add, n_layers ); // alternative vector() implementation

        // we changed the base and 'k', the current index 'idx' is not longer valid.
        // thus, we update it:
        idx = get_index( *pt );
    }
    
    // check whether the site is already visited or not:
    if(is_visited[idx] == '\0') {
        is_visited[idx] = '1';
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
// Check if node is already visited, if not, mark it as visited.
// We also check the node is inside the lattice, otherwise we increase its size by 'n_layers' layers (hardcoded value).
// (Percolation version)
bool Lattice_baseconversion::check_new_node(const PointND * pt, std::mt19937_64 & engine) {
    
    bigInt idx = 0;
    const unsigned n_layers = 100;   // hardcoded parameter, where one 'layer' is the number of nodes required for k = k+1 (i.e. increasing size of lattice by 1)
    bigInt to_add = 0;               // number of new nodes (or indexes in the array) to add.

    // we first gather the index of the position 'pt' in the 'visited' array:
    idx = get_index( *pt );
    
    // check whether 'idx' is inside the lattice or not,
    // i.e. inside the array 'visited', containing all the indexes of the 
    // possible positions in the lattice with current size L.
    // Otherwise, we increase the size of the lattice:
    if( idx >= nodes ) {
        
        // compute the number of nodes to add to the lattice
        to_add = get_n_nodes( n_layers );
        
        // ----------------------- MEMORY REALLOCATION ('visited' array):
        // Here we use a different method:
        // (1) we find the visited ('1') and non-visited ('0') sites/nodes,
        // (2) we compute the new indexes of visited and non-visited sites,
        // (3) all other indexes are considered as new sites/nodes (with '0' value),
        // (4) we then percolate these new sites/nodes.
        
        // (A) Declare the array (vector) of visited positions and
        // fill it with the index of positions already visited:
        bigInt * index_visited = (bigInt *) malloc(nodes * sizeof(bigInt));
        bigInt * index_not_visited = (bigInt *) malloc(nodes * sizeof(bigInt));
        // we fill it with the index of visited and not visited positions:
        bigInt n_visited = 0; // is the number of visited sites.
        bigInt not_visited = 0;
        for(bigInt i=0; i < nodes; ++i) {
            if(is_visited[i] == '1')
                index_visited[n_visited++] = i;
            else
                index_not_visited[not_visited++] = i;
        }
        
        // (B) We reset 'visited' array and reallocate the memory accordingly to new number of nodes:
        // (B-1) the total number of nodes in the lattice is:
        nodes += to_add;
        
        // (B-2) the size of the array (re-allocation):
        is_visited  = (char *) realloc( is_visited, nodes * sizeof(char) );
        memset( &is_visited[0], '\0', nodes * sizeof(char) ); //we reset all the values to '\0'
        
        // We update 'L_DIM_SIZE' and 'k' variables:
        L_dim_size += 2 * n_layers;
        
        const cInt new_k = (cInt) (L_dim_size-1) / 2; //new value of 'k'
        const cInt old_k = k; //old value of 'k'
        //we switch between the two 'k' in order to change the bases (see next section).
        
        // ----------------------- RE-FILL THE ARRAY WITH PREVIOUS (VISITED) VALUES:
        // We now update the new 'is_visited':
        PointND * pt = new PointND( L_dim );
        bigInt idx2 = 0;
        for(bigInt kn = 0; kn < n_visited; kn++) {
            
            // get index 'n' at position index_visited[i] and
            // compute it back to PointND:
            k = old_k;
            get_Point_from_index( index_visited[kn], *pt );     //get corresponding position
            
            // compute the new index with new 'k':
            k = new_k;
            idx2 = get_index( *pt );
            
            // this site is marked as (already) visited:
            is_visited[idx2] = '1';
        }
        
        // ----------------------- RE-FILL THE ARRAY WITH PREVIOUS (NOT VISITED) VALUES:
        // We now update the new 'is_visited':
        for(bigInt kn = 0; kn < not_visited; kn++) {
            
            // get index 'n' at position index_visited[i] and
            // compute it back to PointND:
            k = old_k;
            get_Point_from_index( index_not_visited[kn], *pt );     //get corresponding position
            
            // compute the new index with new 'k':
            k = new_k;
            idx2 = get_index( *pt );
            
            // this site is marked with a special value '2':
            is_visited[idx2] = '2';
        }
        
        // We changed the base and 'k', the current index 'idx' is not longer valid,
        // Thus, we update it:
        idx = get_index( *pt );
                
        // ----------------------- PERCOLATE NEW (ADDED) SITES/NODES:
        // We now have an array filled with '\0', '1' and '2'.
        // We then search for these '2' values and replace them 
        // with either '1' (visited) or '0' (not visited), such that 
        // we have the following map:
        // '1'  -> '1'  : already visited, no change
        // '2'  -> '\0' : not visited previously, reverse back to '\0'
        // '\0' -> '1'  : either percolated or '\0' (a new -not percolated- site/node)
        // we browse the full array, which can be costly if 'nodes' is large:
        
        std::uniform_real_distribution<double> rand_p(0.0, 1.0);
        
        for(bigInt kn = 0; kn < nodes; kn++) {
            // is site == '2', it means that it was already marked as (previously) not-visited
            // we then reset its value to '0'
            if( is_visited[kn] == '2' )
                is_visited[kn] = '\0';
            else 
                // is site == '0', it's a new site, added during the resizing procedure of the lattice.
                // we apply percolation:
                if( is_visited[kn] == '\0' )
                    if( rand_p(engine) < p_perco )
                        is_visited[kn] = '1'; //percolated: considered as already visited ( = empty )
        }
        
        delete pt;
        free(index_not_visited);
        free(index_visited);
    }
    
    // Either the walker is 'inside' or 'outside' the lattice, 
    // but the site can already be visited:
    if(is_visited[idx] == '\0') {
        is_visited[idx] = '1';
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
// Check if node is already visited, if not, mark it as visited.
// We also check the node is inside the lattice, otherwise we increase its size by 'n_layers' layers (hardcoded value).
// (Jump version)
bool Lattice_baseconversion::check_new_node_jump(const PointND * pt) {

    bigInt idx = 0;
    unsigned n_layers = 0;
    bigInt to_add = 0;  // number of new nodes (or indexes in the array) to add.
    // Sets a 'margin' variable which allows for some 'room' (i.e. additional layers of nodes) for latter jumps,
    // this prevents entering the idx >= nodes conditions 'too often'.
    const unsigned margin = 4;
    
    // we first gather the index of the position 'pt' in the 'visited' array:
    //std::cout << "get_index(): ";
    idx = get_index( *pt );

    // check whether 'idx' is inside the lattice or not,
    // i.e. inside the array 'visited', containing all the indexes of the 
    // possible positions in the lattice with current size L.
    // Otherwise, we increase the size of the lattice:
    if( idx >= nodes ) {
        
        //simply increase the number of layers by the max jump_size * margin
        n_layers = max_jump_size * margin;
        // or alternatively, compute the requested number of nodes to keep 
        // the walker inside the lattice using:
        // get_n_nodes_after_jump( *pt, n_layers );

        //=> check_new_node(pt, max_jump_size);
        to_add = get_n_nodes( n_layers );

        // resize the lattice accordingly:
        resize_lattice_carray( to_add, n_layers ); //for a c-style array implementation
        // resize_lattice_vector( to_add, n_layers ); //alternative vector() implementation
        
        // We changed the base and 'k', the current index 'idx' is not longer valid,
        // Thus, we update it:
        idx = get_index( *pt );
    } 

    // Check whether the node at index 'idx' was previously visited or not:
    if( is_visited[idx] == '\0' ) {
        is_visited[idx] = '1';
        return true;
    } 
    return false;
}

//------------------------------------------------------------------------------
// Return the number of nodes to add to the lattice such as 'pt' is inside the lattice.
// (generalized to 'n' dimensions)
void Lattice_baseconversion::get_n_nodes_after_jump( const PointND & pt, unsigned & n_layers ) const {

    const unsigned margin = 2; //the number of additional 'layers' we want to add
    // to avoid increasing the size of the lattice at each step.

    // first, find which dimension and how far is lying outside the lattice:
    double max_dim_outside = 0.0;
    const auto maxH = std::floor( (double) L_dim_size/2 );
    for( unsigned z = 0; z < L_dim; z++ ) {
        if( pt[z] > maxH && pt[z] > max_dim_outside) //outside of the positive side of the lattice
            max_dim_outside = pt[z];
        else 
            if( pt[z] < -maxH && -pt[z] > max_dim_outside)
            max_dim_outside = -pt[z];
    }

    // second, estimate the required increase in L_dim_size:
    if( max_dim_outside < 0.0 )
        max_dim_outside = - max_dim_outside;

    n_layers = (unsigned) max_dim_outside - (unsigned) maxH + margin;
}

void Lattice_baseconversion::get_Point_from_index( const bigInt n, PointND & pt ) const {

    const unsigned base = 2*k+1;

    if(L_dim == 1) //1-dimensional
        pt[0] = (cInt) n - k;
    else
        if(L_dim == 2) {
            const unsigned quot1 = (unsigned)n / base;  //quotient
            pt[0] = (cInt) (n % base) - k;              //reminder
            pt[1] = (cInt) (quot1 % base) - k;
        }
        else
            if(L_dim == 3) {
                const unsigned quot1 = (unsigned)n / base; //quotient
                pt[0] = (cInt) (n % base) - k;       // reminder
                pt[1] = (cInt) (quot1 % base) - k; 
                pt[2] = (cInt) ( n - pt[0] - k - base * (pt[1] + k) ) / (base * base) - k;
            }
            else
                if(L_dim > 3) //previous approach can be generalized to larger dimensions.
                    // (also see Horner's method)
                    std::cout << "todo" << std::endl;
}

//------------------------------------------------------------------------------
//returns index from a n-Dimensional position:
bigInt Lattice_baseconversion::get_index(const PointND &pt) const {
    
    unsigned long long int p = 0;
    
    // for 1,2 and 3 dimensional, we push cInt types ( from pt[] )
    // and promotes 'nodes' (bigInt) to a larger type ( unsigned long long int )
    
    //1-dimensional
    if(L_dim == 1) {
        if( pt[0] < -k || pt[0] > k ) //assert -k <= pt[.] <= k
            p = nodes + 1; //by setting p = nodes+1, we notify check_new_node() that the walker is outside the lattice.
        else
            p = pt[0] + k;
    } else
        //2-dimensional
        if(L_dim == 2) {
            if(     pt[0] < -k || pt[0] > k ||
                    pt[1] < -k || pt[1] > k ) { //assert  -k <= pt[.] <= k
                p = nodes + 1; //by setting p = nodes+1, we notify check_new_node() that the walker is outside the lattice.
            } else
                //idx = x + (2*k+1)*y; which translates to:
                p = pt[0] + k + ( 2*k+1 ) * (pt[1] + k);
        } else
            //3-dimensional
            if(L_dim == 3) {
                if(     pt[0] < -k || pt[0] > k ||
                        pt[1] < -k || pt[1] > k ||
                        pt[2] < -k || pt[2] > k ) //assert  -L_dim_size <= pt[.] <= L_dim_size
                    p = nodes + 1; //by setting p = nodes+1, we notify check_new_node() that the walker is outside the lattice.
                else {
                    // p = (bigInt) (pt[0] + k)  + ( 2*k+1 ) * (pt[1] + k)
                    //                          + ( 2*k+1 ) * ( 2*k+1 ) * (pt[2] + k);
                    // or following Horner's method (nested form polynomial) gives:
                    p  = pt[0] + k  + ( 2*k+1 ) * ( (pt[1] + k) + ( 2*k+1 ) * (pt[2] + k) );
                }
            }
            else
                //D-dimensional
                if(L_dim > 3) //previous approach can be generalized to larger dimensions.
                    p = 0;
    
    check_limits( p );
    
    return (bigInt) p;
    
}
