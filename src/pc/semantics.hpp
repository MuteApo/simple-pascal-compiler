#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "symboltable.hpp"
#include "errorhandler.hpp"

class semanticAnalyser{
  public:
    SymbolTable st;
    errorHandler err;
    semanticAnalyser(){}
    semanticAnalyser(SymbolTable s, errorHandler e): st(s), err(e){}

    void analyseTree(treeNode* t);
    void check_decl(string name, int kind, int line);
    
    // void check_redecl(treeNode* t){
    // }    

    // void check_return_type(char *value) {
    // }

    // int check_types(char *type1, char *type2){
    // }
};

#endif