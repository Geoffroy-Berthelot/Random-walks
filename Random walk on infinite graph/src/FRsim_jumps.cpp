/*==========================================================
 * RANDOM WALK simulation for Functionnal responses
 * version 1.0 with 2D and nD (infinite) lattices
  *========================================================*/
#include "FRsim_jumps.hpp"

/*==========================================================
 * FRsim (simulation) with jumps
 *
 *========================================================*/
//------------------------------------------------------------------------------
// Constructor with initialization of 'perturbated' simulations
FRsim_jumps::FRsim_jumps(const bigInt cur_n, const double end_time,
        const std::shared_ptr<Lattice> lattice, 
        const std::shared_ptr<Walker> walker,
        const std::shared_ptr<Power_law> PowerLaw)
    :FRsim_base( cur_n, end_time, lattice, walker ), PL{ PowerLaw }
{}

//------------------------------------------------------------------------------
// Run the simulation (n-Dimensional random walk, with no perturbations):
void FRsim_jumps::run(bigInt &cpt) {

    double T = 0.0;   // initial time

    /*****************************************************
     * SIMULATION RUN
     *****************************************************/
    // initialize random distributions
    std::uniform_int_distribution<> rd_dir(0, 1);           //for the direction sign (+/-).
    std::uniform_int_distribution<> rd_dim(0, L_dim-1);     //for the dimension
    std::uniform_real_distribution<> rd_U(0, 1);

    // compute the time taken to reach one node at each time step:
    const double dt = L->get_L_space() / W->get_speed();

    // set initial position of walker------------
    L->set_center_pos( W->loc );
    
    unsigned step_size = 0;

    while(T <= time_end) {
        // check if node (prey) is currently visited and
        // if not, mark it as visited, otherwise do nothing
        if(L->check_new_node_jump( W->loc ) == true) { //this is a new node
            cpt++; //only increase the interaction counter
            T += W->get_handling_time(); //add handling time
        }

        // move walker to another proximal node, first draw direction 'dir' and 'dimension',
        // and check boundaries:
        int r_dim = rd_dim(engine);
        int r_dir = rd_dir(engine);

        // draw step size, according to the parameters ot the PL distrubtion (see PL)
        step_size = PL->rand( rd_U(engine) ); //get step size from discrete power law distribution
     
        // then move to new loc:
        W->move( r_dim, r_dir, step_size );

        // increase time taken to move to new loc (implicit conversion from unsigned to double)
        T += dt * step_size;    
    }
}
