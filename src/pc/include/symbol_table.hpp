#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include "tree.hpp"
#include "error_handler.hpp"
#include "node_type.hpp"
#include <list>
#include <string>
#include <unordered_map>

enum symbol_item_kind { symbol_const = 10001, symbol_type, symbol_var, symbol_proc_func };

class TableItem {
  public:
    int              level;
    std::string      name;
    int              kind;
    int              line;
    treeNode*        node;
    symbol_item_kind item_kind;
    TypeAttrNode*    type;
    TableItem(int lv, std::string nm, int k, treeNode* nd)
            : level(lv), name(nm), kind(k), line(nd->getLine()), node(nd) {}
    TableItem(std::string s, TypeAttrNode* t) : name(s), item_kind(symbol_type), type(t) {}
    std::string toString() {
        return "[name: " + name + "; kind:" + enum2str(kind) + "; level: " + to_string(level) + "]";
    }
};

class SymbolTable {
  private:
    const int MAXSIZE = 256;

    int currLevel;

    std::unordered_map<std::string, std::list<TableItem>> ConstDeclMap;
    std::unordered_map<std::string, std::list<TableItem>> TypeDeclMap;
    std::unordered_map<std::string, std::list<TableItem>> VarDeclMap;
    std::unordered_map<std::string, std::list<TableItem>> FuncDeclMap;
    std::unordered_map<std::string, std::list<TableItem>> ProcDeclMap;

  public:
    errorHandler err;

    SymbolTable() {
        currLevel = 0;
    }
    // void addSymbol(treeNode* t);
    void addSymbol(treeNode* t, int kind);

    void addSymbol(std::string id, TypeAttrNode* type) {
        auto item = TypeDeclMap.find(id);
        if (item == TypeDeclMap.end()) {
            std::list<TableItem> l;
            l.clear();
            l.push_back(TableItem(id, type));
            TypeDeclMap.insert(std::make_pair(id, l));
        } else
            item->second.push_front(TableItem(id, type));
    }

    TableItem* findSymbol(std::string name, int kind);

    void enterScope();
    void leaveScope();

    void printTable();
};

#endif