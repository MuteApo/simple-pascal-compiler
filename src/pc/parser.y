%{
#include "defs.hpp"
#include "tree.hpp"
#include <iostream>

using namespace std;

extern void yyerror(const char *s);
extern int yylex();
extern int yylineno;

int global_uid = 0;
treeNode *root = nullptr;

%}

/***** Standard Ids *****/
%token SID_FALSE SID_MAXINT SID_TRUE // Consts
%token SID_BOOLEAN SID_CHAR SID_INTEGER SID_REAL SID_STRING // Types
%token SID_READ SID_WRITE // Built-in Procs

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
%token WSYM_SET WSYM_SHL WSYM_SHR WSYM_THEN WSYM_TO WSYM_TYPE
%token WSYM_UNTIL WSYM_VAR WSYM_WHILE WSYM_WITH WSYM_XOR

%precedence WSYM_THEN
%precedence WSYM_ELSE

%union {
    int ival;
    double dval;
    char *sval;
    treeNode *node;
}

%token <ival> VAL_INT
%token <dval> VAL_REAL
%token <sval> VAL_ID VAL_CHAR VAL_STRING

%type <node> Program Block Id IdList TypeDefList
%type <node> TypeDefPart TypeDef Type StructTypeDef SetTypeDef ArrayTypeDef StringTypeDef
%type <node> IndexTypeList RecordTypeDef PtrTypeDef OrdTypeDef ResultType
%type <node> BasicRealType VarDeclList VarDeclPart VarDecl
%type <node> VarAccess IndexList Expr Term Factor Item Stmt AssignStmt
%type <node> FuncStmt IfStmt CaseStmt CaseList Case RepeatStmt
%type <node> WhileStmt ForStmt WithStmt CompoundStmt StmtList
%type <node> ConstDefPart ConstDefList ConstDef ConstList
%type <node> SignedLiteral Sign Literal ProcAndFuncDeclPart ProcDecl
%type <node> FuncDecl ArgList Arg ParamList Param

%start Program

%%

Program:  WSYM_PROGRAM Id SYM_SEMI Block SYM_DOT {
    $$ = new treeNode(DK_Prog, yylineno);
    $$->addChild($2);
    $$->addChild($4);
    root = $$;
}| WSYM_PROGRAM Id SYM_LPAR IdList SYM_RPAR SYM_SEMI Block SYM_DOT {
    $$ = new treeNode(DK_Prog, yylineno);
    $$->addChild($2);
    $$->addChild($4);
    $$->addChild($7);
    root = $$;
}

Block: ConstDefPart TypeDefPart VarDeclPart ProcAndFuncDeclPart CompoundStmt {
    $$ = new treeNode(DK_Block, yylineno);
    $$->addChild($1);
    $$->addChild($2);
    $$->addChild($3);
    $$->addChild($4);
    $$->addChild($5);
}

Id: VAL_ID {
    $$ = new treeNode(TK_Id, yylineno);
    $$->setValue($1, TK_Void);
}

IdList: IdList SYM_COMMA Id {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($3);
        $$ = $1;
    } else $$ = $3;
}| Id {
    $$ = $1;
}

/************************* Rules of Type *************************/

TypeDefPart: WSYM_TYPE TypeDefList {
    $$ = $2;
}| {
    $$ = nullptr;
}

TypeDefList: TypeDefList TypeDef SYM_SEMI {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($2);
        $$ = $1;
    } else $$ = $2;
}| TypeDef SYM_SEMI {
    $$ = $1;
}

TypeDef: Id SYM_EQ Type {
    $$ = new treeNode(DK_Type, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}

Type: BasicRealType {
    $$ = $1;
}| OrdTypeDef {
    $$ = $1;
}| StructTypeDef {
    $$ = $1;
}| PtrTypeDef { //TODO
    $$ = nullptr;
}| Id {
    $$ = $1;
}

StructTypeDef:  SetTypeDef{
    $$ = $1;
}| ArrayTypeDef {
    $$ = $1;
}| StringTypeDef {
    $$ = $1;
}| RecordTypeDef {
    $$ = $1;
}

SetTypeDef: WSYM_SET WSYM_OF OrdTypeDef {
    $$ = new treeNode(TK_Set, yylineno);
    $$->addChild($3);
}| WSYM_SET WSYM_OF Id {
    $$ = new treeNode(TK_Set, yylineno);
    $$->addChild($3);
}

ArrayTypeDef: WSYM_ARRAY SYM_LSBKT IndexTypeList SYM_RSBKT WSYM_OF Type {
    $$ = new treeNode(TK_Array, yylineno);
    $$->addChild($3);
    $$->addChild($6);
}

IndexTypeList: IndexTypeList SYM_COMMA Type {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($3);
        $$ = $1;
    } else $$ = $3;
}| Type {
    $$ = $1;
}

