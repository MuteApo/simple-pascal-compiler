#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "error_handler.hpp"
#include "node_const.hpp"
#include "node_func.hpp"
#include "node_type.hpp"
#include "node_var.hpp"
#include <list>
#include <map>
#include <set>
#include <string>

class TableItem {
  public:
    std::string name;
    int         level;  // the nested level of scope

    TableItem(std::string id, int lv);

    int getLevel();

    std::string toString();
};

class ConstTableItem : public TableItem {
  public:
    ConstDefNode *const_def;

    ConstTableItem(std::string id, int lv, ConstDefNode *c_d);

    bool operator<(const ConstTableItem &rhs) const;

    std::string toString();
};

class TypeTableItem : public TableItem {
  public:
    TypeAttrNode *type_attr;

    TypeTableItem(std::string id, int lv, TypeAttrNode *t_a);

    bool operator<(const TypeTableItem &rhs) const;

    std::string toString();
};

class VarTableItem : public TableItem {
  public:
    VarDefNode *var_def;
    int         order;  // order in the scope of the same level

    VarTableItem(std::string id, int lv, VarDefNode *v_d, int ord);

    bool operator<(const VarTableItem &rhs) const;

    std::string toString();
};

class FuncTableItem : public TableItem {
  public:
    FuncDefNode *func_def;

    FuncTableItem(std::string id, int lv, FuncDefNode *f_d);

    bool operator<(const FuncTableItem &rhs) const;

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
    SymbolTable();

    int getLevel();

    bool addSymbol(std::string id, ConstDefNode *c_d);
    bool addSymbol(std::string id, TypeAttrNode *t_a);
    bool addSymbol(std::string id, VarDefNode *v_d, int ord);
    bool addSymbol(std::string id, FuncDefNode *f_d);

    ConstDefNode *findConstSymbol(std::string id);
    TypeAttrNode *findTypeSymbol(std::string id);
    VarDefNode   *findVarSymbol(std::string id);
    FuncDefNode  *findFuncSymbol(std::string id);

    bool existSymbol(std::string id);

    std::vector<ConstDefNode *> getValidConsts(int level);
    std::vector<TypeAttrNode *> getValidTypes(int level);
    std::vector<VarDefNode *>   getValidVars(int level);
    std::vector<FuncDefNode *>  getValidFuncs(int level);

    std::set<VarTableItem> getVarScope(int level);

    ExprNode     *translateConstId(ExprNode *id);
    TypeAttrNode *translateTypeId(TypeAttrNode *id);

    template <class T> void printSymbol(const std::map<std::string, std::list<T>> &decl_map);

    void printTable();

    void enterScope();

    void leaveScope();
};

extern SymbolTable symbol_table;

#endif