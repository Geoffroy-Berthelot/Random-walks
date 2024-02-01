#ifndef RANDOM_WALK_SIM_DRIFT	// avoid repeated expansion
#define RANDOM_WALK_SIM_DRIFT

#include "FRsim_base.hpp"

class FRsim_drift : public FRsim_base {
public:
    FRsim_drift(const bigInt, const double,
            const std::shared_ptr<Lattice>, 
            const std::shared_ptr<Walker>,
            const double);
    void run(bigInt &);  //run the simulation

private:
    const double p_drift = 0.0;
};

#endif
