#ifndef RANDOM_WALK_SIM	// avoid repeated expansion
#define RANDOM_WALK_SIM

#include "FRsim_base.hpp"

class FRsim : public FRsim_base {
public:
    FRsim(const bigInt, const double,
            const std::shared_ptr<Lattice>, 
            const std::shared_ptr<Walker>);    //Constructor (alternative)
    void run(bigInt &);  //run the simulation
};

#endif
