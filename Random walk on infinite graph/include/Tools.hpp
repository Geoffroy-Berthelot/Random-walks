#ifndef _TOOLS_
#define _TOOLS_

#include <iostream>
#include <fstream>
#include <json.hpp>
#include <cmath>
#include "config_types.hpp"

/***********************************
 * Class TOOLS()
 * contains functions for:
 * - csv import
 * - options parsing
 * - csv to arma conversion
 * - print data information
 * - checking integers
 ***********************************/

class Tools
{

public:
    static void loadJSON(std::istream&, nlohmann::json&);
    static double json_to_double(const nlohmann::json&);
    static int json_to_int(const nlohmann::json&);
    static unsigned json_to_uint(const nlohmann::json&);
    static bigInt json_to_bigInt(const nlohmann::json&);
    static void print_info(const nlohmann::json&);
    static int fcompare(const double, const double);
    static bool is_integer( const double );

private:
    static bool check_sign(const std::string); 
};

#endif // _TOOLS_H_

