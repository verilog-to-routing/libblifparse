%{
    /*
     * Include Files
     */
    #include "blifparse.hpp"
    #include "blif_common.hpp"

    #include "blif_lexer.hpp"

%}

/*
 * Options
 */

/* track line numbers*/
%option yylineno 

/* No lexing accross files */
%option noyywrap

/* unistd.h doesn't exist on windows */
%option nounistd

/* Avoid unused yyunput function warning */
%option nounput

/* isatty() doesn't exist on windows */
%option never-interactive

/* no default rule to echo unrecongaized tokens to output */
%option nodefault

/*%option bison-bridge*/
%option reentrant

/*
 * Use a prefix to avoid name clashes with other
 * flex lexers
 */
%option prefix="blifparse_" 

/*
 * Symbol Definitions
 */
%%
"#".*                           { /* skip comments */ }
[\n\r]+                         { return blifparse::Parser::make_EOL(); }
[ \t]+                          { /* skip white space */ }

[-+]?[0-9]+                     { return blifparse::Parser::make_INT_NUMBER(atoi(blifparse_get_text(yyscanner))); }
[-+]?([0-9]*\.?[0-9]+|[0-9]+\.) { return blifparse::Parser::make_FLOAT_NUMBER(atof(blifparse_get_text(yyscanner))); }
[-a-zA-Z0-9_~|:*\\/]*           { 
                                  return blifparse::Parser::make_STRING(blifparse_get_text(yyscanner)); 
                                }
<<EOF>>                         { /* If the file has no blank line at the end there will
                                     not be the expected EOL following the last command. 
                                     So first time through, return EOL, and subsequently 
                                     return 0 (which indicated end of file). This ensures
                                     there will always be an EOL provided to the parser. 
                                     However it may also generate a stray EOL if the last
                                     line IS blank - so the parse must handle those correclty. */
                                  static bool once; return (once = !once) ? blifparse::Parser::make_EOL() : blifparse::Parser::make_EOF();
                                }
.                               { assert(false); }
%%
