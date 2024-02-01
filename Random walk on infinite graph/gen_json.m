% Generate the JSON file for the random walk in {1,2,3} dimensions

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% JSON file generation parameters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
precision = 10; %number of digits we allow for real values

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% COMPUTER and SIMULATION parameters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n_cores = 30;               %number of cores to use
n_sims = 1e6;               %number of simulations to run. 
dim = 3;                    %dimension of lattice $L$ (should be an integer)
end_time = 1e7;             %ending simulation time (in seconds)
walk_speed = 1.015;           %average speed of the walker in m.s-1
walk_handling_time = 0.0113500015;   %handling time (in s.)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Core parameters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
L = 1e3;            %'size' of lattice. Example for dim=2, L is the size of one side of the lattice
n_nodes = 10;       %number of densities to want to test \in [L, max_nodes]
min_nodes = L;      %min number of nodes
max_nodes = 10000;  %max number of nodes (ie. preys) in the lattice (must be squared)

% We only want x = n^dim values:
V = linspace(L, max_nodes, n_nodes); %true values
Xs = unique( fix( nthroot(V, dim) ).^dim ); %fix X values that does not respect: x = n^dim
% The spacing between nodes is L ./ (Xs.^(1/dim) -1)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Parameters for perturbated models:
%% Additional parameters for testing the robustness of the model
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Power-law parameters that follow P(k) ~ k^-gamma with k \in [x_{min},
%%% x_{max}]
power_law_gamma = 2.5; %gamma value for the power law (jumps)
power_law_xmin = 1.0; %x_min
power_law_xmax = 3.0; %x_max

%%% Drift probability: p \in [0,1]
p_drift = 0.1; %proba to drift is p(->) = (1+3*p_drift)/4 and not to drift is p(|) = p(<-) = p(v) = (1-p_drift)-4

%%% percolation probability: p \in [0,1]
p_perco = 0.5; %proba to find a prey on the node (1 = always, 0 = never)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Encode to strings and generate JSON file
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fprintf(1, 'encoding JSON file...');
encoding_c = strcat({'%.'}, string(precision), {'f'});
Init.ncores = string( n_cores );              %number of cores to use
Init.n_sims = string( n_sims );               %number of simulations to run. (500)
Init.dim_lattice =  string( dim );            %dimension of lattice $L$.
Init.end_time = sprintf(encoding_c, end_time);   %ending simulation time (in seconds) : 1 day. 100e3
Init.walk_speed = sprintf(encoding_c, walk_speed);   %average speed of the predator in m.s-1
Init.walk_handling_time = sprintf(encoding_c, walk_handling_time);   %handling time
Init.size_lattice = sprintf(encoding_c, L);   %size of lattice $L$
Init.Xs = string( Xs );

%additional Inits:
%---Power law
Init.power_law_gamma = string(power_law_gamma); %gamma value for the power law (jumps)
Init.power_law_xmin = string(power_law_xmin); %x_min
Init.power_law_xmax = string(power_law_xmax); %x_max
%---Drift
Init.p_drift = string(p_drift);
%---Percolation
Init.p_perco = string(p_perco);

JSON = jsonencode(Init);
fprintf(1, 'ok\n');

fprintf(1, 'exporting JSON file...');
fid = fopen('Config.json', 'w');
if fid == -1
     error('Cannot create JSON file');
end
fwrite(fid, JSON, 'char');
fclose(fid);
fprintf(1, 'ok\n');

