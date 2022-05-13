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

%type <node> Program Block Identifier IdentifierList TypeDefinitionList
%type <node> TypeDefinitionPart TypeDefinition Type StructuredTypeDef SetTypeDef ArrayTypeDef
%type <node> IndexTypeList RecordTypeDef FieldList PointerTypeDef OrdinalTypeDef ResultType
%type <node> BasicRealType VariableDeclarationList VariableDeclarationPart VariableDeclaration
%type <node> VariableAccess IndexList Expression Term Factor Item Statement AssignStatement
%type <node> ProcedureStatement IfStatement CaseStatement CaseList Case RepeatStatement
%type <node> WhileStatement ForStatement WithStatement CompoundStatement StatementList
%type <node> ConstantDefinitionPart ConstantDefinitionList ConstantDefinition ConstantList
%type <node> SignedLiteral Sign Literal ProcAndFuncDeclarationPart ProcedureDeclaration
%type <node> FunctionDeclaration ActualParameterList ActualParameter FormalParameterList FormalParameter

%start Program

%%

Program:  WSYM_PROGRAM Identifier SYM_SEMI Block{

}| WSYM_PROGRAM Identifier SYM_LPAR IdentifierList SYM_RPAR SYM_SEMI Block{

}

Block: ConstantDefinitionPart TypeDefinitionPart VariableDeclarationPart ProcAndFuncDeclarationPart CompoundStatement {

}

Identifier: VAL_ID {

}

IdentifierList: IdentifierList SYM_COMMA Identifier {

}| Identifier {

}

/************************* Rules of Type *************************/

TypeDefinitionList: TypeDefinitionList TypeDefinition SYM_SEMI {

}| TypeDefinition SYM_SEMI {

}

TypeDefinitionPart: WSYM_TYPE TypeDefinitionList {

} | /* empty */

TypeDefinition: Identifier SYM_EQ Type {
    
}

Type: BasicRealType {

}| OrdinalTypeDef {

}| StructuredTypeDef {

}| PointerTypeDef {

}| Identifier {

}

StructuredTypeDef:  SetTypeDef{

}| ArrayTypeDef {

}| RecordTypeDef {

}

SetTypeDef: WSYM_SET WSYM_OF OrdinalTypeDef {

}| WSYM_SET WSYM_OF Identifier {

}

ArrayTypeDef: WSYM_ARRAY SYM_LSBKT IndexTypeList SYM_RSBKT WSYM_OF Type {

}

IndexTypeList: IndexTypeList SYM_COMMA Type {

}| Type {

}

RecordTypeDef: WSYM_RECORD FieldList WSYM_END {

}

FieldList: FieldList IdentifierList SYM_COL Type SYM_SEMI {

}| IdentifierList SYM_COL Type SYM_SEMI {

}

PointerTypeDef: SYM_HAT BasicRealType {

}| SYM_HAT Identifier {

}

OrdinalTypeDef: SYM_LPAR ConstantList SYM_RPAR {

}| Identifier SYM_DDOT Identifier {

}| Identifier SYM_DDOT SignedLiteral {

}| SignedLiteral SYM_DDOT Identifier {

}| SignedLiteral SYM_DDOT SignedLiteral {

}

ResultType: BasicRealType {

}| Identifier {

}

BasicRealType: SID_BOOLEAN {

}| SID_INTEGER {

}| SID_CHAR {

}| SID_REAL {

}

/************************* Rules of Variable *************************/

VariableDeclarationList: VariableDeclarationList VariableDeclaration SYM_SEMI {

}| VariableDeclaration SYM_SEMI {

}

VariableDeclarationPart: WSYM_VAR VariableDeclarationList {

}| /* empty */

VariableDeclaration: IdentifierList SYM_COL Type{

}

VariableAccess: Identifier SYM_HAT {

}| Identifier SYM_DOT Identifier {

}| Identifier SYM_LSBKT IndexList SYM_RSBKT {

}

IndexList: IndexList SYM_COMMA Expression {

}| Expression {

}

/************************* Rules of Expression *************************/

Expression: Expression SYM_EQ Term {

}| Expression SYM_NE Term {

}| Expression SYM_GT Term {

}| Expression SYM_GE Term {

}| Expression SYM_LT Term {

}| Expression SYM_LE Term {

}| Expression WSYM_IN Term {

}| Term {

}

Term: Term SYM_ADD Factor {

}| Term SYM_SUB Factor {

}| Term WSYM_OR Factor {

}| Factor {

}

