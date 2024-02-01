#ifndef CONFIG_PARAMS			// avoid repeated expansion
#define CONFIG_PARAMS

#include "Tools.hpp"

struct Config_params
{
    //------------------------------------------------------------------------------
    // Constructor
    Config_params( const unsigned p_n_sims, const unsigned p_n_cores, 
            const unsigned p_L_dim, const double p_L_size, const double p_end_time, 
            const double p_walk_speed, const double p_walk_time, 
            const double p_p_drift, const double p_x_min_t, 
            const double p_x_max_t, const double p_gamma, 
            const double p_p_perco, const std::vector<bigInt> Xs )
    :n_sims{ p_n_sims }, n_cores{ p_n_cores }, L_dim{ p_L_dim }, L_size{ p_L_size }, 
    end_time{ p_end_time }, walk_speed{ p_walk_speed }, walk_handling_time{ p_walk_time }, 
    p_drift{ p_p_drift }, x_min_t{ p_x_min_t }, x_max_t{ p_x_max_t }, gamma{ p_gamma },
    p_perco{ p_p_perco }, N0s { Xs }
    {}
    
    //------------------------------------------------------------------------------
    // Destructor
    ~Config_params() {}    
    
    //------------------------------------------------------------------------------
    // Check for incorrect values of the parameters
    void assert_params_values() {
        
        // Assert each parameter is in the correct range of values ------------------
        // (LATTICE PARAMETERS)
        if( L_dim < 1 || L_dim > 3 ) {
            std::cerr << "Error: dim_lattice = " << L_dim << std::endl;
            std::cerr << "=> make sure the number of dimensions is an integer in [1, 3]." << std::endl;
            exit(EXIT_FAILURE);
        }
        
        if( L_size < 0 || L_size == 0 ) {
            std::cerr << "Error: The size of the lattice is <= 0 (L_size = " << L_size << ")" << std::endl;
            std::cerr << "=> the size of the lattice should be positive." << std::endl;
            exit(EXIT_FAILURE);
        }
        
        // (WALKER SPEED AND HANDLING TIME)
        if( walk_speed <= 0 ) {
            std::cerr << "Error: walker's speed = " << walk_speed << std::endl;
            std::cerr << "=> make sure the walker's speed is greater than 0." << std::endl;
            exit(EXIT_FAILURE);
        }
        
        if( walk_handling_time < 0 ) {
            std::cerr << "Error: walker's handling time = " << walk_handling_time << std::endl;
            std::cerr << "=> make sure the walker's handling time is equal or greater than 0 ." << std::endl;
            exit(EXIT_FAILURE);
        }

        // (Values of Xs (initial number of nodes in the lattice) )  ------------------
        for (auto i = N0s.cbegin(); i != N0s.cend(); ++i) {
            if( *i == 0 ) {
                std::cerr << "Error: Xs value " << *i << " must be greater than 0" << std::endl;
                std::cerr << "=> make sure each value of Xs is > 0." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        
        // Init additional parameters for "perturbated versions" of the simulations ------------------
        // (DRIFT)
        if( p_drift < 0 || p_drift > 1  ) {
            std::cerr << "Error: p_drift = " << p_drift << std::endl;
            std::cerr << "=> make sure p_drift is in [0, 1]." << std::endl;
            exit(EXIT_FAILURE);
        }
        
        // (JUMPS)
        if( x_min_t < 0 ) {
            std::cerr << "Error: x_min = " << x_min_t << std::endl;
            std::cerr << "=> make sure x_min is greater than 0." << std::endl;
            exit(EXIT_FAILURE);
        }
        if( x_max_t < 0 ) {
            std::cerr << "Error: x_max = " << x_max_t << std::endl;
            std::cerr << "=> make sure x_max is greater than 0." << std::endl;
            exit(EXIT_FAILURE);
        }
        if( x_max_t < x_min_t ) {
            std::cerr << "Error: x_min = " << x_min_t << ", xmax = " << x_max_t << std::endl;
            std::cerr << "=> make sure x_min < x_max." << std::endl;
            exit(EXIT_FAILURE);
        }
       
        if( Tools().is_integer( x_min_t ) == false ) {
            std::cerr << "Error: x_min = " << x_min_t << std::endl;
            std::cerr << "=> make sure x_min is an integer value." << std::endl;
            exit(EXIT_FAILURE);
        }

        if( Tools().is_integer( x_max_t ) == false ) {
            std::cerr << "Error: x_max = " << x_min_t << std::endl;
            std::cerr << "=> make sure x_max is an integer value." << std::endl;
            exit(EXIT_FAILURE);
        }
        
        // (PERCOLATION)
        if( p_perco < 0 || p_perco > 1  ) {
            std::cerr << "Error: p_perco = " << p_perco << std::endl;
            std::cerr << "=> make sure p_perco is in [0, 1]." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    //------------------------------------------------------------------------------
    // Print parameters values to console:
    void print_info() {
        std::cout << "\n___PRINT INFO___\n";
        std::cout << "n_sims = " << n_sims << std::endl;
        std::cout << "n_cores = " << n_cores << std::endl;
        std::cout << "dim_lattice = " << L_dim << std::endl;
        std::cout << "size_lattice = " << L_size << std::endl;
        std::cout << "end_time = " << end_time << " (max: " << std::numeric_limits<double>::max() << ")" << std::endl;        
        std::cout << "walker speed = " << walk_speed << std::endl;
        std::cout << "walker handling time = " << walk_handling_time << std::endl;
        std::cout << "drift = " << p_drift << std::endl;
        std::cout << "percolation = " << p_perco << std::endl;
        std::cout << "x_min = " << x_min_t << std::endl;
        std::cout << "x_max = " << x_max_t << std::endl;
        std::cout << "gamma = " << gamma << std::endl;
        
        // Print N0s values:
        std::cout << "X = [ ";
        for ( const int& i : N0s )
            std::cout << i << " ";
        std::cout << "]" << std::endl;

    }
    
    const unsigned n_sims = 1; //number of simulations
    const unsigned n_cores = 1; //number of cores to use
    const unsigned L_dim = 2; //dimension of the lattice
    const double L_size = 1000.0; //value of 'L'
    const double end_time = 1000.0; //finale time value
    const double walk_speed = 1.0; //walker speed
    const double walk_handling_time = 1.0; //walker's hendling time
    const double p_drift = 0.0; //value of drift
    const double x_min_t = 1.0; //min jump value (will be cast as an integer value in main_FRsim.cpp)
    const double x_max_t = 10.0; //max jump value (will be cast as an integer value in main_FRsim.cpp)
    const double gamma = 2.0; //gamma value for the power law
    const double p_perco = 0.0; //percolation value
    const std::vector<bigInt> N0s; //vector containing the initial number of nodes (N0) for each simulation
};

#endif