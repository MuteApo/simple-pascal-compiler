%{
#include "include/defs.hpp"
#include "include/node_const.hpp"
#include "include/node_type.hpp"
#include "include/node_var.hpp"
#include "include/node_func.hpp"
#include "include/node_stmt.hpp"
#include "include/node_expr.hpp"
#include "include/node_block.hpp"
#include <iostream>

using namespace std;

extern void yyerror(const char *s);
extern int yylex();
extern int yylineno;

int global_uid = 0;
ProgramNode *root = nullptr;

%}

%union {
    int ival;
    double dval;
    char cval;
    char *sval;
    ProgramNode *prog_node;
    BlockNode *block_node;
    ConstDefListNode *const_def_list_node;
    ConstDefNode *const_def_node;
    ConstListNode *const_list_node;
    TypeDefListNode *type_def_list_node;
    TypeDefNode *type_def_node;
    TypeAttrListNode *type_attr_list_node;
    TypeAttrNode *type_attr_node;
    BasicAttrNode *basic_attr_node;
    OrdAttrNode *ord_attr_node;
    StructAttrNode *struct_attr_node;
    ArrayAttrNode *array_attr_node;
    RecordAttrNode *record_attr_node;
    PtrAttrNode *ptr_attr_node;
    VarDefListNode *var_def_list_node;
    VarDefNode *var_def_node;
    FuncDefListNode *func_def_list_node;
    FuncDefNode *func_def_node;
    ParamDefListNode *param_def_list_node;
    ParamDefNode *param_def_node;
    StmtListNode *stmt_list_node;
    StmtNode *stmt_node;
    AssignStmtNode *assign_stmt_node;
    IfStmtNode *if_stmt_node;
    ForStmtNode *for_stmt_node;
    WhileStmtNode *while_stmt_node;
    RepeatStmtNode *repeat_stmt_node;
    SwitchStmtNode *switch_stmt_node;
    CaseListNode *case_list_node;
    CaseStmtNode *case_stmt_node;
    FuncStmtNode *func_stmt_node;
    ReadStmtNode *read_stmt_node;
    WriteStmtNode *write_stmt_node;
    ExprListNode *expr_list_node;
    ExprNode *expr_node;
    IdNode *id_node;
    IdListNode *id_list_node;
    FuncNode *func_node;
    ExprEvalType expr_eval_type;
}

/***** Standard Ids *****/
%token SID_FALSE SID_MAXINT SID_TRUE // Consts
%token SID_BOOLEAN SID_CHAR SID_INTEGER SID_REAL SID_STRING // Types
%token SID_READ SID_WRITE SID_WRITELN // Built-in Procs

/**** Symbols ****/
// Special Symbols
%token <expr_eval_type> SYM_ADD SYM_SUB SYM_MUL SYM_DIV // + - * /

%token <expr_eval_type> SYM_EQ SYM_LT SYM_GT SYM_LE SYM_GE SYM_NE // = < > <= >= <>
%token SYM_DOT SYM_COMMA SYM_COL SYM_SEMI SYM_ASSIGN SYM_DDOT // . , : ; := ..
%token SYM_LPAR SYM_RPAR SYM_RSBKT SYM_LSBKT SYM_HAT // ( ) [ ] ^

// Word Symbols (reversed words)
%token <expr_eval_type> WSYM_AND WSYM_DIV WSYM_IN WSYM_MOD WSYM_NOT WSYM_OR WSYM_SHL WSYM_SHR WSYM_XOR
%token WSYM_ARRAY WSYM_BEGIN WSYM_CASE WSYM_CONST
%token WSYM_DO WSYM_DOWNTO WSYM_ELSE WSYM_END
%token WSYM_FOR WSYM_FUNCTION WSYM_IF
%token WSYM_NIL WSYM_OF
%token WSYM_PROCEDURE WSYM_PROGRAM WSYM_RECORD WSYM_REPEAT
%token WSYM_SET WSYM_THEN WSYM_TO WSYM_TYPE
%token WSYM_UNTIL WSYM_VAR WSYM_WHILE WSYM_WITH

%precedence WSYM_THEN
%precedence WSYM_ELSE

%token <ival> VAL_INT
%token <dval> VAL_REAL
%token <sval> VAL_ID VAL_CHAR VAL_STRING