Factor: Factor SYM_MUL Item {

}| Factor SYM_DIV Item {

}| Factor WSYM_DIV Item {

}| Factor WSYM_MOD Item {

}| Factor WSYM_AND Item {

}| Item {

}

Item: Literal { 

}| VariableAccess {

}| Identifier {

}| Sign Item {

}| WSYM_NOT Item {

}| SYM_LPAR Expression SYM_RPAR {

}

/************************* Rules of Statement *************************/

Statement: CompoundStatement {

}| AssignStatement {

}| ProcedureStatement {

}| IfStatement {

}| CaseStatement {

}| WhileStatement {

}| RepeatStatement {

}| ForStatement {

}| WithStatement {

}

AssignStatement: Identifier SYM_ASSIGN Expression {

}| VariableAccess SYM_ASSIGN Expression {

}

ProcedureStatement: Identifier {

}| Identifier SYM_LPAR ActualParameterList SYM_RPAR {

}

IfStatement: WSYM_IF Expression WSYM_THEN Statement {

} WSYM_IF Expression WSYM_THEN Statement WSYM_ELSE Statement {

}

CaseStatement: WSYM_CASE Expression WSYM_OF CaseList WSYM_END {

}

CaseList: CaseList Case SYM_SEMI {

}| Case SYM_SEMI {

}

Case: ConstantList SYM_COL Statement {

}

RepeatStatement: WSYM_REPEAT StatementList WSYM_UNTIL Expression {

}

WhileStatement: WSYM_WHILE Expression WSYM_DO Statement {

}

ForStatement: WSYM_FOR Identifier SYM_ASSIGN Expression WSYM_TO Expression WSYM_DO Statement {

}| WSYM_FOR Identifier SYM_ASSIGN Expression WSYM_DOWNTO Expression WSYM_DO Statement {

}

WithStatement: WSYM_WITH IdentifierList WSYM_DO Statement {

}

CompoundStatement: WSYM_BEGIN StatementList WSYM_END {

}

StatementList: StatementList Statement SYM_SEMI {

}| /* empty */

/************************* Rules of Constant *************************/

ConstantDefinitionPart: WSYM_CONST ConstantDefinitionList {

}| /* empty */

ConstantDefinitionList:  ConstantDefinitionList ConstantDefinition SYM_SEMI {

}| ConstantDefinition SYM_SEMI {

}

ConstantDefinition: Identifier SYM_EQ SignedLiteral {

}| Identifier SYM_EQ Identifier {

}

ConstantList: ConstantList SYM_COMMA Identifier {

}| ConstantList SYM_COMMA SignedLiteral {

}| Identifier {

}| SignedLiteral {

}

SignedLiteral: Literal {

}| Sign Literal {

}

Sign: SYM_ADD {

}| SYM_SUB {

}

Literal: VAL_INT {

}| VAL_REAL {

}| SID_MAXINT {

}| SID_TRUE {

}| SID_FALSE {

}| WSYM_NIL {

}| VAL_STRING {

}| VAL_CHAR {

}

/************************* Rules of Procedure&Statement *************************/

ProcAndFuncDeclarationPart: ProcAndFuncDeclarationPart ProcedureDeclaration SYM_SEMI {

}| ProcAndFuncDeclarationPart FunctionDeclaration SYM_SEMI {

}| /* empty */

ProcedureDeclaration: WSYM_PROCEDURE Identifier SYM_SEMI Block {

}| WSYM_PROCEDURE Identifier SYM_LPAR FormalParameterList SYM_RPAR SYM_SEMI Block {

}

FunctionDeclaration: WSYM_FUNCTION Identifier SYM_COL ResultType SYM_SEMI Block {

}| WSYM_FUNCTION Identifier SYM_LPAR FormalParameterList SYM_RPAR SYM_COL ResultType SYM_SEMI Block {

}

ActualParameterList: ActualParameterList SYM_COMMA ActualParameter {

}| ActualParameter{

}

ActualParameter: Expression {

}

FormalParameterList: FormalParameterList SYM_SEMI FormalParameter {

}| FormalParameter {

}

FormalParameter: Identifier SYM_COL Type {

}|WSYM_VAR Identifier SYM_COL Type {

}

%%

void yyerror(const char *s){
	std::cerr << s << " | line: " << yylineno << std::endl;
}

int main(){
    yyparse();
    treeNode::travel(root);
    return 0;
}