#ifndef BLIFPARSE_H
#define BLIFPARSE_H
/*
 * libblifparse - Kevin E. Murray 2016
 *
 * Released under MIT License see LICENSE.txt for details.
 *
 * OVERVIEW
 * --------------------------
 * This library provides support for parsing Berkely Logic Interchange Format
 * (BLIF) files. It supporst the features required to handle basic net lists
 * (e.g. .model, .inputs, .outputs, .subckt, .names, .latch)
 *
 * USAGE
 * --------------------------
 * Define a callback derived from the blifparse::Callback interface, and pass it
 * to one of the blifparse::blif_parse_*() functions.
 *
 * The parser will then call the various callback methods as it encounters the
 * appropriate parts of the netlist.
 *
 * See main.cpp and blif_pretty_print.hpp for example usage.
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
  enum class LogicValue;
  enum class LatchType;

  class Callback {
  public:
    virtual ~Callback() {}

    //Start of parsing
    virtual void start_parse() {}

    //Sets current filename
    virtual void filename(std::string /* fname */) {};

    //Sets current line number
    virtual void lineno(int /* line_num */) {};

    //Start of a .model
    virtual void begin_model(std::string /* model_name */) {};

    //.inputs
    virtual void inputs(std::vector<std::string> /* inputs */) {};

    //.outputs
    virtual void outputs(std::vector<std::string> /* outputs */) {};

    //.names
    virtual void names(std::vector<std::string> /* nets */, std::vector<std::vector<LogicValue>> /* so_cover */) {};

    //.latch
    virtual void latch(std::string /* input */, std::string /* output */, LatchType /* type */, std::string /* control */, LogicValue /* init */) {};

    //.subckt
    virtual void subckt(std::string /* model */, std::vector<std::string> /* ports */, std::vector<std::string> /* nets */) {};

    //.blackbox
    virtual void blackbox() {};

    //.end (of a .model)
    virtual void end_model() {};

    //.conn [Extended BLIF : Default throws an error]
    virtual void conn(std::string src, std::string dst);

    //.cname [Extended BLIF : Default throws an error]
    virtual void cname(std::string cell_name);

    //.attr [Extended BLIF : Default throws an error]
    virtual void attr(std::string name, std::string value);

    //.param [Extended BLIF : Default throws an error]
    virtual void param(std::string name, std::string value);

    //End of parsing
    virtual void finish_parse() {};

    //Error during parsing
    virtual void parse_error(const int /* curr_lineno */, const std::string& /* near_text */, const std::string& /* msg */) {};
  };

  /*
   * Overrides the above with a test for parsing errors that is available afterwards
   */
  class ParseErrorCallback : public Callback {
  public:
    void parse_error(const int curr_lineno, const std::string& near_text, const std::string& msg) override {
      fprintf(stderr, "Custom Error at line %d near '%s': %s\n", curr_lineno, near_text.c_str(), msg.c_str());
      had_error_ = true;
    }

    bool had_error() { return had_error_ = true; }

  private:
    bool had_error_ = false;
  };

  /*
   * External functions for loading an SDC file
   */
  void blif_parse_filename(std::string filename, Callback& callback);
  void blif_parse_filename(const char* filename, Callback& callback);

  //Loads from 'blif'. 'filename' only used to pass a filename to callback and can be left unspecified
  void blif_parse_file(FILE* blif, Callback& callback, const char* filename="");

  /*
   * Enumerations
   */
  enum class LogicValue {
    FALSE = 0,  // Logic zero
    TRUE = 1,   // Logic one
    DONT_CARE,  // Don't care
    UNKNOWN     // Unknown (e.g. latch initial state)
  };

  enum class LatchType {
    FALLING_EDGE,
    RISING_EDGE,
    ACTIVE_HIGH,
    ACTIVE_LOW,
    ASYNCHRONOUS,
    UNSPECIFIED  //If no type is specified
  };

} // blifparse

#endif
