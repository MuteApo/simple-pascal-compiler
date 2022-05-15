#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include "tree.hpp"
#include "errorhandler.hpp"

using namespace std;

class TableItem{
public:
    int level;
    string name;
    int kind;
    int line;
    treeNode* node;
    TableItem(int lv, string nm, int k, treeNode* nd):level(lv),name(nm),kind(k),line(nd->getLine()),node(nd){}
    string toString(){
        return "[name:"+name+";kind:"+enum2str(kind)+";level:"+to_string(level)+"]";
    }
};

class SymbolTable {
private:
    const int MAXSIZE = 256;

    int currLevel;

    unordered_map<string, list<TableItem> > TypeDeclMap;
    unordered_map<string, list<TableItem> > VarDeclMap;
    unordered_map<string, list<TableItem> > FuncDeclMap;
    unordered_map<string, list<TableItem> > ProcDeclMap;
	
public:
    errorHandler err;

	SymbolTable(){
        currLevel=0;
    }
    // void addSymbol(treeNode* t);
    void addSymbol(treeNode* t, int kind);

    TableItem* findSymbol(string name, int kind);

    void enterScope();
    void leaveScope();

    void printTable();
};

#endif