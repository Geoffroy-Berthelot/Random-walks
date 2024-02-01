# Random walk on infinite graph in 1, 2 or 3 dimensions
This is the C++ code used for simulating random walks in 1, 2 or 3 dimensional graph or latttices using CPUs only.

Please refer to the following article for more information:

(*i*) Vincent Bansaye, Geoffroy Berthelot, Amina El Bachari, Jean-René Chazottes, and Sylvain Billiard. Stochasticity in foraging explains large and invariant fluctuations in consumption rates within and across species (submitted)

## Usage
1. Compil the C++ project using: ```make frsim``` (see 'makefile' and 'makefile.complete')
2. Either create or generate a JSON configuration file (see details about the configuration file in the 'JSON configuration file' section below). 
A matlab file 'gen_json.mat' is provided to help generate a proper JSON file.
3. Make sure a results/ folder is already created before launching the simulations.
4. Run the simulations of random walks using: ```./frsim ./your_configuration_file``` for non-perturbated random walks.  
Perturbations can be used:  
```./frsim ./your_configuration_file -drift``` will make a random walk with a drift, according to the drift parameter $p_d$  
```./frsim ./your_configuration_file -jumps``` a random walk with jumps, where jumps are drawn for power-law distribution with parameter $\gamma$  
```./frsim ./your_configuration_file -memory``` the walker have a memory, and will avoid the last visited site  
```./frsim ./your_configuration_file -percolation``` a random walk on a percolated graph, according to the percolation parameter $p$  
```./frsim -help``` will display the help.  

## JSON Configuration file
A JSON file should be provided in order to use the simulation. This JSON file contains almost all parameters for properly simulating random walks:


Examples of correct JSON configuration files are:
```
{"ncores":"3","n_sims":"10000","dim_lattice":"2","end_time":"100000000000","walk_speed":"1","walk_handling_time":"0.1","size_lattice":"1000","Xs":"100","power_law_gamma":"2.5","power_law_xmin":"1","power_law_xmax":"10","p_drift":"0.1","p_perco":"0.5"}
```
or  

## Additional hardcoded parameters

