#include "include/symbol_table.hpp"
#include "include/exception.hpp"
#include <iostream>

SymbolTable symbol_table;

TableItem::TableItem(std::string id, int lv) : name(id), level(lv) {}

int TableItem::getLevel() {
    return level;
}

std::string TableItem::toString() {
    return "<TableItem Root Class>";
}

ConstTableItem::ConstTableItem(std::string id, int lv, ConstDefNode *c_d)
        : TableItem(id, lv), const_def(c_d) {}

bool ConstTableItem::operator<(const ConstTableItem &rhs) const {
    if (level != rhs.level) return level < rhs.level;
    LiteralNode *lhs_value = const_def->getExpr()->getLiteralNode();
    LiteralNode *rhs_value = rhs.const_def->getExpr()->getLiteralNode();
    if (!lhs_value->isValueEqual(rhs_value)) return *lhs_value < *rhs_value;
    return name < rhs.name;
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

VarTableItem::VarTableItem(std::string id, int lv, VarDefNode *v_d, int o)
        : TableItem(id, lv), var_def(v_d), offset(o) {}

bool VarTableItem::operator<(const VarTableItem &rhs) const {
    return level < rhs.level || level == rhs.level && offset > rhs.offset;
}

int VarTableItem::getOffset() {
    return offset;
}

std::string VarTableItem::toString() {
    return name + "\t[level: " + to_string(level) + "\toffset: " + to_string(offset) + "]";
}

FuncTableItem::FuncTableItem(std::string id, int lv, FuncDefNode *f_d)
        : TableItem(id, lv), func_def(f_d) {}

bool FuncTableItem::operator<(const FuncTableItem &rhs) const {
    return level < rhs.level || level == rhs.level && name < rhs.name;
}

std::string FuncTableItem::toString() {
    return name + "\t[level: " + to_string(level) + "]";
}

SymbolTable::SymbolTable() : isPrint(false), currLevel(-1) {}

int SymbolTable::getLevel() {
    return currLevel;
}

void SymbolTable::setIsPrint() {
    isPrint = true;
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

ConstDefNode *SymbolTable::findConstSymbol(std::string id, int *level) {
    auto item = ConstDeclMap.find(id);
    if (item == ConstDeclMap.end()) return nullptr;
    if (level != nullptr) *level = item->second.front().level;
    return item->second.front().const_def;
}
TypeAttrNode *SymbolTable::findTypeSymbol(std::string id, int *level) {
    auto item = TypeDeclMap.find(id);
    if (item == TypeDeclMap.end()) return nullptr;
    if (level != nullptr) *level = item->second.front().level;
    return item->second.front().type_attr;
}
VarDefNode *SymbolTable::findVarSymbol(std::string id, int *level) {
    auto item = VarDeclMap.find(id);
    if (item == VarDeclMap.end()) return nullptr;
    if (level != nullptr) *level = item->second.front().level;
    return item->second.front().var_def;
}
FuncDefNode *SymbolTable::findFuncSymbol(std::string id, int *level) {
    auto item = FuncDeclMap.find(id);
    if (item == FuncDeclMap.end()) return nullptr;
    if (level != nullptr) *level = item->second.front().level;
    return item->second.front().func_def;
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
    for (auto it : decl_map)
        if (!it.second.empty()) symbols.insert(it.second.front());
    for (auto symbol : symbols) std::cout << symbol.toString() << std::endl;
}

void SymbolTable::printTable() {
    std::cout << "current level: " << to_string(currLevel) << std::endl;
    std::cout << "--------------Const--------------\n";
    printSymbol<ConstTableItem>(ConstDeclMap);
    std::cout << "--------------Type---------------\n";
    printSymbol<TypeTableItem>(TypeDeclMap);
    std::cout << "--------------Var----------------\n";
    printSymbol<VarTableItem>(VarDeclMap);
    std::cout << "--------------Func---------------\n";
    printSymbol<FuncTableItem>(FuncDeclMap);
}

void SymbolTable::enterScope() {
    if (isPrint) {
        std::cout << "\nBefore Enter Scope, ";
        printTable();
    }
    currLevel++;
}

void SymbolTable::leaveScope() {
    if (isPrint) {
        std::cout << "\nBefore Leave Scope, ";
        printTable();
    }
    popConstSymbol();
    popTypeSymbol();
    popVarSymbol();
    popFuncSymbol();
    currLevel--;
}
