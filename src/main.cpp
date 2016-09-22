#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "blifparse.hpp"
#include "blif_pretty_print.hpp"

using namespace blifparse;

int exit_code = 0;

void custom_blif_error(const int lineno, const std::string& near_text, const std::string& msg);

int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s filename.blif\n", argv[0]);
        fprintf(stderr, "\n");
        fprintf(stderr, "Reads in an blif file into internal data structures\n");
        fprintf(stderr, "and then prints it out\n");
        exit(1);
    }

    //Use custom error handling
    blifparse::set_blif_error_handler(custom_blif_error);

    //Parse the file
    blifparse::BlifPrettyPrinter pretty_printer;
    blif_parse_filename(argv[1], pretty_printer);
    return exit_code;
}

void custom_blif_error(const int lineno, const std::string& near_text, const std::string& msg) {
    fprintf(stderr, "Custom Error at line %d near '%s': %s\n", lineno, near_text.c_str(), msg.c_str());
    exit_code = 1;
}

