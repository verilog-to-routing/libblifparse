#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "blifparse.hpp"
#include "blif_pretty_print.hpp"

using namespace blifparse;

int main(int argc, char **argv) {
  if(argc != 2) {
    fprintf(stderr, "Usage: %s filename.blif\n", argv[0]);
    fprintf(stderr, "\n");
    fprintf(stderr, "Reads in an blif file into internal data structures\n");
    fprintf(stderr, "and then prints it out\n");
    exit(1);
  }

  //Parse the file
  blifparse::BlifPrettyPrinter callback(true);
  blif_parse_filename(argv[1], callback);

  if(callback.had_error()) {
    return 1;
  } else {
    return 0;
  }
}
