#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H
#include "error_handler.hpp"
#include "tree.hpp"
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

class TableItem {
  public:
    int         level;
    std::string name;
    int         kind;
    int         line;
    treeNode*   node;
    TableItem(int lv, std::string nm, int k, treeNode* nd)
            : level(lv), name(nm), kind(k), line(nd->getLine()), node(nd) {}
    std::string toString() {
        return "[name:" + name + ";kind:" + enum2str(kind) +
               ";level:" + to_string(level) + "]";
    }
};

class SymbolTable {
  private:
    const int MAXSIZE = 256;

    int currLevel;

    std::unordered_map<std::string, std::list<TableItem>> TypeDeclMap;
    std::unordered_map<std::string, std::list<TableItem>> VarDeclMap;
    std::unordered_map<std::string, std::list<TableItem>> FuncDeclMap;
    std::unordered_map<std::string, std::list<TableItem>> ProcDeclMap;

  public:
    errorHandler err;

    SymbolTable() { currLevel = 0; }
    // void addSymbol(treeNode* t);
    void addSymbol(treeNode* t, int kind);

    TableItem* findSymbol(std::string name, int kind);

    void enterScope();
    void leaveScope();

    void printTable();
};

#endif