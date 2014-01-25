%{
/* this is for atof() */
#include <math.h>
/* This %{ ... %} section is code that is glued into the generated scanner. */
#include "parser.h"
#define YYDEBUG 1
extern int outputStage; // This variable is located in vslc.c
    #define RETURN(t) do {                                             \
        if ( outputStage == 1 )                                        \
            fprintf ( stderr, "TOKEN ( %d,\t'%s' )\n", t, yytext );    \
        return t;                                                      \
    } while ( 0 )
    
    
%}

%option array
%option noyywrap
%option yylineno

comment \/\/.*\n

keyword FUNC|PRINT|RETURN|IF|THEN|ELSE|END|WHILE|DO|INT|BOOL|FLOAT|CLASS|HAS|WITH|START|VOID|THIS|NEW

assignment ":="
arithmetric "+"|"-"|"*"|"/"
comparison ">"|"<"|">="|"<="|"=="|"!="
logical "||"|"&&"|"!"
selection "."
operator {assignment}|{arithmetric}|{comparison}|{logical}|{selection}

digit [0-9]

boolean TRUE|FALSE

string \"[^\n]*\"

letter [a-zA-Z_]
identifier {letter}+({letter}|{digit})*

punctuation "("|")"|","|";"

%%
[ \t\n]+                /* eat dem whitespace */
{comment}+              { printf("Comment: %s\n", yytext); }
{keyword}               { printf("Keyword: %s\n", yytext); }
{operator}              { printf("Operator: %s\n", yytext); }
{digit}+                { printf("Integer: %s (%d)\n", yytext, atoi(yytext)); }
{digit}+"."{digit}+     { printf("Float: %s (%g)\n", yytext, atof(yytext)); }
{boolean}               { printf("Boolean value: %s\n", yytext); }
{string}                { printf("String: %s\n", yytext); }
{identifier}            { printf("Identifier: %s\n", yytext); }
{punctuation}           { printf("Punctuation: %s\n", yytext); }
.                       { printf("Unrecognized characer: %s\n", yytext); }
%%

int main() {
    yylex();
}
