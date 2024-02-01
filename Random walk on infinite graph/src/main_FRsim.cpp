/*==========================================================
 * RANDOM WALK simulation on infinite graph/lattice
 * version 1.0 with in 1, 2 or 3 dimensions including perturbations (drift, etc.)
 *
 * Copyright (C) <2024> Vincent Bansaye, Geoffroy Berthelot, Amina El Bachari, 
 * Jean-René Chazottes, and Sylvain Billiard
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *========================================================*/
#include <sstream>
#include <iomanip>
#include <chrono>
#include <sys/stat.h>
#include "FRsim.hpp"
#include "FRsim_base.hpp"
#include "FRsim_drift.hpp"
#include "FRsim_jumps.hpp"
#include "FRsim_memory.hpp"
#include "FRsim_perco.hpp"
#include "Tools.hpp"
#include "PointND.hpp"
#include "Config_params.hpp"

using namespace std::chrono;

namespace
{
    void print_usage() {
        std::cout << "Usage is: ./frsim ./Config_file.json -perturbation" << std::endl;
        std::cout << "Where 'Config_file.json' is the JSON configuration file" << std::endl;
        std::cout << "and '-perturbation' is one of the following perturbation:" << std::endl;
        std::cout << "-drift : random walk with drift" << std::endl;
        std::cout << "-memory : random walk with memory (past site only)" << std::endl;
        std::cout << "-jumps : random walk with jumps" << std::endl;
        std::cout << "-percolation : random walk in a percolated lattice" << std::endl << std::endl;
        std::cout << "Examples:" << std::endl;
        std::cout << "./frsim ./Config.Json : typical random walk using 'Config.Json' without any perturbation" << std::endl;
        std::cout << "./frsim ./Config.Json -memory : random walk using 'Config.Json' and a memory" << std::endl;
        std::cout << "./frsim ./Config.Json -drift : random walk using 'Config.Json' with a drift" << std::endl;
    }
}

/*********************************
 * MAIN
 *********************************/
