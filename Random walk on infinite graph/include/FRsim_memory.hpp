#ifndef RANDOM_WALK_SIM_MEMORY	// avoid repeated expansion
#define RANDOM_WALK_SIM_MEMORY

#include "FRsim_base.hpp"

class FRsim_memory : public FRsim_base {
public:
    FRsim_memory(const bigInt, const double,
            const std::shared_ptr<Lattice>, 
            const std::shared_ptr<Walker>);
    void run(bigInt &);  //run the simulation

private:
    // Helper function to know what is the current direction of the walker.
    // Encode/map random dimension and direction to a char value following the current diagram (1D, 2D, 3D):
    //        1
    //        ^  6
    //        | /
    //        |/
    // 4 <----0----> 2
    //       /|
    //      / |
    //     5  v
    //        3
    static char get_cur_dir( const int r_dim, const int r_dir ) {
        
        // Encode the motion of the walker in one, two or three dimensions
        if( r_dim == 0 && r_dir == 0)
            return '4'; // left (1D, 2D, 3D)
        
        if( r_dim == 0 && r_dir == 1)
            return '2'; // right (1D, 2D, 3D)
        
        if( r_dim == 1 && r_dir == 0)
            return '3'; // down (2D, 3D)
        
        if( r_dim == 1 && r_dir == 1)
            return '1'; // up (2D, 3D)

        if( r_dim == 2 && r_dir == 0)
            return '5'; // forward (3D) 

        if( r_dim == 2 && r_dir == 1)
            return '6'; // back (3D)
        
        // otherwise returns 0
        return '0';
    }
    
    // Encode/map the 'complementary' to a given direction 'cdir'
    static char get_cur_dir_comp( const char cdir ) {
        
        switch( cdir ) {
        case '1': return '3';
        case '3': return '1';
        case '2': return '4';
        case '4': return '2';
        case '5': return '6';
        case '6': return '5';
        default : return '0';
        }
    }
};

#endif
