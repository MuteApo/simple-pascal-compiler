%{
#include <iostream>
#include "node.h"

syntaxNode *root = nullptr;

extern "C" {
    extern void yyerror(const char *s);
    extern int yylex(void);
    extern int line_no;
}
%}

%union {
    int ival;
    double dval;
    char *sval;
    syntaxNode *node;
}

%token <ival> VAL_INT
%token <dval> VAL_REAL
%token <sval> VAL_ID VAL_CHAR VAL_STRING
%token VAL_FALSE VAL_TRUE VAL_MAXINT
%token KWD_PROGRAM KWD_VAR KWD_BEGIN KWD_END KWD_WRITE
%token MARK_COLON MARK_SEMI MARK_COMMA MARK_ASSIGN MARK_LP MARK_RP
%token MARK_ADD MARK_SUB MARK_MUL MARK_DIV MARK_MOD
%token TYPE_INT TYPE_REAL TYPE_BOOL TYPE_CHAR TYPE_STRING

%type <node> program prog_name_part id expr term factor literal
%type <node> var_decl_part var_decl var_list decl_type decl_type_simple
%type <node> stmt_part stmt assign_stmt write_stmt arg_list

%start program

%%

program
: prog_name_part KWD_VAR var_decl_part KWD_BEGIN stmt_part KWD_END {
    $$ = $5;
    $$->setValue($1->getValue()._sval);
    root = $$;
}
;

prog_name_part
: KWD_PROGRAM VAL_ID MARK_SEMI {
    $$ = new syntaxNode(DK_Prog, line_no);
    $$->setValue($2);
}
;

var_decl_part
: var_decl_part var_decl {
    if ($1 != nullptr) {
        syntaxNode* t = $1->lastSibling();
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
        syntaxNode* t = $1->lastSibling();
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
    $$->setValue($1);
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
        syntaxNode* t = $1->lastSibling();
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
}
;

write_stmt
: KWD_WRITE MARK_LP arg_list MARK_RP {
    $$ = new syntaxNode(SK_Write, line_no);
    $$->addChild($3);
}
;

arg_list
: arg_list MARK_COMMA expr {
    if ($1 != nullptr) {
        syntaxNode* t = $1->lastSibling();
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
| expr MARK_SUB term {
    $$ = new syntaxNode($1, $3, OK_Sub, line_no);
}
| term {
    $$ = $1;
}
;

term
: term MARK_MUL factor {
    $$ = new syntaxNode($1, $3, OK_Mul, line_no);
}
| term MARK_DIV factor {
    $$ = new syntaxNode($1, $3, OK_Div, line_no);
}
| term MARK_MOD factor {
    $$ = new syntaxNode($1, $3, OK_Mod, line_no);
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
| MARK_SUB factor {
    $$ = new syntaxNode($2, nullptr, OK_Neg, line_no);
}
;

literal
: VAL_INT {
    $$ = new syntaxNode(EK_Literal, line_no);
    $$->setExprType(ET_Int);
    $$->setValue($1);
}
| VAL_REAL {
    $$ = new syntaxNode(EK_Literal, line_no);
    $$->setExprType(ET_Real);
    $$->setValue($1);
}
| VAL_CHAR {
    $$ = new syntaxNode(EK_Literal, line_no);
    $$->setExprType(ET_Char);
    $$->setValue($1);
}
| VAL_STRING {
    $$ = new syntaxNode(EK_Literal, line_no);
    $$->setExprType(ET_String);
    $$->setValue($1);
}
| VAL_FALSE {
    $$ = new syntaxNode(EK_Literal, line_no);
    $$->setExprType(ET_Bool);
    $$->setValue(0);
}
| VAL_TRUE {
    $$ = new syntaxNode(EK_Literal, line_no);
    $$->setExprType(ET_Bool);
    $$->setValue(1);
}
| VAL_MAXINT {
    $$ = new syntaxNode(EK_Literal, line_no);
    $$->setExprType(ET_Int);
    $$->setValue(2147483647);
}
;

%%

void yyerror(const char *s) {
	std::cerr << s << " | line: " << line_no << std::endl;
}

int main() {
    yyparse();
    syntaxNode::travel(root);
    return 0;
}