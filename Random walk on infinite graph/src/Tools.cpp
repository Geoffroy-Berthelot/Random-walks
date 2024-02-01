/*==========================================================
 * FUNCTIONNAL RESPONSES simulations (Tools)
 * 
 *========================================================*/

#include "Tools.hpp"
#include <limits>

/***************************
 * Load and parse the CSV configuration file (whatever the filename)
 ***************************/
void Tools::loadJSON(std::istream& in, nlohmann::json& j) {
    
    // JSON lib is:
    // https://github.com/nlohmann/json
    in >> j; // load the JSON file
}

double Tools::json_to_double(const nlohmann::json& j) {
    return std::stod(j.get<nlohmann::json::string_t>()); //convert to double
}

int Tools::json_to_int(const nlohmann::json& j) {
    return std::stoi(j.get<nlohmann::json::string_t>());
}

unsigned Tools::json_to_uint(const nlohmann::json& j) {

    std::string s = j.get<nlohmann::json::string_t>();

    if( check_sign( s ) == 1 ) {
        std::cerr << "Error: trying to cast a negative value to an unsigned one." << std::endl;
        std::cerr << "=> make sure each value is > 0." << std::endl;
        throw std::out_of_range("stou");
    } else
        // warning: no range check if cast as an unsigned,
        // also, the type is hardcoded using stoull()
        return (unsigned) std::stoi( s );
}

bigInt Tools::json_to_bigInt(const nlohmann::json& j) {

    std::string s = j.get<nlohmann::json::string_t>();
    
    if( check_sign( s ) == 1 ) {
        std::cerr << "Error: trying to cast a negative value to an unsigned one." << std::endl;
        std::cerr << "=> make sure each value is > 0." << std::endl;
        throw std::out_of_range("stou");
    } else
        // warning: no range check if cast as an unsigned,
        // also, the type is hardcoded using stoull()
        return (bigInt) std::stoull( s );
}

// Check for any '-' sign in the (string) JSON expression:
bool Tools::check_sign(const std::string S) {
    std::size_t found = S.find("-");
    return found != std::string::npos;
}

// print some information directly from a JSON config file:
void Tools::print_info( const nlohmann::json& j ) {
    // print info from the json-file:
    std::cout << "\n___PRINT INFO___\n";
    std::cout << "end_time = " << json_to_double(j["end_time"]) << " (max: " << std::numeric_limits<double>::max() << ")" << std::endl;
    std::cout << "n_sims = " << (unsigned) json_to_int(j["n_sims"]) << std::endl;
    std::cout << "n_cores = " << (unsigned) json_to_int(j["ncores"]) << std::endl;
    std::cout << "dim_lattice = " << (unsigned) json_to_int(j["dim_lattice"]) << std::endl;
    std::cout << "size_lattice = " << json_to_double(j["size_lattice"]) << std::endl;

    // print preys:
    std::cout << "Xs = [";
    bigInt n_preys = 0;
    if(j["Xs"].size() == 1) { // if only one density:
        n_preys = Tools().json_to_bigInt(j["Xs"]);
        std::cout << n_preys;
    } else { // if more densities:
        for(unsigned i=0; i < j["Xs"].size()-1; i++) {
            //std::cout << "j[""Xs""].size() ==>" << j["Xs"].size();
            n_preys = Tools().json_to_bigInt(j["Xs"].at(i));
            std::cout << n_preys << ",";
        }
        std::cout << Tools().json_to_bigInt( j["Xs"].at(j["Xs"].size()-1) );
    }
    std::cout << "] (max: " << std::numeric_limits<unsigned long>::max() << ")" << std::endl;
}

int Tools::fcompare(const double val1, const double val2)
{
    return fabs(val1 - val2) < std::numeric_limits<double>::epsilon();
}

// check whether 'k' is an integer value or not:
bool Tools::is_integer( const double k )
{
    return !( k - trunc(k) );
}



