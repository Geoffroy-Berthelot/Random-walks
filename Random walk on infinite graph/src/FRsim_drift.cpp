/*==========================================================
 * RANDOM WALK simulation for Functionnal responses
 * version 1.0 with 2D and nD (infinite) lattices
  *========================================================*/
#include "FRsim_drift.hpp"

/*==========================================================
 * FRsim (simulation) with drift
 *
 *========================================================*/
//------------------------------------------------------------------------------
// Constructor with initialization of 'perturbated' simulations
FRsim_drift::FRsim_drift(const bigInt cur_n, const double end_time,
        const std::shared_ptr<Lattice> lattice, 
        const std::shared_ptr<Walker> walker,
        const double proba_drift)
    :FRsim_base( cur_n, end_time, lattice, walker ), p_drift{ proba_drift }
{}

//------------------------------------------------------------------------------
// Run the simulation (n-Dimensional random walk, with drift):
//------------------------------------------------------------------------------
void FRsim_drift::run(bigInt &cpt) {   
    double T = 0.0;         // initial time
    char is_drift = '0';    // does the walker drift? set to '0' (no drift)
    
    /*****************************************************
     * SIMULATION RUN
     *****************************************************/
    // initialize random distributions
    std::uniform_int_distribution<> rd_dir(0, 1);           //for the direction sign (+/-)
    std::uniform_int_distribution<> rd_dim(0, L_dim-1);     //for the dimension
    std::uniform_real_distribution<> rd_p(0.0, 1.0);         //for the drift probability
    
    // compute the time taken to reach a node at each time step:
    const double dt = L->get_L_space() / W->get_speed();

    // set initial position of walker------------
    L->set_center_pos( W->loc );
    
    while(T <= time_end) {
        // check if node (prey) is currently visited and
        // if not, mark it as visited, otherwise do nothing
        if(L->check_new_node( W->loc ) == true) { //this is a new node
            cpt++; //only increase the interaction counter
            T += W->get_handling_time(); //add handling time
        }
        
        // move walker to another proximal node, first draw direction 'dir' and 'dimension',
        // and check boundaries:
        int r_dim = rd_dim(engine);
        int r_dir = rd_dir(engine);
        
        is_drift = '0'; //reset is_drift to '0' (no drift effect)
        if( rd_p(engine) <= p_drift ) //pick a random value and test against p_drift
            is_drift = '1'; //if sucessfull then produce a drift

        // then move to new loc:
        W->move( r_dim, r_dir, is_drift);
        T += dt;    //increase time taken to move to new loc:
    }
}


