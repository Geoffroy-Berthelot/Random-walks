/*==========================================================
 * Walker class
 * version 1.0 full CPP
 *
 *========================================================*/

//#define _USE_MATH_DEFINES //PI value (for old compilers).

#include "Walker.hpp"

//------------------------------------------------------------------------------
// Constructor:
Walker::Walker( const double cur_speed, const double cur_handling, const unsigned L_dim )
    :loc{ new PointND( L_dim ) }, speed{ cur_speed }, handling_time{ cur_handling }
{}

//------------------------------------------------------------------------------
// Distructor
Walker::~Walker() {
    delete( loc );
};

//------------------------------------------------------------------------------
// returns walker's speed:
double Walker::get_speed() const {
    //randomly increases or decreases an indice in one random dimension:
    return speed;
}

//------------------------------------------------------------------------------
// returns walker's handling time:
double Walker::get_handling_time() const {
    //randomly increases or decreases an indice in one random dimension:
    return handling_time;
}

//------------------------------------------------------------------------------
// Move the walker (one step):
void Walker::move( PointND * new_loc ) {
    //randomly increases or decreases an indice in one random dimension:
    loc = new_loc;
}

//------------------------------------------------------------------------------
// Small utility function that prints current walker's position.
void Walker::print_loc() const {
    std::cout << "Walker location=[" << loc << "]" << std::endl ;
}

//------------------------------------------------------------------------------
// Move the walker (one step):
void Walker::move( const int r_dim, const int r_dir ) {
    // r_dim is the dimension (x,y,z) in which the index increase/decrease takes place,
    // r_dir is the direction 
    // randomly increases or decreases an indice in one random dimension:
    if( r_dir == 0 )
        (*loc)[r_dim] += -1;
    else
        (*loc)[r_dim] += 1;
}

//------------------------------------------------------------------------------
// Move the walker (one step of size 'step'):
void Walker::move( const int r_dim, const int r_dir, const unsigned step ) {
    //randomly increases or decreases an indice in one random dimension with step magnitude 'step':
    if( r_dir == 0 ) {
        (*loc)[r_dim] += -(cInt) step;
    }
    else
        (*loc)[r_dim] += step;
}

//------------------------------------------------------------------------------
// Move the walker (one step, with drift):
void Walker::move( const int r_dim, const int r_dir, const char is_drift ) {

    if(is_drift == '0') { //NO DRIFT
        //randomly increases or decreases an indice in one random dimension:
        if( r_dir == 0 )
            (*loc)[r_dim] += -1;
        else
            (*loc)[r_dim] += 1;
    } else { //DRIFTING in the (fist) right direction (arbitrary)
        (*loc)[0] += 1;
    }
}


