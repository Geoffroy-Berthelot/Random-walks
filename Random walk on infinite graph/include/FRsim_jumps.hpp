#ifndef RANDOM_WALK_SIM_JUMPS	// avoid repeated expansion
#define RANDOM_WALK_SIM_JUMPS

#include "FRsim_base.hpp"

class FRsim_jumps : public FRsim_base {
public:
    FRsim_jumps(const bigInt, const double,
            const std::shared_ptr<Lattice>, 
            const std::shared_ptr<Walker>,
            const std::shared_ptr<Power_law>);
    void run(bigInt &);  //run the simulation

private:
    const std::shared_ptr<Power_law> PL = NULL;
};

#endif
