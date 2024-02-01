#ifndef RANDOM_WALK_SIM_PERCOLATION	// avoid repeated expansion
#define RANDOM_WALK_SIM_PERCOLATION

#include "FRsim_base.hpp"

class FRsim_perco : public FRsim_base {
public:
    FRsim_perco(const bigInt, const double,
            const std::shared_ptr<Lattice>, 
            const std::shared_ptr<Walker>,
            const double);
    void run(bigInt &);  //run the simulation

private:
    const double p_perco = 0.0;
};

#endif