StringTypeDef: SID_STRING {
    $$ = new treeNode(TK_String, yylineno);
    treeNode* lb = new treeNode(EK_Literal, yylineno);
    lb->setValue(1, TK_Int);
    $$->addChild(lb);
    treeNode* ub = new treeNode(EK_Literal, yylineno);
    ub->setValue(255, TK_Int);
    $$->addChild(ub);
}| SID_STRING SYM_LSBKT Id SYM_RSBKT {
    $$ = new treeNode(TK_String, yylineno);
    treeNode* lb = new treeNode(EK_Literal, yylineno);
    lb->setValue(1, TK_Int);
    $$->addChild(lb);
    $$->addChild($3);
}| SID_STRING SYM_LSBKT VAL_INT SYM_RSBKT {
    $$ = new treeNode(TK_String, yylineno);
    treeNode* lb = new treeNode(EK_Literal, yylineno);
    lb->setValue(1, TK_Int);
    $$->addChild(lb);
    treeNode* ub = new treeNode(EK_Literal, yylineno);
    ub->setValue($3, TK_Int);
    $$->addChild(ub);
}

RecordTypeDef: WSYM_RECORD VarDeclList WSYM_END {
    $$ = new treeNode(TK_Record, yylineno);
    $$->addChild($2);
}

PtrTypeDef: SYM_HAT BasicRealType { // TODO

}| SYM_HAT Id { // TODO

}

