#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include "tree.hpp"
#include "error_handler.hpp"
#include "node_const.hpp"
#include "node_type.hpp"
#include "node_var.hpp"
#include <list>
#include <map>
#include <string>

class TableItem {
  public:
    std::string name;
    int         level;  // nested level of the scope

    TableItem(std::string id, int lv) : name(id), level(lv) {}

    std::string toString();
};

class ConstTableItem : public TableItem {
  public:
    ConstDefNode* const_def;

    ConstTableItem(std::string id, int lv, ConstDefNode* const_)
            : TableItem(id, lv), const_def(const_) {}

    std::string toString();
};

class TypeTableItem : public TableItem {
  public:
    TypeAttrNode* type_attr;

    TypeTableItem(std::string id, int lv, TypeAttrNode* type)
            : TableItem(id, lv), type_attr(type) {}

    std::string toString();
};

class VarTableItem : public TableItem {
  public:
    VarDefNode* var_def;
    int         order;  // order in the scope of the same level

    VarTableItem(std::string id, int lv, VarDefNode* var, int ord)
            : TableItem(id, lv), var_def(var), order(ord) {}

    bool operator<(const VarTableItem& rhs) const {
        return level < rhs.level || level == rhs.level && order < rhs.order;
    }

    std::string toString();
};

class SymbolTable {
  private:
    int currLevel;

    std::map<std::string, std::list<ConstTableItem>> ConstDeclMap;
    std::map<std::string, std::list<TypeTableItem>>  TypeDeclMap;
    std::map<std::string, std::list<VarTableItem>>   VarDeclMap;
    // std::map<std::string, std::list<TableItem>> FuncDeclMap;
    // std::map<std::string, std::list<TableItem>> ProcDeclMap;

    void popConstSymbol();
    void popTypeSymbol();
    void popVarSymbol();

  public:
    errorHandler err;

    SymbolTable() : currLevel(0) {}

    int getLevel();

    int addSymbol(std::string id, ConstDefNode* const_);
    int addSymbol(std::string id, TypeAttrNode* type);
    int addSymbol(std::string id, VarDefNode* var, int ord);

    ConstDefNode* findConstSymbol(std::string id);
    TypeAttrNode* findTypeSymbol(std::string id);
    VarDefNode*   findVarSymbol(std::string id);

    std::vector<ConstDefNode*> getValidConsts();
    std::vector<TypeAttrNode*> getValidTypes();
    std::vector<VarDefNode*>   getValidVars();

    std::set<VarTableItem>
    getVarScope(const std::map<std::string, std::list<VarTableItem>>& decl_map, int level);

    template <class T> void printSymbol(const std::map<std::string, std::list<T>>& decl_map);

    void enterScope();

    void leaveScope();

    void printTable();
};

extern SymbolTable symbol_table;

#endif