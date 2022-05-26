#include "include/symbol_table.hpp"
#include "include/exception.hpp"
#include <iostream>

SymbolTable symbol_table;

TableItem::TableItem(std::string id, int lv) : name(id), level(lv) {}

std::string TableItem::toString() {
    return "<TableItem Root Class>";
}

ConstTableItem::ConstTableItem(std::string id, int lv, ConstDefNode *c_d)
        : TableItem(id, lv), const_def(c_d) {}

bool ConstTableItem::operator<(const ConstTableItem &rhs) const {
    if (level != rhs.level) return level < rhs.level;
    return const_def->getExpr()->getLiteralNode() < rhs.const_def->getExpr()->getLiteralNode();
}

std::string ConstTableItem::toString() {
    return name + "\t[level: " + to_string(level) +
           "\tvalue:" + to_string(const_def->getExpr()->getLiteralNode()->toString()) + "]";
}

TypeTableItem::TypeTableItem(std::string id, int lv, TypeAttrNode *t_a)
        : TableItem(id, lv), type_attr(t_a) {}

bool TypeTableItem::operator<(const TypeTableItem &rhs) const {
    return level < rhs.level || level == rhs.level && name < rhs.name;
}

std::string TypeTableItem::toString() {
    return name + "\t[level: " + to_string(level) + "]";
}

VarTableItem::VarTableItem(std::string id, int lv, VarDefNode *v_d, int ord)
        : TableItem(id, lv), var_def(v_d), order(ord) {}

bool VarTableItem::operator<(const VarTableItem &rhs) const {
    return level < rhs.level || level == rhs.level && order < rhs.order;
}

std::string VarTableItem::toString() {
    return name + "\t[level: " + to_string(level) + "\torder: " + to_string(order) + "]";
}

FuncTableItem::FuncTableItem(std::string id, int lv, FuncDefNode *f_d)
        : TableItem(id, lv), func_def(f_d) {}

bool FuncTableItem::operator<(const FuncTableItem &rhs) const {
    return level < rhs.level || level == rhs.level && name < rhs.name;
}

std::string FuncTableItem::toString() {
    return name + "\t[level: " + to_string(level) + "]";
}

SymbolTable::SymbolTable() : currLevel(-1) {}

int SymbolTable::getLevel() {
    return currLevel;
}

bool SymbolTable::addSymbol(std::string id, ConstDefNode *c_d) {
    auto item = ConstDeclMap.find(id);
    if (item == ConstDeclMap.end()) {
        ConstDeclMap.insert(make_pair(id, std::list<ConstTableItem>()));
        item = ConstDeclMap.find(id);
    }
    if (!item->second.empty() && item->second.front().level >= currLevel)
        throw RedefineError(c_d->getLineNumber(), id);
    item->second.push_front(ConstTableItem(id, currLevel, c_d));
    return true;
}
bool SymbolTable::addSymbol(std::string id, TypeAttrNode *t_a) {
    auto item = TypeDeclMap.find(id);
    if (item == TypeDeclMap.end()) {
        TypeDeclMap.insert(make_pair(id, std::list<TypeTableItem>()));
        item = TypeDeclMap.find(id);
    }
    if (!item->second.empty() && item->second.front().level >= currLevel)
        throw RedefineError(t_a->getLineNumber(), id);
    item->second.push_front(TypeTableItem(id, currLevel, t_a));
    return true;
}
bool SymbolTable::addSymbol(std::string id, VarDefNode *v_d, int ord) {
    auto item = VarDeclMap.find(id);
    if (item == VarDeclMap.end()) {
        VarDeclMap.insert(make_pair(id, std::list<VarTableItem>()));
        item = VarDeclMap.find(id);
    }
    if (!item->second.empty() && item->second.front().level >= currLevel)
        throw RedefineError(v_d->getLineNumber(), id);
    item->second.push_front(VarTableItem(id, currLevel, v_d, ord));
    return true;
}
bool SymbolTable::addSymbol(std::string id, FuncDefNode *f_d) {
    auto item = FuncDeclMap.find(id);
    if (item == FuncDeclMap.end()) {
        FuncDeclMap.insert(make_pair(id, std::list<FuncTableItem>()));
        item = FuncDeclMap.find(id);
    }
    if (!item->second.empty() && item->second.front().level >= currLevel)
        throw RedefineError(f_d->getLineNumber(), id);
    item->second.push_front(FuncTableItem(id, currLevel, f_d));
    return true;
}

void SymbolTable::popConstSymbol() {
    for (auto &it : ConstDeclMap)
        if (!it.second.empty())
            if (it.second.front().level == currLevel) it.second.pop_front();
}
void SymbolTable::popTypeSymbol() {
    for (auto &it : TypeDeclMap)
        if (!it.second.empty())
            if (it.second.front().level == currLevel) it.second.pop_front();
}
void SymbolTable::popVarSymbol() {
    for (auto &it : VarDeclMap)
        if (!it.second.empty())
            if (it.second.front().level == currLevel) it.second.pop_front();
}
void SymbolTable::popFuncSymbol() {
    for (auto &it : FuncDeclMap)
        if (!it.second.empty())
            if (it.second.front().level == currLevel) it.second.pop_front();
}