%type <prog_node> Program
%type <block_node> Block
%type <const_def_list_node> ConstDefPart ConstDefList
%type <const_def_node> ConstDef
%type <const_list_node> ConstList
%type <type_def_list_node> TypeDefPart TypeDefList
%type <type_def_node> TypeDef
%type <type_attr_list_node> IndexTypeList
%type <type_attr_node> Type ResultType
%type <basic_attr_node> BasicRealType
%type <ord_attr_node> OrdTypeDef
%type <struct_attr_node> StructTypeDef
%type <array_attr_node> ArrayTypeDef
%type <ptr_attr_node> PtrTypeDef
%type <record_attr_node> RecordTypeDef
%type <var_def_list_node> VarDeclPart VarDeclList VarDecl
%type <func_def_list_node> ProcAndFuncDeclPart
%type <func_def_node> ProcDecl FuncDecl
%type <param_def_list_node> ParamList Param
%type <stmt_list_node> CompoundStmt StmtList
%type <stmt_node> Stmt
%type <assign_stmt_node> AssignStmt
%type <if_stmt_node> IfStmt
%type <for_stmt_node> ForStmt
%type <while_stmt_node> WhileStmt
%type <repeat_stmt_node> RepeatStmt
%type <switch_stmt_node> CaseStmt
%type <case_list_node> CaseList
%type <case_stmt_node> Case
%type <func_stmt_node> FuncStmt ProcStmt
%type <read_stmt_node> ReadStmt
%type <write_stmt_node> WriteStmt
%type <expr_list_node> ArgList IndexList
%type <expr_node> Arg Expr Term Factor Item VarAccess Accessible Id SignedLiteral Literal
%type <id_list_node> IdList
%type <func_node> FuncExpr ProcExpr
%type <expr_eval_type> Sign

%type <type_def_node> SetTypeDef StringTypeDef
%type <stmt_node> WithStmt

%start Program

%%

Program:  WSYM_PROGRAM Id SYM_SEMI Block SYM_DOT {
    $$ = new ProgramNode($2->getIdNode()->getName(), $4);
    $4->setGlobal();
    root = $$;
}| WSYM_PROGRAM Id SYM_LPAR IdList SYM_RPAR SYM_SEMI Block SYM_DOT {
    $$ = new ProgramNode($2->getIdNode()->getName(), $4->getIdList(), $7);
    $7->setGlobal();
    root = $$;
}

Block: ConstDefPart TypeDefPart VarDeclPart ProcAndFuncDeclPart CompoundStmt {
    $$ = new BlockNode(false, $1, $2, $3, $4, $5);
}

Id: VAL_ID {
    $$ = new ExprNode(new IdNode($1));
}

IdList: IdList SYM_COMMA Id {
    $$ = $1;
    $$->addId($3->getIdNode());
}| Id {
    $$ = new IdListNode();
    $$->addId($1->getIdNode());
}

/************************* Rules of Type *************************/

TypeDefPart: WSYM_TYPE TypeDefList {
    $$ = $2;
}| {
    $$ = nullptr;
}

TypeDefList: TypeDefList TypeDef SYM_SEMI {
    $$ = $1;
    $$->addTypeDef($2);
}| TypeDef SYM_SEMI {
    $$ = new TypeDefListNode();
    $$->addTypeDef($1);
}

TypeDef: Id SYM_EQ Type {
    $$ = new TypeDefNode($1->getIdNode()->getName(), $3);
}

Type: BasicRealType {
    $$ = new TypeAttrNode($1);
}| OrdTypeDef {
    $$ = new TypeAttrNode($1);
}| StructTypeDef {
    $$ = new TypeAttrNode($1);
}| PtrTypeDef {
    $$ = new TypeAttrNode($1);
}| Id {
    $$ = new TypeAttrNode($1->getIdNode()->getName());
}

StructTypeDef:  SetTypeDef{
    $$ = nullptr;   // TODO
}| ArrayTypeDef {
    $$ = new StructAttrNode($1);
}| StringTypeDef {
    $$ = nullptr;   // TODO
}| RecordTypeDef {
    $$ = new StructAttrNode($1);
}

SetTypeDef: WSYM_SET WSYM_OF OrdTypeDef {
    $$ = nullptr;    // TODO
}| WSYM_SET WSYM_OF Id {
    $$ = nullptr;    // TODO
}

ArrayTypeDef: WSYM_ARRAY SYM_LSBKT IndexTypeList SYM_RSBKT WSYM_OF Type {
    $$ = new ArrayAttrNode($3, $6);
}