int main( int argc, char *argv[] ) {
    
    // initialize timer
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    std::cout << "==== Random walks - FRsim (V1) ====" << std::endl;
    
    if( argc < 2 || argc > 3 ) {
        std::cerr << "Error: wrong number of arguments." << std::endl;
        std::cerr << "Type /frsim -help for usage." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // check for the existence of /results folder in the project's directory:
    const char* dir = "results/";
    struct stat sb; //metadata
 
    if( stat(dir, &sb) != 0 ) {
        std::cerr << "Error: Please create a results/ folder in the project's directory." << std::endl;
        std::cerr << "The results will then be exported in this folder." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // parse the arguments:
    std::vector<std::string> args(argv, argv+argc);
    
    if ( args[1] == "-help" ) {
        print_usage();
        return EXIT_SUCCESS;
    }
    
    // try to access the configuration file:
    std::ifstream in( argv[1] );
    if( !in ) {
        std::cerr << "Error: configuration file not found." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // then check for additional options, if any:
    int what_perturb = 0; // =0 means that this is a standard random walk.
    if( argc == 3 ) {
        // read the additional option
        if( args[2] == "-drift" )
            what_perturb = 1; // 1 is a drifted random walk
        else if( args[2] == "-jumps" )
            what_perturb = 2; // 2 is a random walk with jumps
        else if( args[2] == "-memory" )
            what_perturb = 3; // 3 is a random walk with memory
        else if( args[2] == "-percolation" )
            what_perturb = 4; // 4 is a standard random walk in a percolated lattice
        else {
            std::cerr << "Error: wrong option." << std::endl;
            std::cerr << "Type /frsim -help for usage." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    
    std::cout << "loading/parsing configuration file \"" << argv[1] << "\"...";
    nlohmann::json JSON_parse; //structure that will hold all parameters for each simulation:
    Tools().loadJSON(in, JSON_parse); //load to 'JSON_parse'
    std::cout << "ok\n";
    
    // ---- Collect N0 data ----
    // note:
    // JSON_parse["Xs"].get< std::vector<char> >() 
    // may fail here, thus let's use:
    std::vector<bigInt> V;
    V.reserve( JSON_parse["Xs"].size() );
    
    if( JSON_parse["Xs"].size() == 1 ) // if only one N0 value (alt. one can use .is_array())
        V.push_back( Tools().json_to_bigInt( JSON_parse["Xs"] ) );
    else // if multiple N0 values, cast them as 'bigInt' type:
        for(unsigned i=0; i < JSON_parse["Xs"].size(); i++)
            V.push_back( Tools().json_to_bigInt( JSON_parse["Xs"].at(i) ) );
    
    // ---- Assign to config ----
    // note:
    // V is passed by value and the lifetime of V is in the main() function,
    // while the lifetime of 'N0s' (member of Config_params) is shorter.
    // Another option is to pass by reference (aggreation).
    Config_params config(
            Tools().json_to_uint( JSON_parse["n_sims"] ),
            Tools().json_to_uint( JSON_parse["ncores"] ),
            Tools().json_to_uint( JSON_parse["dim_lattice"] ),
            Tools().json_to_double( JSON_parse["size_lattice"] ),
            Tools().json_to_double( JSON_parse["end_time"] ),
            Tools().json_to_double( JSON_parse["walk_speed"] ),
            Tools().json_to_double( JSON_parse["walk_handling_time"] ),
            Tools().json_to_double( JSON_parse["p_drift"] ),
            Tools().json_to_double( JSON_parse["power_law_xmin"] ),
            Tools().json_to_double( JSON_parse["power_law_xmax"] ),
            Tools().json_to_double( JSON_parse["power_law_gamma"] ),
            Tools().json_to_double( JSON_parse["p_perco"] ),
            V );
    
    std::cout << "checking parameters values...";
    config.assert_params_values(); //assert each parameter value is in the expected range of values.
    std::cout << "ok\n";
    
    // printing information and parameters, parsed from the configuration file:
    config.print_info();
    // or, alternatively, printing the parameters using Tools:
    // Tools().print_info( JSON_parse ); //print 'config' values
    std::cout << "\n";
    
    // ---- Additional initializations ----
    // create temporary array for holding results in summary configuration:
    const unsigned N_nodeconf = JSON_parse["Xs"].size();
    std::vector<bigInt> sum_nnodes;
    std::vector<bigInt> sum_nvisited;
    // set the number of elements in each vector using reserve():
    const size_t vector_size = config.n_sims * N_nodeconf; // total number of elements
    std::cout << "total size = " << vector_size << std::endl;
    sum_nnodes.reserve( vector_size );
    sum_nvisited.reserve( vector_size );
    
    // Init lattice (depending on dimension) ------------------
    bigInt n_nodes = 0; //number of nodes required in the configfile
    bigInt N0 = 0; //number of nodes in the lattice at the start of the simulation (see 'config.hpp')
    
    // config.assert_params_values() are ok, we can now convert to (unsigned) type:
    const unsigned x_max = (unsigned) config.x_max_t;
    const unsigned x_min = (unsigned) config.x_min_t;
    std::shared_ptr<Power_law> PL{ std::make_shared<Power_law>( x_min, x_max, config.gamma ) };
    
#pragma omp parallel num_threads( config.n_cores )
    {
        for(unsigned i=0; i < N_nodeconf; i++) {
            
            // For this number of nodes configuration ------------------
            
            // Set the current number of nodes (or density X)
            // which also defines the distance between nodes ('y')
            // and thus the time taken to travel between two nodes ('\Delta').
            n_nodes = config.N0s.at(i);
            
            // Assign N0 (the initial/starting number of nodes in the lattice)
            // depending on the dimension L_dim and the number of preys 'n_nodes'
            // see 'config.hpp' fo the definition of N0_1D, N0_2D and N0_3D:
            if( config.L_dim == 1 )
                N0 = N0_1D;
            else if ( config.L_dim == 2 )
                N0 = N0_2D;
            else if ( config.L_dim == 3 )
                N0 = N0_3D;
            
#pragma omp for
            for(unsigned n_s=0; n_s < config.n_sims; n_s++) {
                
                // Init walker ------------------
                // Init central position of the walker:
                std::shared_ptr<Walker> W{ std::make_shared<Walker>(
                        config.walk_speed,
                        config.walk_handling_time,
                        config.L_dim) };
                        
                // Print simulation number ------------------
                std::stringstream stream;
                stream << "running simulation " << n_s+1 << " (n=" << n_nodes << ")" << std::endl;
                std::cout << stream.str(); //avoir interleaving when printing to std::cout using multiple threads
                        
                // Initializing the simulation ------------------
                bigInt cpt = 0; //set the number of discovered preys / new sites
                        
                // Initialize lattice ------------------
                std::shared_ptr<Lattice> L = NULL;
                        
                if( config.L_dim == 2 )
                    // if dimension == 2, then we implement a spiral indexation, which is faster
                    L = std::make_shared<Lattice_spiral>(n_nodes, N0, config.L_size, config.L_dim, config.p_perco, x_max);
                else
                    // Otherwise we use the standard lattice
                    L = std::make_shared<Lattice_baseconversion>(n_nodes, N0, config.L_size, config.L_dim, config.p_perco, x_max);
                        
                // Create the simulation ------------------
                FRsim_base * sim;
                        
                // Encoding is the following:
                switch( what_perturb ) {
                    case 0 : { // 0 : standard random walk (no perturbation)
                        sim = new FRsim(n_nodes, config.end_time, L, W);
                        break; }
                    case 1 : { // 1 is a drifted random walk
                        sim = new FRsim_drift(n_nodes, config.end_time, L, W, config.p_drift);
                        break; }
                    case 2 : { // 2 is a random walk with jumps
                        sim = new FRsim_jumps(n_nodes, config.end_time, L, W, PL);
                        break; }
                    case 3 : { // 3 is a random walk with memory
                        sim = new FRsim_memory(n_nodes, config.end_time, L, W);
                        break; }
                    case 4 : { // 4 is a standard random walk in a percolated lattice
                        sim = new FRsim_perco(n_nodes, config.end_time, L, W, config.p_perco);
                        break; }
                    default: { //otherwise: standard random walk (no perturbation)
                        sim = new FRsim(n_nodes, config.end_time, L, W);
                        break; }
                    }
                
                // Run the simulation (depending on the simulation type) ------------------
                sim->run(cpt);
                
                // Append concurrently to a single array:
                // critical because of push_back(), that reallocates the vector as its size increases
#pragma omp critical
                {
                    sum_nnodes.push_back(n_nodes); //total number of nodes (ie. preys)
                    sum_nvisited.push_back(cpt);   //total number of visited nodes (ie. interactions)
                }
                
                // Delete current simulation  ------------------
                delete sim;
            }
        }
    }
    
    // defines (filename) output format for numbers:
    std::stringstream stream;
    stream << std::fixed << std::setprecision(output_precision) << config.L_size;
    std::string size_L = stream.str();
    stream.str(""); stream.clear();
    
    stream << std::fixed << std::setprecision(output_precision) << config.end_time;
    std::string total_time = stream.str();
    stream.str(""); stream.clear();
    
    stream << std::fixed << std::setprecision(output_precision) << config.walk_handling_time;
    std::string handling_time = stream.str();
    stream.str(""); stream.clear();
    
    const std::string filename = "results/res_dim=" +
            std::to_string( config.L_dim ) +
            std::string("_sizeL=") + size_L +
            std::string("_t=") + total_time +
            std::string("_th=") + handling_time +
            std::string(".txt");
    std::ofstream res_file;
    res_file.open( filename );
    
    // Then write results to file:
    // (push vector elements to file):
    for (unsigned i = 0; i < sum_nnodes.size(); ++i)
        res_file << sum_nnodes[i] << "," << sum_nvisited[i] << "\r\n";
    
    res_file.close();
    
    if ( ! res_file ) {
        std::cerr << "Error: cannot write the result file for some reason." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<seconds>( t2 - t1 ).count();
    std::cout << "Duration is: " << duration << " seconds\n";
    
    return EXIT_SUCCESS;
}
