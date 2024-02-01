# Random walk on infinite graph in 1, 2 or 3 dimensions
This is the C++ code used for simulating random walks in 1, 2 or 3 dimensional graph (or latttices) using CPUs only.

Please refer to the following article for more information:

(*i*) Vincent Bansaye, Geoffroy Berthelot, Amina El Bachari, Jean-René Chazottes, and Sylvain Billiard. Stochasticity in foraging explains large and invariant fluctuations in consumption rates within and across species (submitted)

## Preamble
This program simulates random walks in an homogeneous graph (or lattice). The walker travels throught edges and visits nodes. At each new (unvisited) node/site the walker pauses for duration $c$, which can be set to 0.
The distance $y$ between two nodes 

## Usage
1. Compil the C++ project using: ```make frsim``` (see 'makefile' and 'makefile.complete')
2. Either create or generate a JSON configuration file (see details about the configuration file in the 'JSON configuration file' section below). 
A matlab file 'gen_json.mat' is provided to help generate a proper JSON file.
3. Make sure a results/ folder is already created before launching the simulations.
4. Run the simulations of random walks using: ```./frsim ./your_configuration_file``` for non-perturbated random walks.  
Perturbated random walk can also be simulated using:  
```./frsim ./your_configuration_file -drift``` will make a random walk with a drift, according to the drift parameter $p_d$  
```./frsim ./your_configuration_file -jumps``` a random walk with jumps, where jumps are drawn from a discrete and truncated power-law distribution with parameter $\gamma$  
```./frsim ./your_configuration_file -memory``` the walker have a memory, and will avoid the last visited site  
```./frsim ./your_configuration_file -percolation``` a random walk on a percolated graph, according to the percolation parameter $p$  
```./frsim -help``` will display the help.  

## JSON Configuration file
A JSON file should be provided in order to use the simulation. This JSON file contains almost all parameters for properly simulating random walks:
* ```ncores``` is the number of cores to use for the simulation,
* ```n_sims``` is the number of (independant) simulated random walls,
* ```dim_lattice``` is the dimension of the lattice, with values in {1, 2, 3},
* ```end_time``` is the ending time of the random walk. The walker walks through the graph, and traveling through an edge (ie. do one step) takes time $\Delta$,
* ```walk_speed``` is the walker speed,
* ```walk_handling_time``` is the 'waiting time' at each (new) node/site. The walker stops for a duration $c$ at each new, unvisited node/site,
* ```size_lattice``` is the size of the graph/lattice,
* ```Xs``` can be a vector of values, reprensenting the (initial) number of nodes in the lattice. The walk is operated on an infinite graph/lattice,
but $\Delta$ depends on this value 
travelling time $\Delta$ through a node,
* ```power_law_gamma```: value of the $\gamma$ parameter for the power-law distribution (jumps)
* ```power_law_xmin``` the minimal step value the walker can do (jumps)
* ```power_law_xmax``` the maximal step value the walker can do (jumps)
* ```p_drift```: the value of drift $p_d$
* ```p_perco```: the value of percolation $p$


Examples of correct JSON configuration files are:
```
{"ncores":"3","n_sims":"10000","dim_lattice":"2","end_time":"100000000000","walk_speed":"1","walk_handling_time":"0.1","size_lattice":"1000","Xs":"100","power_law_gamma":"2.5","power_law_xmin":"1","power_law_xmax":"10","p_drift":"0.1","p_perco":"0.5"}
```
(one graph/lattice density value ```Xs```)
```
{"ncores":"30","n_sims":"100000","dim_lattice":"2","end_time":"1000000","walk_speed":"1","walk_handling_time":"0.1","size_lattice":"1000","Xs":["961","2500","10000","4900","400","225"],"power_law_gamma":"3.5","power_law_xmin":"1","power_law_xmax":"10","p_drift":"0.1","p_perco":"0.5"}
```
(mulitple graph/lattice density values ```Xs```)

## Additional hardcoded parameters
The following parameters are hardcoded in the 'Config_type.hpp' header file:
* ```bigInt``` : the specific (unsigned) type of the variable which holds the index of a node. This can be huge, depending on the simulation time for example.
* ```cInt``` : the specific (signed) type of the variable which holds the (integer) coordinate of a node (in the Z subset).
* ```output_precision``` : the precision of the numeric values written in the filename
* ```N0_1D``` : The number of nodes of the initial (at time $t=0$) of the graph/lattice in 1 dimension. 
* ```N0_2D``` : The number of nodes of the initial (at time $t=0$) of the graph/lattice in 2 dimensions.
* ```N0_3D``` : The number of nodes of the initial (at time $t=0$) of the graph/lattice in 3 dimensions.
Bear in mind that these values can have a strong impact for running time performance.


## Technical notes:
* needs C++14
* the memory is implemented in a loosy way, where we only keep the past direction (instead of position), and avoid repeating the complementary one. 
This can be improved by implementing a memory to the Walker Class, for instance a vector containing a sequence of the immediate past (visited) $n$ positions.
One should note that if $n$ is large (depending on the dimension) then the walker can get 'stuck' or 'surrounded' between past visited positions, preventing it from moving (as the walk with memory avoids memorised positions).