IndexTypeList: IndexTypeList SYM_COMMA Type {
    $$ = $1;
    $$->addTypeAttr($3);
}| Type {
    $$ = new TypeAttrListNode();
    $$->addTypeAttr($1);
}

StringTypeDef: SID_STRING {
    $$ = nullptr;    // TODO
}| SID_STRING SYM_LSBKT Id SYM_RSBKT {
    $$ = nullptr;    // TODO
}| SID_STRING SYM_LSBKT VAL_INT SYM_RSBKT {
    $$ = nullptr;    // TODO
}

RecordTypeDef: WSYM_RECORD VarDeclList WSYM_END {
    $$ = new RecordAttrNode($2);
}

PtrTypeDef: SYM_HAT BasicRealType {
    $$ = new PtrAttrNode(new TypeAttrNode($2));
}| SYM_HAT Id {
    $$ = new PtrAttrNode(new TypeAttrNode($2->getIdNode()->getName()));
}

OrdTypeDef: SYM_LPAR ConstList SYM_RPAR {
    $$ = new OrdAttrNode(new EnumAttrNode($2->getConstList()));
}| Id SYM_DDOT Id {
    $$ = new OrdAttrNode(new SubrangeAttrNode($1, $3));
}| Id SYM_DDOT SignedLiteral {
    $$ = new OrdAttrNode(new SubrangeAttrNode($1, $3));
}| SignedLiteral SYM_DDOT Id {
    $$ = new OrdAttrNode(new SubrangeAttrNode($1, $3));
}| SignedLiteral SYM_DDOT SignedLiteral {
    $$ = new OrdAttrNode(new SubrangeAttrNode($1, $3));
}

ResultType: BasicRealType {
    $$ = new TypeAttrNode($1);
}| Id {
    $$ = new TypeAttrNode($1->getIdNode()->getName());
}

BasicRealType: SID_BOOLEAN {
    $$ = new BasicAttrNode(boolean);
}| SID_INTEGER {
    $$ = new BasicAttrNode(integer);
}| SID_CHAR {
    $$ = new BasicAttrNode(character);
}| SID_REAL {
    $$ = new BasicAttrNode(real);
}

/************************* Rules of Var *************************/

VarDeclPart: WSYM_VAR VarDeclList {
    $$ = $2;
}| {
    $$ = nullptr;
}

VarDeclList: VarDeclList VarDecl SYM_SEMI {
    $$ = $1;
    $$->mergeVarDefList($2);
}| VarDecl SYM_SEMI {
    $$ = $1;
}

VarDecl: IdList SYM_COL Type{
    $$ = new VarDefListNode();
    $$->addVarDef($1, $3);
}

VarAccess: Accessible SYM_HAT {
    $$ = new ExprNode(new VarAccessNode($1));
}| Accessible SYM_LSBKT IndexList SYM_RSBKT {
    $$ = new ExprNode(new VarAccessNode($1, $3));
}| Accessible SYM_DOT Id {
    $$ = new ExprNode(new VarAccessNode($1, $3));
}

Accessible: VarAccess {
    $$ = $1;
}| Id {
    $$ = $1;
}

IndexList: IndexList SYM_COMMA Expr {
    $$ = $1;
    $$->addExpr($3);
}| Expr {
    $$ = new ExprListNode();
    $$->addExpr($1);
}

/************************* Rules of Expr *************************/

Expr: Expr SYM_EQ Term {
    $$ = new ExprNode(EK_Eq, $1, $3);
}| Expr SYM_NE Term {
    $$ = new ExprNode(EK_Ne, $1, $3);
}| Expr SYM_GT Term {
    $$ = new ExprNode(EK_Gt, $1, $3);
}| Expr SYM_GE Term {
    $$ = new ExprNode(EK_Ge, $1, $3);
}| Expr SYM_LT Term {
    $$ = new ExprNode(EK_Lt, $1, $3);
}| Expr SYM_LE Term {
    $$ = new ExprNode(EK_Le, $1, $3);
}| Expr WSYM_IN Term {
    $$ = new ExprNode(EK_In, $1, $3);
}| Term {
    $$ = $1;
}

Term: Term SYM_ADD Factor {
    $$ = new ExprNode(EK_Add, $1, $3);
}| Term SYM_SUB Factor {
    $$ = new ExprNode(EK_Sub, $1, $3);
}| Term WSYM_OR Factor {
    $$ = new ExprNode(EK_Or, $1, $3);
}| Term WSYM_XOR Factor {
    $$ = new ExprNode(EK_Xor, $1, $3);
}| Factor {
    $$ = $1;
}