OrdTypeDef: SYM_LPAR ConstList SYM_RPAR {
    $$ = new treeNode(TK_Enum, yylineno);
    $$->addChild($2);
}| Id SYM_DDOT Id {
    $$ = new treeNode(TK_Range, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}| Id SYM_DDOT SignedLiteral {
    $$ = new treeNode(TK_Range, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}| SignedLiteral SYM_DDOT Id {
    $$ = new treeNode(TK_Range, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}| SignedLiteral SYM_DDOT SignedLiteral {
    $$ = new treeNode(TK_Range, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}

ResultType: BasicRealType {
    $$ = $1;
}| Id {
    $$ = $1;
}

BasicRealType: SID_BOOLEAN {
    $$ = new treeNode(TK_Bool, yylineno);
}| SID_INTEGER {
    $$ = new treeNode(TK_Int, yylineno);
}| SID_CHAR {
    $$ = new treeNode(TK_Char, yylineno);
}| SID_REAL {
    $$ = new treeNode(TK_Real, yylineno);
}

/************************* Rules of Var *************************/

VarDeclPart: WSYM_VAR VarDeclList {
    $$ = $2;
}| {
    $$ = nullptr;
}

VarDeclList: VarDeclList VarDecl SYM_SEMI {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($2);
        $$ = $1;
    } else $$ = $2;
}| VarDecl SYM_SEMI {
    $$ = $1;
}

VarDecl: IdList SYM_COL Type{
    $$ = new treeNode(DK_Var, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}

VarAccess: Id SYM_HAT {
    $$ = new treeNode(TK_Ptr, yylineno);
    $$->addChild($1);
}| Id SYM_DOT Id {
    $$ = new treeNode(TK_Range, yylineno);
    $$->addChild($1);
}| Id SYM_LSBKT IndexList SYM_RSBKT {
    $$ = new treeNode(TK_Array, yylineno);
    $$->addChild($1);
    $1->lastSibling()->setSibling($3);
}

IndexList: IndexList SYM_COMMA Expr {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($3);
        $$ = $1;
    } else $$ = $3;
}| Expr {
    $$ = $1;
}

/************************* Rules of Expr *************************/

Expr: Expr SYM_EQ Term {
    $$ = new treeNode($1, $3, EK_Eq, yylineno);
}| Expr SYM_NE Term {
    $$ = new treeNode($1, $3, EK_Ne, yylineno);
}| Expr SYM_GT Term {
    $$ = new treeNode($1, $3, EK_Gt, yylineno);
}| Expr SYM_GE Term {
    $$ = new treeNode($1, $3, EK_Ge, yylineno);
}| Expr SYM_LT Term {
    $$ = new treeNode($1, $3, EK_Lt, yylineno);
}| Expr SYM_LE Term {
    $$ = new treeNode($1, $3, EK_Le, yylineno);
}| Expr WSYM_IN Term {
    $$ = new treeNode($1, $3, EK_In, yylineno);
}| Term {
    $$ = $1;
}

Term: Term SYM_ADD Factor {
    $$ = new treeNode($1, $3, EK_Add, yylineno);
}| Term SYM_SUB Factor {
    $$ = new treeNode($1, $3, EK_Sub, yylineno);
}| Term WSYM_OR Factor {
    $$ = new treeNode($1, $3, EK_Or, yylineno);
}| Term WSYM_XOR Factor {
    $$ = new treeNode($1, $3, EK_Xor, yylineno);
}| Factor {
    $$ = $1;
}

Factor: Factor SYM_MUL Item {
    $$ = new treeNode($1, $3, EK_Mul, yylineno);
}| Factor SYM_DIV Item {
    $$ = new treeNode($1, $3, EK_Div, yylineno);
}| Factor WSYM_DIV Item {
    $$ = new treeNode($1, $3, EK_Div, yylineno);
}| Factor WSYM_MOD Item {
    $$ = new treeNode($1, $3, EK_Mod, yylineno);
}| Factor WSYM_AND Item {
    $$ = new treeNode($1, $3, EK_And, yylineno);
}| Factor WSYM_SHL Item {
    $$ = new treeNode($1, $3, EK_Shl, yylineno);
}| Factor WSYM_SHR Item {
    $$ = new treeNode($1, $3, EK_Shr, yylineno);
}| Item {
    $$ = $1;
}

Item: Literal { 
    $$ = $1;
}| VarAccess {
    $$ = $1;
}| Id {
    $$ = $1;
}| Sign Item {
    $$ = new treeNode($1, $2, EK_Sign, yylineno);
}| WSYM_NOT Item {
    $$ = new treeNode($2, nullptr, EK_Not, yylineno);
}| SYM_LPAR Expr SYM_RPAR {
    $$ = $2;
}| FuncStmt {
    $$ = $1;
}

/************************* Rules of Stmt *************************/

Stmt: CompoundStmt {
    $$ = $1;
}| AssignStmt {
    $$ = $1;
}| FuncStmt {
    $$ = $1;
}| IfStmt {
    $$ = $1;
}| CaseStmt {
    $$ = $1;
}| WhileStmt {
    $$ = $1;
}| RepeatStmt {
    $$ = $1;
}| ForStmt {
    $$ = $1;
}| WithStmt { // TODO
    $$ = nullptr;
}

AssignStmt: Id SYM_ASSIGN Expr {
    $$ = new treeNode(SK_Assign, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}| VarAccess SYM_ASSIGN Expr {
    $$ = new treeNode(SK_Assign, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}

FuncStmt: Id SYM_LPAR ArgList SYM_RPAR {
    $$ = new treeNode(SK_Func, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}| Id SYM_LPAR SYM_RPAR {
    $$ = new treeNode(SK_Func, yylineno);
    $$->addChild($1);
}

IfStmt: WSYM_IF Expr WSYM_THEN Stmt{
    $$ = new treeNode(SK_If, yylineno);
    $$->addChild($2);
    treeNode *if_part = new treeNode(SK_Then, yylineno);
    if_part->addChild($4);
    $$->addChild(if_part);
}| WSYM_IF Expr WSYM_THEN Stmt WSYM_ELSE Stmt {
    $$ = new treeNode(SK_If, yylineno);
    $$->addChild($2);
    treeNode *if_part = new treeNode(SK_Then, yylineno);
    if_part->addChild($4);
    $$->addChild(if_part);
    treeNode *else_part = new treeNode(SK_Else, yylineno);
    else_part->addChild($6);
    $$->addChild(else_part);
}

CaseStmt: WSYM_CASE Expr WSYM_OF CaseList WSYM_END {
    $$ = new treeNode(SK_Switch, yylineno);
    $$->addChild($2);
    $$->addChild($4);
}

CaseList: CaseList Case SYM_SEMI {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($2);
        $$ = $1;
    } else $$ = $2;
}| Case SYM_SEMI {
    $$ = $1;
}

Case: ConstList SYM_COL Stmt {
    $$ = new treeNode(SK_Case, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}

RepeatStmt: WSYM_REPEAT StmtList WSYM_UNTIL Expr {
    $$ = new treeNode(SK_Repeat, yylineno);
    $$->addChild($2);
    $$->addChild($4);
}

WhileStmt: WSYM_WHILE Expr WSYM_DO Stmt {
    $$ = new treeNode(SK_While, yylineno);
    $$->addChild($2);
    $$->addChild($4);
}

ForStmt: WSYM_FOR Id SYM_ASSIGN Expr WSYM_TO Expr WSYM_DO Stmt {
    $$ = new treeNode(SK_For, yylineno);
    $$->addChild($2);
    treeNode* t = new treeNode(EK_To, yylineno);
    t->addChild($4);
    t->addChild($6);
    $$->addChild(t);
    $$->addChild($8);
}| WSYM_FOR Id SYM_ASSIGN Expr WSYM_DOWNTO Expr WSYM_DO Stmt {
    $$ = new treeNode(SK_For, yylineno);
    $$->addChild($2);
    treeNode* t = new treeNode(EK_Downto, yylineno);
    t->addChild($4);
    t->addChild($6);
    $$->addChild(t);
    $$->addChild($8);
}

WithStmt: WSYM_WITH IdList WSYM_DO Stmt {

}

CompoundStmt: WSYM_BEGIN StmtList WSYM_END {
    $$ = new treeNode(SK_Compound, yylineno);
    $$->addChild($2);
}

StmtList: StmtList Stmt SYM_SEMI {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($2);
        $$ = $1;
    } else $$ = $2;
}| {
    $$ = nullptr;
}

/************************* Rules of Const *************************/

ConstDefPart: WSYM_CONST ConstDefList {
    $$ = $2;
}| {
    $$ = nullptr;
}

ConstDefList:  ConstDefList ConstDef SYM_SEMI {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($2);
        $$ = $1;
    } else $$ = $2;
}| ConstDef SYM_SEMI {
    $$ = $1;
}

ConstDef: Id SYM_EQ SignedLiteral {
    $$ = new treeNode(DK_Const, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}| Id SYM_EQ Id {
    $$ = new treeNode(DK_Const, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}

ConstList: ConstList SYM_COMMA Id {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($3);
        $$ = $1;
    } else $$ = $3;
}| ConstList SYM_COMMA SignedLiteral {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($3);
        $$ = $1;
    } else $$ = $3;
}| Id {
    $$ = $1;
}| SignedLiteral {
    $$ = $1;
}

SignedLiteral: Literal {
    $$ = $1;
}| Sign Literal {
    $$ = new treeNode(EK_Sign, yylineno);
    $$->addChild($1);
    $$->addChild($2);
}

Sign: SYM_ADD {
    $$ = new treeNode(EK_Add, yylineno);
    $$->setValue("+", TK_Char);
}| SYM_SUB {
    $$ = new treeNode(EK_Sub, yylineno);
    $$->setValue("-", TK_Char);
}

Literal: VAL_INT {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setValue($1, TK_Int);
}| VAL_REAL {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setValue($1, TK_Real);
}| SID_MAXINT {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setValue(2147483647, TK_Int);
}| SID_TRUE {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setValue(1, TK_Bool);
}| SID_FALSE {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setValue(0, TK_Bool);
}| WSYM_NIL {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setValue(2147483647, TK_Nil);
}| VAL_STRING {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setValue($1, TK_String);
}| VAL_CHAR {
    $$ = new treeNode(EK_Literal, yylineno);
    $$->setValue($1, TK_Char);
}

/************************* Rules of Proc&Stmt *************************/

ProcAndFuncDeclPart: ProcAndFuncDeclPart ProcDecl SYM_SEMI {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($2);
        $$ = $1;
    } else $$ = $2;
}| ProcAndFuncDeclPart FuncDecl SYM_SEMI {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($2);
        $$ = $1;
    } else $$ = $2;
}| {
    $$ = nullptr;
}

ProcDecl: WSYM_PROCEDURE Id SYM_SEMI Block {
    $$ = new treeNode(DK_Proc, yylineno);
    $$->addChild($2);
    $$->addChild($4);
}| WSYM_PROCEDURE Id SYM_LPAR ParamList SYM_RPAR SYM_SEMI Block {
    $$ = new treeNode(DK_Proc, yylineno);
    $$->addChild($2);
    $$->addChild($4);
    $$->addChild($7);
}

FuncDecl: WSYM_FUNCTION Id SYM_COL ResultType SYM_SEMI Block {
    $$ = new treeNode(DK_Func, yylineno);
    $$->addChild($2);
    $$->addChild($4);
    $$->addChild($6);
}| WSYM_FUNCTION Id SYM_LPAR ParamList SYM_RPAR SYM_COL ResultType SYM_SEMI Block {
    $$ = new treeNode(DK_Func, yylineno);
    $$->addChild($2);
    $$->addChild($4);
    $$->addChild($7);
    $$->addChild($9);
}

ArgList: ArgList SYM_COMMA Arg {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($3);
        $$ = $1;
    } else $$ = $3;
}| Arg{
    $$ = $1;
}

Arg: Expr {
    $$ = $1;
}

ParamList: ParamList SYM_SEMI Param {
    if ($1 != nullptr) {
        treeNode* t = $1->lastSibling();
        t->setSibling($3);
        $$ = $1;
    } else $$ = $3;
}| Param {
    $$ = $1;
}

Param: IdList SYM_COL Type {
    $$ = new treeNode(DK_Var, yylineno);
    $$->addChild($1);
    $$->addChild($3);
}| WSYM_VAR IdList SYM_COL Type {
    $$ = new treeNode(DK_Ref, yylineno);
    $$->addChild($2);
    $$->addChild($4);
}

%%

void yyerror(const char *s){
	std::cerr << s << " | line: " << yylineno << std::endl;
}