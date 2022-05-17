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
    int         level;
    std::string name;
    int         kind;
    int         line;
    treeNode*   node;

    symbol_item_kind item_kind;
    TypeAttrNode*    type;

    TableItem(int lv, std::string nm, int k, treeNode* nd)
            : level(lv), name(nm), kind(k), line(nd->getLine()), node(nd) {}
    TableItem(std::string s, TypeAttrNode* t) : name(s), item_kind(symbol_type), type(t) {}
    std::string toString() {
        return "\t[name: " + name + "\tkind: " + enum2str(kind) + "\tlevel: " + to_string(level) +
               "]";
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

    int getLevel() {
        return this->currLevel;
    }

    /* old */
    void addSymbol(treeNode* t, int kind);

    int addSymbol(std::string id, TypeAttrNode* type) {
        auto item = TypeDeclMap.find(id);
        if (item == TypeDeclMap.end()) {
            TypeDeclMap.insert(std::make_pair(id, std::list<TableItem>()));
            item = TypeDeclMap.find(id);
        }
        if (!item->second.empty() && item->second.front().level >= this->currLevel)
            return -1;
        item->second.push_front(TableItem(id, type));
        return 0;
    }

    /* old */
    TableItem* findSymbol(std::string name, int kind);

    TypeAttrNode* findSymbol(std::string id) {
        auto item = TypeDeclMap.find(id);
        return item == TypeDeclMap.end() ? nullptr : item->second.front().type;
    }

    std::vector<TypeAttrNode*> getScope() {
        std::vector<TypeAttrNode*> result;
        result.clear();
        for (auto it : TypeDeclMap)
            if (it.second.front().level == this->currLevel)
                result.push_back(it.second.front().type);
        return result;
    }

    void popSymbol(std::unordered_map<std::string, std::list<TableItem>>& decl_map, int level) {
        for (auto& it : decl_map) {
            if (!it.second.empty() && it.second.front().level == level)
                it.second.pop_front();
            if (it.second.empty())
                decl_map.erase(it.first);
        }
    }
    void enterScope();
    void leaveScope();

    void printSymbol(const std::unordered_map<std::string, std::list<TableItem>>& decl_map) {
        for (auto it1 : decl_map) {
            std::cout << it1.first;
            for (auto it2 : it1.second) std::cout << it2.toString() << std::endl;
        }
    }
    void printTable();
};

extern SymbolTable symbol_table;

#endif