Factor: Factor SYM_MUL Item {
    $$ = new ExprNode(EK_Mul, $1, $3);
}| Factor SYM_DIV Item {
    $$ = new ExprNode(EK_Fdiv, $1, $3);
}| Factor WSYM_DIV Item {
    $$ = new ExprNode(EK_Div, $1, $3);
}| Factor WSYM_MOD Item {
    $$ = new ExprNode(EK_Mod, $1, $3);
}| Factor WSYM_AND Item {
    $$ = new ExprNode(EK_And, $1, $3);
}| Factor WSYM_SHL Item {
    $$ = new ExprNode(EK_Shl, $1, $3);
}| Factor WSYM_SHR Item {
    $$ = new ExprNode(EK_Shr, $1, $3);
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
    $$ = new ExprNode($1, $2, nullptr); 
}| FuncExpr {
    $$ = new ExprNode($1);
}| WSYM_NOT Item {
    $$ = new ExprNode(EK_Not, $2, nullptr); 
}| SYM_LPAR Expr SYM_RPAR {
    $$ = $2;
}

FuncExpr: Id SYM_LPAR ArgList SYM_RPAR {
    $$ = new FuncNode($1->getIdNode()->getName(), $3);
}| Id SYM_LPAR SYM_RPAR {
    $$ = new FuncNode($1->getIdNode()->getName(), nullptr);
}

ProcExpr: Id {
    $$ = new FuncNode($1->getIdNode()->getName(), nullptr);
}

/************************* Rules of Stmt *************************/

Stmt: CompoundStmt {
    $$ = new StmtNode($1);
}| AssignStmt {
    $$ = new StmtNode($1);
}| FuncStmt {
    $$ = new StmtNode($1);
}| ProcStmt {
    $$ = new StmtNode($1);
}| IfStmt {
    $$ = new StmtNode($1);
}| CaseStmt {
    $$ = new StmtNode($1);
}| WhileStmt {
    $$ = new StmtNode($1);
}| RepeatStmt {
    $$ = new StmtNode($1);
}| ForStmt {
    $$ = new StmtNode($1);
}| WithStmt {
    $$ = nullptr;    // TODO
}| ReadStmt {
    $$ = new StmtNode($1);
}| WriteStmt {
    $$ = new StmtNode($1);
}

AssignStmt: Id SYM_ASSIGN Expr {
    $$ = new AssignStmtNode($1, $3);
}| VarAccess SYM_ASSIGN Expr {
    $$ = new AssignStmtNode($1, $3);
}

FuncStmt: FuncExpr {
    $$ = new FuncStmtNode($1);
}

ProcStmt: ProcExpr {
    $$ = new FuncStmtNode($1);
}

IfStmt: WSYM_IF Expr WSYM_THEN Stmt{
    $$ = new IfStmtNode($2, $4);
}| WSYM_IF Expr WSYM_THEN Stmt WSYM_ELSE Stmt {
    $$ = new IfStmtNode($2, $4, $6);
}

CaseStmt: WSYM_CASE Expr WSYM_OF CaseList WSYM_END {
    $$ = new SwitchStmtNode($2, $4);
}

CaseList: CaseList Case SYM_SEMI {
    $$ = $1;
    $$->addCase($2);
}| Case SYM_SEMI {
    $$ = new CaseListNode();
    $$->addCase($1);
}

Case: ConstList SYM_COL Stmt {
    $$ = new CaseStmtNode($1, $3);
}

RepeatStmt: WSYM_REPEAT StmtList WSYM_UNTIL Expr {
    $$ = new RepeatStmtNode($2, $4);
}

WhileStmt: WSYM_WHILE Expr WSYM_DO Stmt {
    $$ = new WhileStmtNode($2, $4);
}

ForStmt: WSYM_FOR Id SYM_ASSIGN Expr WSYM_TO Expr WSYM_DO Stmt {
    $$ = new ForStmtNode($2->getIdNode()->getName(), true, $4, $6, $8);
}| WSYM_FOR Id SYM_ASSIGN Expr WSYM_DOWNTO Expr WSYM_DO Stmt {
    $$ = new ForStmtNode($2->getIdNode()->getName(), false, $4, $6, $8);
}

WithStmt: WSYM_WITH IdList WSYM_DO Stmt {
    $$ = nullptr;    // TODO
}

ReadStmt: SID_READ SYM_LPAR ArgList SYM_RPAR {
    $$ = new ReadStmtNode($3);
}