ConstDefNode *SymbolTable::findConstSymbol(std::string id) {
    auto item = ConstDeclMap.find(id);
    return item == ConstDeclMap.end() ? nullptr : item->second.front().const_def;
}
TypeAttrNode *SymbolTable::findTypeSymbol(std::string id) {
    auto item = TypeDeclMap.find(id);
    return item == TypeDeclMap.end() ? nullptr : item->second.front().type_attr;
}
VarDefNode *SymbolTable::findVarSymbol(std::string id) {
    auto item = VarDeclMap.find(id);
    return item == VarDeclMap.end() ? nullptr : item->second.front().var_def;
}
FuncDefNode *SymbolTable::findFuncSymbol(std::string id) {
    auto item = FuncDeclMap.find(id);
    return item == FuncDeclMap.end() ? nullptr : item->second.front().func_def;
}

bool SymbolTable::existSymbol(std::string id) {
    auto const_item = ConstDeclMap.find(id);
    if (const_item != ConstDeclMap.end())
        if (const_item->second.front().level >= currLevel) return true;
    auto type_item = TypeDeclMap.find(id);
    if (type_item != TypeDeclMap.end())
        if (type_item->second.front().level >= currLevel) return true;
    auto var_item = VarDeclMap.find(id);
    if (var_item != VarDeclMap.end())
        if (var_item->second.front().level >= currLevel) return true;
    auto func_item = FuncDeclMap.find(id);
    if (func_item != FuncDeclMap.end())
        if (func_item->second.front().level >= currLevel) return true;
    return false;
}

std::vector<ConstDefNode *> SymbolTable::getValidConsts() {
    std::vector<ConstDefNode *> result;
    result.clear();
    for (auto it : ConstDeclMap)
        if (it.second.front().level == currLevel) result.push_back(it.second.front().const_def);
    return result;
}
std::vector<TypeAttrNode *> SymbolTable::getValidTypes() {
    std::vector<TypeAttrNode *> result;
    result.clear();
    for (auto it : TypeDeclMap)
        if (it.second.front().level == currLevel) result.push_back(it.second.front().type_attr);
    return result;
}
std::vector<VarDefNode *> SymbolTable::getValidVars() {
    std::vector<VarDefNode *> result;
    result.clear();
    for (auto it : VarDeclMap)
        if (it.second.front().level == currLevel) result.push_back(it.second.front().var_def);
    return result;
}
std::vector<FuncDefNode *> SymbolTable::getValidFuncs() {
    std::vector<FuncDefNode *> result;
    result.clear();
    for (auto it : FuncDeclMap)
        if (it.second.front().level == currLevel) result.push_back(it.second.front().func_def);
    return result;
}

std::set<VarTableItem> SymbolTable::getVarScope(int level) {
    std::set<VarTableItem> result;
    result.clear();
    for (auto it1 : VarDeclMap) {
        if (it1.second.empty() || it1.second.front().level < level) continue;
        for (auto it2 : it1.second)
            if (it2.level == level) {
                result.insert(it2);
                break;
            }
    }
    return result;
}

ExprNode *SymbolTable::translateConstId(ExprNode *id) {
    if (id->getNodeType() != el_id) return id;
    ConstDefNode *const_def = findConstSymbol(id->getIdNode()->getName());
    if (const_def == nullptr) throw UndefineError(id->getLineNumber(), id->getIdNode()->getName());
    return const_def->getExpr();
}

TypeAttrNode *SymbolTable::translateTypeId(TypeAttrNode *id) {
    if (id->getType() != type_identifier) return id;
    TypeAttrNode *type_attr = findTypeSymbol(id->getName());
    if (type_attr == nullptr) throw UndefineError(id->getLineNumber(), id->getName());
    return type_attr;
}

template <class T>
void SymbolTable::printSymbol(const std::map<std::string, std::list<T>> &decl_map) {
    std::set<T> symbols;
    for (auto it1 : decl_map)
        if (!it1.second.empty())
            for (auto it2 : it1.second) symbols.insert(it2);
    for (auto symbol : symbols) std::cout << symbol.toString() << std::endl;
}

void SymbolTable::printTable() {
    std::cout << "current level: " << to_string(currLevel) << std::endl;
    std::cout << "--------------Const--------------" << std::endl;
    printSymbol<ConstTableItem>(ConstDeclMap);
    std::cout << "--------------Type---------------" << std::endl;
    printSymbol<TypeTableItem>(TypeDeclMap);
    std::cout << "--------------Var----------------" << std::endl;
    printSymbol<VarTableItem>(VarDeclMap);
    std::cout << "--------------Func---------------" << std::endl;
    printSymbol<FuncTableItem>(FuncDeclMap);
    std::cout << std::endl;
}

void SymbolTable::enterScope() {
    // std::cout << "Before Enter Scope, ";
    // printTable();
    currLevel++;
}

void SymbolTable::leaveScope() {
    // std::cout << "Before Leave Scope, ";
    // printTable();
    popConstSymbol();
    popTypeSymbol();
    popVarSymbol();
    popFuncSymbol();
    currLevel--;
}
