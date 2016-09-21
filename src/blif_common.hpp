#ifndef BLIF_COMMON_HPP
#define BLIF_COMMON_HPP

#include "blifparse.hpp"
#include "blif_lexer.hpp"

namespace blifparse {

/*
 * Function Declarations
 */

//utility
char* strdup(const char* src);
char* strndup(const char* src, size_t len);

} //namespace
#endif
