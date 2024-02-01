#ifndef WALKER  // avoid repeated expansion
#define WALKER

#include <json.hpp>
#include <sstream>
#include "PointND.hpp"

class Walker {
public:
    PointND *loc = NULL; //current location

    Walker( const double, const double, const unsigned ); //Constructor
    void move( PointND * );
    void move( const int, const int );
    void move( const int, const int, const unsigned );
    void move( const int, const int, const char );
    void print_loc() const;
    double get_speed() const;
    double get_handling_time() const;
    ~Walker(); //Destructor

private:
    const double speed = 0.0;           // walker's speed
    const double handling_time = 0.0;   // walker's handling time
};

#endif
