#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include "error_handler.hpp"
#include "node_const.hpp"
#include "node_func.hpp"
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
    ConstDefNode *const_def;

    ConstTableItem(std::string id, int lv, ConstDefNode *c_d) : TableItem(id, lv), const_def(c_d) {}

    bool operator<(const ConstTableItem &rhs) const {
        return level < rhs.level || level == rhs.level && name < rhs.name;
    }

    std::string toString();
};

class TypeTableItem : public TableItem {
  public:
    TypeAttrNode *type_attr;

    TypeTableItem(std::string id, int lv, TypeAttrNode *t_a) : TableItem(id, lv), type_attr(t_a) {}

    bool operator<(const TypeTableItem &rhs) const {
        return level < rhs.level || level == rhs.level && name < rhs.name;
    }

    std::string toString();
};

class VarTableItem : public TableItem {
  public:
    VarDefNode *var_def;
    int         order;  // order in the scope of the same level

    VarTableItem(std::string id, int lv, VarDefNode *v_d, int ord)
            : TableItem(id, lv), var_def(v_d), order(ord) {}

    bool operator<(const VarTableItem &rhs) const {
        return level < rhs.level || level == rhs.level && order < rhs.order;
    }

    std::string toString();
};

class FuncTableItem : public TableItem {
  public:
    FuncDefNode *func_def;

    FuncTableItem(std::string id, int lv, FuncDefNode *f_d) : TableItem(id, lv), func_def(f_d) {}

    bool operator<(const FuncTableItem &rhs) const {
        return level < rhs.level || level == rhs.level && name < rhs.name;
    }

    std::string toString();
};

class SymbolTable {
  private:
    int currLevel;

    std::map<std::string, std::list<ConstTableItem>> ConstDeclMap;
    std::map<std::string, std::list<TypeTableItem>>  TypeDeclMap;
    std::map<std::string, std::list<VarTableItem>>   VarDeclMap;
    std::map<std::string, std::list<FuncTableItem>>  FuncDeclMap;

    void popConstSymbol();
    void popTypeSymbol();
    void popVarSymbol();
    void popFuncSymbol();

  public:
    errorHandler err;

    SymbolTable() : currLevel(-1) {}

    int getLevel();

    bool addSymbol(std::string id, ConstDefNode *c_d);
    bool addSymbol(std::string id, TypeAttrNode *t_a);
    bool addSymbol(std::string id, VarDefNode *v_d, int ord);
    bool addSymbol(std::string id, FuncDefNode *f_d);

    ConstDefNode *findConstSymbol(std::string id);
    TypeAttrNode *findTypeSymbol(std::string id);
    VarDefNode   *findVarSymbol(std::string id);
    FuncDefNode  *findFuncSymbol(std::string id);

    std::vector<ConstDefNode *> getValidConsts();
    std::vector<TypeAttrNode *> getValidTypes();
    std::vector<VarDefNode *>   getValidVars();
    std::vector<FuncDefNode *>  getValidFuncs();

    std::set<VarTableItem> getVarScope(int level);

    template <class T> void printSymbol(const std::map<std::string, std::list<T>> &decl_map);

    void enterScope();

    void leaveScope();

    void printTable();
};

extern SymbolTable symbol_table;

#endif