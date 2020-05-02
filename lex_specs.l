
%{

// need header for the call to exit() in lex.yy.c.
#include <stdlib.h>
#include <io.h>
#include "lex_specs.h"

#define isatty	_isatty
#define fileno	_fileno

%}

%option yylineno

DIGIT						[0-9]
ID							[a-zA-Z_][a-zA-Z_0-9]*

%%

"SET"						return SET;
"GET"						return GET;
=							return ASSIGN;
"ADD"						return ADDITION;
"SUB"						return SUBTRACTION;
"EXIT"						return EXIT;
,							return COMMA;
{ID}						return IDENTIFIER;
(-)?{DIGIT}+				return INTEGER;
[ \t]						;
\n							return NEWLINE;

"//".*                      { 
								/* 
								Single Line Comment Processing.
								The regular expression associated with this rule means : 
								on encountering a token that begins with "//" followed by 
								zero or more characters except '\n'.
								*/			
								/* Do nothing. Just consume comment */ 
							}

"##".*                      { 
								/*
								Another Single Line Comment Processing.
								The regular expression associated with this rule means :
								on encountering a token that begins with "##" followed by 
								zero or more characters except '\n'.
								*/
								/* Do nothing. Just consume comment */ 
							}

.							{ 
								printf("Unexpected Text [%s].\n", yytext); 
								return SYNTAX_ERROR;
							}

%%

int yywrap(void)
{
	return 1;
}

void SkipAllTokensInLine()
{
	while (yylex() != NEWLINE);
}