WriteStmt: SID_WRITE SYM_LPAR ArgList SYM_RPAR {
    $$ = new WriteStmtNode(false, $3);
}| SID_WRITELN SYM_LPAR ArgList SYM_RPAR {
    $$ = new WriteStmtNode(false, $3);
}

CompoundStmt: WSYM_BEGIN StmtList WSYM_END {
    $$ = $2;
}

StmtList: StmtList Stmt SYM_SEMI {
    $$ = $1 == nullptr ? new StmtListNode() : $1;
    $$->addStmt($2);
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
    $$ = $1;
    $$->addConstDef($2);
}| ConstDef SYM_SEMI {
    $$ = new ConstDefListNode();
    $$->addConstDef($1);
}

ConstDef: Id SYM_EQ SignedLiteral {
    $$ = new ConstDefNode($1->getIdNode()->getName(), $3);
}| Id SYM_EQ Id {
    $$ = new ConstDefNode($1->getIdNode()->getName(), $3);
}

ConstList: ConstList SYM_COMMA Id {
    $$ = $1;
    $$->addConst($3);
}| ConstList SYM_COMMA SignedLiteral {
    $$ = $1;
    $$->addConst($3);
}| Id {
    $$ = new ConstListNode();
    $$->addConst($1);
}| SignedLiteral {
    $$ = new ConstListNode();
    $$->addConst($1);
}

SignedLiteral: Literal {
    $$ = $1;
}| Sign Literal {
    $$ = new ExprNode($1, $2, nullptr); 
}

Sign: SYM_ADD {
    $$ = EK_Add;
}| SYM_SUB {
    $$ = EK_Sub;
}

Literal: VAL_INT {
    $$ = new ExprNode(new LiteralNode($1));
}| VAL_REAL {
    $$ = new ExprNode(new LiteralNode($1));
}| SID_MAXINT {
    $$ = new ExprNode(new LiteralNode(2147483647));
}| SID_TRUE {
    $$ = new ExprNode(new LiteralNode(true));
}| SID_FALSE {
    $$ = new ExprNode(new LiteralNode(false));
}| WSYM_NIL {
    $$ = new ExprNode(new LiteralNode());
}| VAL_STRING {
    $$ = nullptr;   // TODO
}| VAL_CHAR {
    $$ = new ExprNode(new LiteralNode($1[1]));
}

/************************* Rules of Proc&Stmt *************************/

ProcAndFuncDeclPart: ProcAndFuncDeclPart ProcDecl SYM_SEMI {
    $$ = $1 == nullptr ? new FuncDefListNode() : $1;
    $$->addFunc($2);
}| ProcAndFuncDeclPart FuncDecl SYM_SEMI {
    $$ = $1 == nullptr ? new FuncDefListNode() : $1;
    $$->addFunc($2);
}| {
    $$ = nullptr;
}

ProcDecl: WSYM_PROCEDURE Id SYM_SEMI Block {
    $$ = new FuncDefNode($2->getIdNode()->getName(), nullptr, $4);
}| WSYM_PROCEDURE Id SYM_LPAR ParamList SYM_RPAR SYM_SEMI Block {
    $$ = new FuncDefNode($2->getIdNode()->getName(), $4, $7);
}

FuncDecl: WSYM_FUNCTION Id SYM_COL ResultType SYM_SEMI Block {
    $$ = new FuncDefNode($2->getIdNode()->getName(), nullptr, $4, $6);
}| WSYM_FUNCTION Id SYM_LPAR ParamList SYM_RPAR SYM_COL ResultType SYM_SEMI Block {
    $$ = new FuncDefNode($2->getIdNode()->getName(), $4, $7, $9);
}

ArgList: ArgList SYM_COMMA Arg {
    $$ = $1;
    $$->addExpr($3);
}| Arg{
    $$ = new ExprListNode();
    $$->addExpr($1);
}

Arg: Expr {
    $$ = $1;
}

ParamList: ParamList SYM_SEMI Param {
    $$ = $1;
    $$->mergeParamDefList($3);
}| Param {
    $$ = $1;
}

Param: IdList SYM_COL Type {
    $$ = new ParamDefListNode();
    $$->addParamDef(false, $1, $3);
}| WSYM_VAR IdList SYM_COL Type {
    $$ = new ParamDefListNode();
    $$->addParamDef(true, $2, $4);
}

%%

void yyerror(const char *s){
	std::cerr << s << " | line: " << yylineno << std::endl;
}