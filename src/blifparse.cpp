#include <cstdio>

#include "blifparse.hpp"

#include "blif_common.hpp"
#include "blif_lexer.hpp"
#include "blif_error.hpp"


namespace blifparse {

/*
 * Given a filename parses the file as an BLIF file
 * and returns a pointer to a struct containing all
 * the blif commands.  See blif.h for data structure
 * detials.
 */
std::shared_ptr<BlifData> blif_parse_filename(std::string filename) {
    return blif_parse_filename(filename.c_str());
}

std::shared_ptr<BlifData> blif_parse_filename(const char* filename) {
    std::shared_ptr<BlifData> blif_data;

    FILE* infile = std::fopen(filename, "r");
    if(infile != NULL) {
        blif_data = blif_parse_file(infile);
        std::fclose(infile);
    } else {
        std::fclose(infile);
        blif_error_wrap(0, "", "Could not open file %s.\n", filename);
    }

    return blif_data;
}

std::shared_ptr<BlifData> blif_parse_file(FILE* blif_file) {

    //Initialize the lexer
    Lexer lexer(blif_file);

    auto blif_data = std::make_shared<BlifData>();

    //Setup the parser + lexer
    Parser parser(lexer, blif_data);

    //Do the parse
    int error = parser.parse();
    if(error) {
        blif_error_wrap(0, "", "BLIF Error: file failed to parse!\n");
    }

    return blif_data;
}

/*
 * Error handling
 */
void default_blif_error(const int line_no, const std::string& near_text, const std::string& msg) {
    fprintf(stderr, "BLIF Error line %d near '%s': %s\n", line_no, near_text.c_str(), msg.c_str());
    exit(1);
}

void set_blif_error_handler(std::function<void(const int, const std::string&, const std::string&)> new_blif_error_handler) {
    blif_error = new_blif_error_handler;
}

}
