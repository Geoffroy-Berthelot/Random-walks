/*==========================================================
 * RANDOM WALK simulation for Functionnal responses
 * version 1.0 with 2D and nD (infinite) lattices
  *========================================================*/
#include "FRsim_base.hpp"

/*==========================================================
 * FRsim (simulation)
 *
 *========================================================*/
//------------------------------------------------------------------------------
// Constructor with initialization of 'perturbated' simulations
FRsim_base::FRsim_base(const bigInt cur_n, const double end_time,
        const std::shared_ptr<Lattice> lattice, 
        const std::shared_ptr<Walker> walker)
    :L_dim{lattice->get_L_dim()}, n{cur_n}, time_end{end_time},
    W{walker}, L{lattice}
{
    // initialize random generator engine and distributions:
    std::random_device seed;  //random seed.
    std::mt19937_64 generator(seed());
    engine = generator; // set generator

    //helper for printing information about the lattice parameters:
    /*lattice->Print_info(); 
    std::cout << "\n";*/
}

//------------------------------------------------------------------------------
// Distructor
FRsim_base::~FRsim_base()
{}
