#ifndef CONFIG_TYPES			// avoid repeated expansion
#define CONFIG_TYPES

// This file defines:
// (*) the types used in the simulation: bigInt, cInt, N0_1D, N0_2D, N0_3D
// (*) the precision output 

//***************
//** Definition of bigInt, cInt types
//***************
// (unsigned type) bigInt: used for the indexes in 'is_visited' array.
// Indexes are natural numbers. (unsigned) long int type ranges from 0 to 4,294,967,295 (32 bits),
// Use unsigned long long int if needed (ranges in 0 to 18,446,744,073,709,551,615 (64 bits))
// please check bigInt Tools::json_to_bigInt() as there is no range check for the cast there,
// and a (too) small type will causes a crash (also check 
using bigInt = unsigned long long int;

// (signed type) cInt: used for the positions in the lattice. 
// Positions are (signed) integers (set of integers Z). 
// (signed) long int type ranges from -2,147,483,648 to 2,147,483,647 (32 bits),
// use long long int if needed (range in -(2^63) to (2^63)-1, 64 bits)
using cInt = long long int;

//***************
//** Precision of output to file
//***************
// Will define the precision of the numbers in the filename of the file containnig the results
const static unsigned output_precision = 2;

//***************
//** Initial number of nodes (N0) in the lattice: N0_1D, N0_2D, N0_3D
//***************
// N0 is the initial number of nodes of the lattice, with respect to a given dimention 'd' or 'D'.
// This number should be odd as:
// - the walker starts at the center node of the lattice.
// - each side is at equal distance (in terms of nodes) of the center cell
// A ''small'' N0 value and a ''large'' 't' will value need lesser memory but will require reallocation (see Lattice::check_new_node()),
// while a ''large'' N0 value will require more memory at the initialization but might avoid a call to (see Lattice::check_new_node()), 
// depending on 't' and '\delta'.
// Please note that this parameter can strongly affect the performance (ie. running time) of the simulation.
// ---- 1D ----
// Initial number of nodes for a 1-dimensional walk:
const static bigInt N0_1D = 10001;
// ---- 2D ----
// Initial number of nodes for a 2-dimensional walk:
// 'L_dim_size' (see Lattice.hpp) is the number of elements (nodes) in one side of the lattice.
// In two dimensions, L_dim_size = 51 nodes ( sqrt(2601) = 51 ),
const static bigInt N0_2D = 2601;
// const static bigInt N0_2D = 100020001; //a higher value of N0 for 2 dimensions.
// ---- 3D ----
// Initial number of nodes for a 3-dimensional walk,
// which gives a lattice side 'L_dim_size' of 101 nodes ( 3rd root of (132651) = 101 ),
const static bigInt N0_3D = 1030301;
// this can be further generalized to larger dimensions using nth roots.

#endif
