#ifndef BLIFPARSE_H
#define BLIFPARSE_H
/*
 * libblifparse - Kevin E. Murray 2016
 *
 * Released under MIT License see LICENSE.txt for details.
 *
 * OVERVIEW
 * --------------------------
 * This library provides support for parsing Berkely Logic Interchange Format (BLIF)
 * files. It supporst the features required to handle basic netlists (e.g. .model, 
 * .inputs, .outputs, .subckt, .names, .latch)
 *
 */
#include <vector>
#include <string>
#include <memory>
#include <limits>
#include <functional>

namespace blifparse {
/*
 * Data structure Forward declarations
 */
struct BlifData;

/*
 * External functions for loading an SDC file
 */
std::shared_ptr<BlifData> blif_parse_filename(std::string filename);
std::shared_ptr<BlifData> blif_parse_filename(const char* filename);
std::shared_ptr<BlifData> blif_parse_file(FILE* blif);


/* 
 * The default blif_error() implementation.
 * By default it prints the error mesage to stderr and exits the program.
 */
void default_blif_error(const int line_number, const std::string& near_text, const std::string& msg);

/*
 * libblifparse calls blif_error() to report errors encountered while parsing an SDC file.  
 *
 * If you wish to define your own error reporting function (e.g. to throw exceptions instead
 * of exit) you can change the behaviour by providing another callable type which matches the signature.
 *
 * The return type is void, while the arguments are:
 *     1) const int line_no            - the file line number
 *     2) const std::string& near_text - the text the parser encountered 'near' the error
 *     3) const std::string& msg       - the error message
 */
void set_blif_error_handler(std::function<void(const int, const std::string&, const std::string&)> new_blif_error_handler);

/*
 * Sentinal values
 */
constexpr double UNINITIALIZED_FLOAT = std::numeric_limits<double>::quiet_NaN();
constexpr int UNINITIALIZED_INT = -1;


/*
 * Enumerations
 */
enum class LogicValue {
    FALSE = 0,  //Logic zero
    TRUE = 1,   //Logic one
    DC,     //Don't care
    UNKOWN  //Unkown (e.g. latch initial state)
};

enum class LatchType {
    FALLING_EDGE,
    RISING_EDGE,
    ACTIVE_HIGH,
    ACTIVE_LOW,
    ASYNCHRONOUS,
    UNSPECIFIED //If no type is specified
};

enum class BlifNodeType {
    NAMES,
    LATCH,
    SUBCKT,
    MODEL
};

enum class NetTermType {
    DRIVER,
    SINK
};

/*
 * Data structures
 */
struct BlifData {

};

} //namespace

#endif
