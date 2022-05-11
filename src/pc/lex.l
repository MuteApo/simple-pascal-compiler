%{
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "node.h"
#include "yacc.tab.h"
int line_no = 1;

extern "C"
{
    int yywrap(void);
    int yylex(void);
}

%}

QUOTE "'"

%%

program return KWD_PROGRAM;
begin return KWD_BEGIN;
end return KWD_END;
var return KWD_VAR;

write return KWD_WRITE;

integer return TYPE_INT;
real return TYPE_REAL;
char return TYPE_CHAR;
string return TYPE_STRING;
boolean return TYPE_BOOL;

":" return MARK_COLON;
";" return MARK_SEMI;
"," return MARK_COMMA;
":=" return MARK_ASSIGN;
"(" return MARK_LP;
")" return MARK_RP;
"+" return MARK_ADD;
"-" return MARK_SUB;
"*" return MARK_MUL;
div return MARK_DIV;
mod return MARK_MOD;

false return VAL_FALSE;
true return VAL_TRUE;
maxint return VAL_MAXINT;

[a-zA-Z]([a-zA-Z0-9_])* {
	yylval.sval = (char*)malloc(strlen(yytext)+1);
	strcpy(yylval.sval,yytext); 
    return VAL_ID;
}

[0-9]+ {
    yylval.ival = atoi(yytext);
    return VAL_INT;
}

[0-9]+"."[0-9]+ {
    yylval.dval = atof(yytext);
    return VAL_REAL;
}

{QUOTE}[^{QUOTE}]{QUOTE} {
    yylval.sval = yytext;
    return VAL_CHAR;
}

{QUOTE}(^{QUOTE}|'')*{QUOTE} {
    yylval.sval = yytext;
    return VAL_STRING;
}

[\t\f] {}

"\n" { line_no++; }

" " {}

. {
    fprintf(stderr, "'%c' (0%o): illegal charcter at line %d\n",
              yytext[0], yytext[0], line_no); }
 
%%

int yywrap ()
{
	printf("token finish(line: %d<<\n", line_no);
    return 1;
}