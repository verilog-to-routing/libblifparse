/* C++ parsers require Bison 3 */
%require "3.0"
%language "C++"

/* Write-out tokens header file */
%defines

/* Use Bison's 'variant' to store values. 
 * This allows us to use non POD types (e.g.
 * with constructors/destrictors), which is
 * not possible with the default mode which
 * uses unions.
 */
%define api.value.type variant

/* 
 * Use the 'complete' symbol type (i.e. variant)
 * in the lexer
 */
%define api.token.constructor

/*
 * Add a prefix the make_* functions used to
 * create the symbols
 */
%define api.token.prefix {TOKEN_}

/*
 * Use a re-entrant (no global vars) parser
 */
/*%define api.pure full*/

/* Wrap everything in our namespace */
%define api.namespace {blifparse}

/* Name the parser class */
%define parser_class_name {Parser}

/* Match the flex prefix */
%define api.prefix {blifparse_}

/* Extra checks for correct usage */
%define parse.assert

/* Enable debugging info */
%define parse.trace

/* Better error reporting */
%define parse.error verbose

/* 
 * Fixes inaccuracy in verbose error reporting.
 * May be slow for some grammars.
 */
/*%define parse.lac full*/

/* Track locations */
/*%locations*/

/* Generate a table of token names */
%token-table

%lex-param {Lexer& lexer}
%parse-param {Lexer& lexer}
%parse-param {std::shared_ptr<BlifData> blif_commands}


%code requires {
    #include <memory>
    #include "blifparse.hpp"
    #include "blif_lexer_fwd.hpp"
}

%code top {
    #include "blif_lexer.hpp"
    //Bison calls blifparse_lex() to get the next token.
    //We use the Lexer class as the interface to the lexer, so we
    //re-defined the function to tell Bison how to get the next token.
    static blifparse::Parser::symbol_type blifparse_lex(blifparse::Lexer& lexer) {
        return lexer.next_token();
    }
}

%{

#include <stdio.h>
#include "assert.h"

#include "blifparse.hpp"
#include "blif_common.hpp"
#include "blif_error.hpp"

using namespace blifparse;

%}

/* declare constant */

%token EOL "end-of-line"
%token EOF 0 "end-of-file"

/* declare variable tokens */
%token <std::string> STRING
%token <std::string> CHAR
%token <double> FLOAT_NUMBER
%token <int> INT_NUMBER

/* declare types */
%type <double> number

%type <std::shared_ptr<BlifData>> blif_data

/* Top level rule */
%start blif_data

%%

blif_data: /*empty*/ {}
    ;

number: FLOAT_NUMBER { $$ = $1; }
    | INT_NUMBER { $$ = $1; }
    ;

%%


void blifparse::Parser::error(const std::string& msg) {
    blif_error_wrap(lexer.lineno(), lexer.text(), msg.c_str());
}
