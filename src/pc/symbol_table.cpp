#include "include/symbol_table.hpp"
using namespace std;

SymbolTable symbol_table;

void SymbolTable::addSymbol(treeNode* t, int kind) {
    unordered_map<string, list<TableItem>>* map;
    switch (kind) {
        case DK_Proc: map = &ProcDeclMap; break;
        case DK_Func: map = &FuncDeclMap; break;
        case DK_Var: map = &VarDeclMap; break;
        case DK_Type: map = &TypeDeclMap; break;
        default: return;
    }
    string name = t->getChild().at(0)->getIdName();
    int    kind = t->getChild().at(1)->getNodeKind();

    unordered_map<string, list<TableItem>>::iterator iter = map->find(name);
    if (iter == map->end()) {
        map->insert(make_pair(name, list<TableItem>()));
        iter = map->find(name);
    }
    if (iter->second.size() > 0 && iter->second.front().level == currLevel) {
        err.add(REDEFINE, name, t->getLine());
        return;
    }
    iter->second.push_front(TableItem(currLevel, name, kind, t));
}

TableItem* SymbolTable::findSymbol(string name, int kind) {
    unordered_map<string, list<TableItem>>* map;
    switch (kind) {
        case DK_Proc: map = &ProcDeclMap; break;
        case DK_Func: map = &FuncDeclMap; break;
        case DK_Var: map = &VarDeclMap; break;
        case DK_Type: map = &TypeDeclMap; break;
        default: return nullptr;
    }
    unordered_map<string, list<TableItem>>::iterator iter = map->find(name);
    if (iter == map->end()) {
        return nullptr;
    } else {
        return &iter->second.front();
    }
}

void SymbolTable::enterScope() {
    currLevel++;
    printTable();
}
void SymbolTable::leaveScope() {
    this->popSymbol(this->ConstDeclMap, this->currLevel);
    this->popSymbol(this->TypeDeclMap, this->currLevel);
    this->popSymbol(this->VarDeclMap, this->currLevel);
    this->popSymbol(this->FuncDeclMap, this->currLevel);
    this->popSymbol(this->ProcDeclMap, this->currLevel);
    currLevel--;
    printTable();
}

void SymbolTable::printTable() {
    cout << "current level: " << to_string(currLevel) << endl;
    cout << "--------Proc--------" << endl;
    this->printSymbol(ProcDeclMap);
    cout << "--------Func--------" << endl;
    this->printSymbol(FuncDeclMap);
    cout << "--------Type--------" << endl;
    this->printSymbol(TypeDeclMap);
    cout << "--------Var--------" << endl;
    this->printSymbol(VarDeclMap);
    cout << endl;
}
