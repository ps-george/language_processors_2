%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "histogram.hpp"

/* End the embedded code section. */
%}


%%

\"(\\.|[^"])*\" { yylval.wordValue = new std::string(yytext); return Quoted; }

[+-]?([0-9]+[.]?)[0-9]* { fprintf(stderr, "Number"); yylval.numberValue = std::stod(yytext); return Number; }

[a-z]+ { yylval.wordValue = new std::string(yytext); return Word; }

[ \t\n]

[^\x00-\x7F]

.

%%

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  exit(1);
}
