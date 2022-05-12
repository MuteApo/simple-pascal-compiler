%{
#include <iostream>
#include "tree.hpp"

using namespace std;

extern void yyerror(const char *s);
extern int yylex();
extern int yylineno;

treeNode *root = nullptr;

%}

/***** Standard Identifiers *****/
%token SID_FALSE SID_MAXINT SID_TRUE // Constants
%token SID_BOOLEAN SID_CHAR SID_INTEGER SID_REAL SID_STRING // Types
%token SID_READ SID_WRITE // Built-in Procedures

/**** Symbols ****/
// Special Symbols
%token SYM_ADD SYM_SUB SYM_MUL SYM_DIV // + - * /
%token SYM_EQ SYM_LT SYM_GT SYM_LE SYM_GE SYM_NE // = < > <= >= <>
%token SYM_DOT SYM_COMMA SYM_COL SYM_SEMI SYM_ASSIGN SYM_DDOT // . , : ; := ..
%token SYM_LPAR SYM_RPAR SYM_RSBKT SYM_LSBKT SYM_HAT // ( ) [ ] ^

// Word Symbols (reversed words)
%token WSYM_AND WSYM_ARRAY WSYM_BEGIN WSYM_CASE WSYM_CONST
%token WSYM_DIV WSYM_DO WSYM_DOWNTO WSYM_ELSE WSYM_END
%token WSYM_FOR WSYM_FUNCTION WSYM_GOTO WSYM_IF WSYM_IN WSYM_LABEL
%token WSYM_MOD WSYM_NIL WSYM_NOT WSYM_OF WSYM_OR
%token WSYM_PROCEDURE WSYM_PROGRAM WSYM_RECORD WSYM_REPEAT
%token WSYM_SET WSYM_THEN WSYM_TO WSYM_TYPE
%token WSYM_UNTIL WSYM_VAR WSYM_WHILE WSYM_WITH

%union {
    int ival;
    double dval;
    char *sval;
    treeNode *node;
}

%token <ival> VAL_INT
%token <dval> VAL_REAL
%token <sval> VAL_ID VAL_CHAR VAL_STRING

%type <node> program prog_name_part id compound_expr expr term factor literal
%type <node> var_decl_part var_decl var_list decl_type decl_type_simple
%type <node> stmt_part stmt assign_stmt write_stmt arg_list

%start program

%%

program: prog_name_part WSYM_VAR var_decl_part WSYM_BEGIN stmt_part WSYM_END {
    $$ = new treeNode(DK_Prog, yylineno);
    $$->addChild($1);
    $$->addChild($3);
    $$->addChild($5);
    $$->setValue("root");
    root = $$;
}
;

prog_name_part: WSYM_PROGRAM VAL_ID SYM_SEMI {
    $$ = new treeNode(DK_Prog, yylineno);
    $$->setValue($2);
}
;

var_decl_part: var_decl_part var_decl {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($2);
        $$ = $1;
    } else $$ = $2;
}
| var_decl {
    $$ = $1;
}
;

var_decl: var_list SYM_COL decl_type SYM_SEMI {
    $$ = new treeNode(DK_Var, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}
;

var_list: var_list SYM_COMMA id {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($3);
        $$ = $1;
    } else $$ = $3;
}| id {
    $$ = $1;
};

id: VAL_ID {
    $$ = new treeNode(EK_Id, yylineno);
    $$->setValue($1);
};

decl_type: decl_type_simple {
    $$ = $1;
};

decl_type_simple: SID_INTEGER {
    $$ = new treeNode(TK_Simple, yylineno);
    $$->setExprType(ET_Int);
}| SID_REAL {
    $$ = new treeNode(TK_Simple, yylineno);
    $$->setExprType(ET_Real);
}| SID_CHAR {
    $$ = new treeNode(TK_Simple, yylineno);
    $$->setExprType(ET_Char);
}| SID_BOOLEAN {
    $$ = new treeNode(TK_Simple, yylineno);
    $$->setExprType(ET_Bool);
}| SID_STRING {
    $$ = new treeNode(TK_Simple, yylineno);
    $$->setExprType(ET_String);
};

