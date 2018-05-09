%{
#include "node.h"
#include "parser.tab.h"
#include<bits/stdc++.h>
extern union Node yylval;
%}

%%
[ \t\n\s]		{ /* Do nothing */ }
"declblock"				return DECLBLOCK;
"codeblock"				return CODEBLOCK;
"int"							{yylval.str=strdup(yytext);return SPECIFIER;}
"for"							return FOR;
"while"						return WHILE;
"goto"						return GOTO;
"print"						{yylval.str=strdup(yytext);return PRINT;}
"println"					{yylval.str=strdup(yytext);return PRINTLN;}
"read" 						return READ;
"if"							return IF;
"else"						return ELSE;
"||"							{yylval.str=strdup(yytext);return OR;}
"&&"							{yylval.str=strdup(yytext);return AND;}
\".*\" 						{yylval.str=strdup(yytext);return STRING;}
[0-9][0-9]*				{yylval.value=std::atoi(strdup(yytext));return NUMBER;}
[a-zA-Z][a-zA-Z0-9]* {yylval.str=strdup(yytext);return IDENTIFIER;}
"+"		{yylval.str=strdup(yytext);return ADD;}
"-"		{yylval.str=strdup(yytext);return SUB;}
"*"		{yylval.str=strdup(yytext);return MULT;}
"/"		{yylval.str=strdup(yytext);return DIV;}
";"		return ';';
","		return ',';
":"		return ':';
"("		return '(';
")"		return ')';
"{"		return '{';
"}"		return '}';
"="		return '=';
"["		return '[';
"]"		return ']';
"=="	{yylval.str=strdup(yytext);return EQ;}
"<"		{yylval.str=strdup(yytext);return LT;}
">"		{yylval.str=strdup(yytext);return GT;}
"<="	{yylval.str=strdup(yytext);return LTEQ;}
">="	{yylval.str=strdup(yytext);return GTEQ;}
"!="	{yylval.str=strdup(yytext);return NTEQ;}
.		{
		  printf("Unexpected token encountered: %s\n", yytext);
		  return ETOK;
		}
