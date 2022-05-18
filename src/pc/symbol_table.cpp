#include "include/symbol_table.hpp"
using namespace std;

SymbolTable symbol_table;

string TableItem::toString() {
    return "<TableItem Root Class>";
}
string ConstTableItem::toString() {
    return "\t[name: " + name + "\tlevel: " + to_string(level) +
           "\tconst: " + const_def->toString() + "]";
}
string TypeTableItem::toString() {
    return "\t[name: " + name + "\tlevel: " + to_string(level) +
           "\ttype: " + type_attr->toString() + "]";
}
string VarTableItem::toString() {
    return "\t[name: " + name + "\tlevel: " + to_string(level) + "\torder: " + to_string(order) +
           "\tvar: " + var_def->toString() + "]";
}

int SymbolTable::getLevel() {
    return this->currLevel;
}

int SymbolTable::addSymbol(string id, ConstDefNode* const_) {
    auto item = ConstDeclMap.find(id);
    if (item == ConstDeclMap.end()) {
        ConstDeclMap.insert(make_pair(id, list<ConstTableItem>()));
        item = ConstDeclMap.find(id);
    }
    if (!item->second.empty() && item->second.front().level >= this->currLevel) return -1;
    item->second.push_front(ConstTableItem(id, this->currLevel, const_));
    return 0;
}
int SymbolTable::addSymbol(string id, TypeAttrNode* type) {
    auto item = TypeDeclMap.find(id);
    if (item == TypeDeclMap.end()) {
        TypeDeclMap.insert(make_pair(id, list<TypeTableItem>()));
        item = TypeDeclMap.find(id);
    }
    if (!item->second.empty() && item->second.front().level >= this->currLevel) return -1;
    item->second.push_front(TypeTableItem(id, this->currLevel, type));
    return 0;
}
int SymbolTable::addSymbol(string id, VarDefNode* var, int ord) {
    auto item = VarDeclMap.find(id);
    if (item == VarDeclMap.end()) {
        VarDeclMap.insert(make_pair(id, list<VarTableItem>()));
        item = VarDeclMap.find(id);
    }
    if (!item->second.empty() && item->second.front().level >= this->currLevel) return -1;
    item->second.push_front(VarTableItem(id, this->currLevel, var, ord));
    return 0;
}

void SymbolTable::popConstSymbol() {
    for (auto& it : this->ConstDeclMap)
        if (!it.second.empty())
            if (it.second.front().level == this->currLevel) it.second.pop_front();
}
void SymbolTable::popTypeSymbol() {
    for (auto& it : this->TypeDeclMap)
        if (!it.second.empty())
            if (it.second.front().level == this->currLevel) it.second.pop_front();
}
void SymbolTable::popVarSymbol() {
    for (auto& it : this->VarDeclMap)
        if (!it.second.empty())
            if (it.second.front().level == this->currLevel) it.second.pop_front();
}

ConstDefNode* SymbolTable::findConstSymbol(string id) {
    auto item = ConstDeclMap.find(id);
    return item == ConstDeclMap.end() ? nullptr : item->second.front().const_def;
}
TypeAttrNode* SymbolTable::findTypeSymbol(string id) {
    auto item = TypeDeclMap.find(id);
    return item == TypeDeclMap.end() ? nullptr : item->second.front().type_attr;
}
VarDefNode* SymbolTable::findVarSymbol(string id) {
    auto item = VarDeclMap.find(id);
    return item == VarDeclMap.end() ? nullptr : item->second.front().var_def;
}

vector<ConstDefNode*> SymbolTable::getValidConsts() {
    vector<ConstDefNode*> result;
    result.clear();
    for (auto it : ConstDeclMap)
        if (it.second.front().level == this->currLevel)
            result.push_back(it.second.front().const_def);
    return result;
}
vector<TypeAttrNode*> SymbolTable::getValidTypes() {
    vector<TypeAttrNode*> result;
    result.clear();
    for (auto it : TypeDeclMap)
        if (it.second.front().level == this->currLevel)
            result.push_back(it.second.front().type_attr);
    return result;
}
vector<VarDefNode*> SymbolTable::getValidVars() {
    vector<VarDefNode*> result;
    result.clear();
    for (auto it : VarDeclMap)
        if (it.second.front().level == this->currLevel) result.push_back(it.second.front().var_def);
    return result;
}

set<VarTableItem> SymbolTable::getVarScope(const map<string, list<VarTableItem>>& decl_map,
                                           int                                    level) {
    set<VarTableItem> result;
    result.clear();
    for (auto it1 : decl_map) {
        if (it1.second.empty() || it1.second.front().level < level) continue;
        for (auto it2 : it1.second)
            if (it2.level == level) {
                result.insert(it2);
                break;
            }
    }
    return result;
}

template <class T> void SymbolTable::printSymbol(const map<string, list<T>>& decl_map) {
    for (auto it1 : decl_map)
        if (!it1.second.empty()) {
            cout << it1.first;
            for (auto it2 : it1.second) cout << it2.toString() << endl;
        }
}

void SymbolTable::enterScope() {
    printTable();
    this->currLevel++;
}

void SymbolTable::leaveScope() {
    // printTable();
    this->popConstSymbol();
    this->popTypeSymbol();
    this->popVarSymbol();
    this->currLevel--;
}

void SymbolTable::printTable() {
    cout << "current level: " << to_string(this->currLevel) << endl;
    cout << "--------Const--------" << endl;
    this->printSymbol<ConstTableItem>(this->ConstDeclMap);
    cout << "--------Type--------" << endl;
    this->printSymbol<TypeTableItem>(this->TypeDeclMap);
    cout << "--------Var--------" << endl;
    this->printSymbol<VarTableItem>(this->VarDeclMap);
    cout << endl;
}