stmt_part: stmt_part stmt SYM_SEMI {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($2);
        $$ = $1;
    } else $$ = $2;
}| stmt SYM_SEMI {
    $$ = $1;
};

stmt: assign_stmt {
    $$ = $1;
}| write_stmt {
    $$ = $1;
};

assign_stmt: id SYM_ASSIGN compound_expr {
    $$ = new treeNode(SK_Assign, yylineno);
    $$->addChild($1);
    $$->addChild($3);
};

write_stmt: SID_WRITE SYM_LPAR arg_list SYM_RPAR {
    $$ = new treeNode(SK_Write, yylineno);
    $$->addChild($3);
};

arg_list: arg_list SYM_COMMA compound_expr {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($3);
        $$ = $1;
    } else $$ = $3;
}| compound_expr {
    $$ = $1;
};

compound_expr: compound_expr SYM_EQ expr {
    $$ = new treeNode($1, $3, OK_Eq, yylineno);
}| compound_expr SYM_NE expr {
    $$ = new treeNode($1, $3, OK_Ne, yylineno);
}| compound_expr SYM_LT expr {
    $$ = new treeNode($1, $3, OK_Lt, yylineno);
}| compound_expr SYM_GT expr {
    $$ = new treeNode($1, $3, OK_Gt, yylineno);
}| compound_expr SYM_LE expr {
    $$ = new treeNode($1, $3, OK_Le, yylineno);
}| compound_expr SYM_GE expr {
    $$ = new treeNode($1, $3, OK_Ge, yylineno);
}| expr {
    $$ = $1;
};

expr: expr SYM_ADD term {
    $$ = new treeNode($1, $3, OK_Add, yylineno);
}| expr SYM_SUB term {
    $$ = new treeNode($1, $3, OK_Sub, yylineno);
}| expr WSYM_OR term {
    $$ = new treeNode($1, $3, OK_Or, yylineno);
}| term {
    $$ = $1;
};

term: term SYM_MUL factor {
    $$ = new treeNode($1, $3, OK_Mul, yylineno);
}| term SYM_DIV factor {
    $$ = new treeNode($1, $3, OK_Div, yylineno);
}| term WSYM_DIV factor {
    $$ = new treeNode($1, $3, OK_Div, yylineno);
}| term WSYM_MOD factor {
    $$ = new treeNode($1, $3, OK_Mod, yylineno);
}| term WSYM_AND factor {
    $$ = new treeNode($1, $3, OK_And, yylineno);
}| factor {
    $$ = $1;
};

factor: SYM_LPAR compound_expr SYM_RPAR {
    $$ = $2;
}| id {
    $$ = $1;
}| literal {
    $$ = $1;
}| SYM_SUB factor {
    $$ = new treeNode($2, nullptr, OK_Neg, yylineno);
}| WSYM_NOT factor {
    $$ = new treeNode($2, nullptr, OK_Not, yylineno);
};

literal: VAL_INT {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setExprType(ET_Int);
    $$->setValue($1);
}| VAL_REAL {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setExprType(ET_Real);
    $$->setValue($1);
}| VAL_CHAR {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setExprType(ET_Char);
    $$->setValue($1);
}| VAL_STRING {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setExprType(ET_String);
    $$->setValue($1);
}| SID_FALSE {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setExprType(ET_Bool);
    $$->setValue(0);
}| SID_TRUE {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setExprType(ET_Bool);
    $$->setValue(1);
}| SID_MAXINT {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setExprType(ET_Int);
    $$->setValue(2147483647);
};

%%

void yyerror(const char *s){
	std::cerr << s << " | line: " << yylineno << std::endl;
}

int main(){
    yyparse();
    treeNode::travel(root);
    return 0;
}