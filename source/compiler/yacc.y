%{
#include <iostream>
#include "node.h"

extern "C" {
    void yyerror(const char *s);
    extern int yylex(void);
    extern int line_no;
}
%}

%union {
    int ival;
    double dval;
    char *sval;
    ptrSyntaxNode node;
}

%token <ival> VAL_INT
%token <dval> VAL_REAL
%token <sval> VAL_ID VAL_CHAR VAL_STRING
%token KWD_PROGRAM KWD_VAR KWD_BEGIN KWD_END KWD_WRITE
%token MARK_COLON MARK_SEMI MARK_COMMA MARK_ASSIGN MARK_LP MARK_RP MARK_ADD MARK_MUL
%token TYPE_INT TYPE_REAL TYPE_BOOL TYPE_CHAR TYPE_STRING

%type <node> program prog_name_part id expr term factor literal
%type <node> var_decl_part var_decl var_list decl_type decl_type_simple
%type <node> stmt_part stmt assign_stmt write_stmt arg_list

%start program

%%

program
: prog_name_part KWD_VAR var_decl_part KWD_BEGIN stmt_part KWD_END {
    $$ = $5;
    $$->setAttr($1->getAttr());
}
;

prog_name_part
: KWD_PROGRAM VAL_ID MARK_SEMI {
    $$ = new syntaxNode(DK_Prog, line_no);
    $$->setAttr($2);
}
;

var_decl_part
: var_decl_part var_decl {
    if ($1 != nullptr) {
        ptrSyntaxNode t = $1->lastSibling();
        t->setSibling($2);
        $$ = $1;
    } else $$ = $2;
}
| var_decl {
    $$ = $1;
}
;

var_decl
: var_list MARK_COLON decl_type MARK_SEMI {
    $$ = new syntaxNode(DK_Var, line_no);
    $$->addChild($1);
    $$->addChild($3);
}
;

var_list
: var_list MARK_COMMA id {
    if ($1 != nullptr) {
        ptrSyntaxNode t = $1->lastSibling();
        t->setSibling($3);
        $$ = $1;
    } else $$ = $3;
}
| id {
    $$ = $1;
}
;

id
: VAL_ID {
    $$ = new syntaxNode(EK_Id, line_no);
    $$->setAttr($1);
}
;

decl_type
: decl_type_simple {
    $$ = $1;
}
;

decl_type_simple
: TYPE_INT {
    $$ = new syntaxNode(TK_Simple, line_no);
    $$->setExprType(ET_Int);
}
| TYPE_REAL {
    $$ = new syntaxNode(TK_Simple, line_no);
    $$->setExprType(ET_Real);
}
| TYPE_CHAR {
    $$ = new syntaxNode(TK_Simple, line_no);
    $$->setExprType(ET_Char);
}
| TYPE_BOOL {
    $$ = new syntaxNode(TK_Simple, line_no);
    $$->setExprType(ET_Bool);
}
;

stmt_part
: stmt_part stmt MARK_SEMI {
    if ($1 != nullptr) {
        ptrSyntaxNode t = $1->lastSibling();
        t->setSibling($2);
        $$ = $1;
    } else $$ = $2;
}
| stmt MARK_SEMI {
    $$ = $1;
}
;

stmt
: assign_stmt {
    $$ = $1;
}
| write_stmt {
    $$ = $1;
}
;

assign_stmt
: id MARK_ASSIGN expr {
    $$ = new syntaxNode(SK_Assign, line_no);
    $$->addChild($1);
    $$->addChild($3);
    $$->setAttr(OK_Id);
}
;

write_stmt
: KWD_WRITE MARK_LP arg_list MARK_RP {
    $$ = new syntaxNode(SK_Sys,line_no);
    $$->setAttr(OK_Write);
    $$->addChild($3);
}
;

arg_list
: arg_list MARK_COMMA expr {
    if ($1 != nullptr) {
        ptrSyntaxNode t = $1->lastSibling();
        t->setSibling($3);
        $$ = $1;
    } else $$ = $3;
}
| expr {
    $$ = $1;
}
;

expr
: expr MARK_ADD term {
    $$ = new syntaxNode($1, $3, OK_Add, line_no);
}
| term {
    $$ = $1;
}
;

term
: term MARK_MUL factor {
    $$ = new syntaxNode($1, $3, OK_Mul, line_no);
}
| factor {
    $$ = $1;
}
;

factor
: id {
    $$ = $1;
}
| literal {
    $$ = $1;
}
;

literal
: VAL_INT {
    $$ = new syntaxNode(EK_Literal, line_no);
    $$->setExprType(ET_Int);
    $$->setAttr($1);
}
;

%%

void yyerror(const char *s)
{
	std::cerr << s << " | line: " << line_no << std::endl;
}

int main() {
    return yyparse();
}