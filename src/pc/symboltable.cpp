#include "symboltable.hpp"
using namespace std;

void SymbolTable::addSymbol(treeNode* t, int kind){
    unordered_map<string, list<TableItem>>* map;
    switch (kind)
    {
    case DK_Proc:
        map=&ProcDeclMap;
        break;
    case DK_Func:
        map=&FuncDeclMap;
        break;
    case DK_Var:
        map=&VarDeclMap;
        break;
    case DK_Type:
        map=&TypeDeclMap;
        break;
    default:
        return;
    }
    string name=t->getChild().at(0)->getIdName();
    int kind=t->getChild().at(1)->getNodeKind();
    unordered_map<string, list<TableItem> >::iterator iter=map->find(name);
    if(iter==map->end()){
        map->insert(make_pair(name, list<TableItem>()));
        iter=map->find(name);
    }
    iter->second.push_front(TableItem(currLevel, name, kind, t));
}

TableItem* SymbolTable::findSymbol(string name, int kind){
    unordered_map<string, list<TableItem>>* map;
    switch (kind)
    {
    case DK_Proc:
        map=&ProcDeclMap;
        break;
    case DK_Func:
        map=&FuncDeclMap;
        break;
    case DK_Var:
        map=&VarDeclMap;
        break;
    case DK_Type:
        map=&TypeDeclMap;
        break;
    default:
        return nullptr;
    }
    unordered_map<string, list<TableItem> >::iterator iter=map->find(name);
    if(iter==map->end()){
        return nullptr;
    }
    else{
        return &iter->second.front();
    }
}

void SymbolTable::enterScope(){
    currLevel+=1;
}
void SymbolTable::leaveScope(){
    currLevel-=1;
}

void SymbolTable::printTable(){
    unordered_map<string, list<TableItem> >::iterator iter;
    list<TableItem>::iterator iter2;

    cout<<"--------Proc--------"<<endl;    
    for(iter=ProcDeclMap.begin();iter!=ProcDeclMap.end();iter++){
        cout<<iter->first;
        for(iter2=iter->second.begin(); iter2!=iter->second.end(); iter2++){
            cout<<iter2->toString()<<endl;
        }
    }
    cout<<"--------Func--------"<<endl;    
    for(iter=FuncDeclMap.begin();iter!=FuncDeclMap.end();iter++){
        cout<<iter->first;
        for(iter2=iter->second.begin(); iter2!=iter->second.end(); iter2++){
            cout<<iter2->toString()<<endl;
        }
    }
    cout<<"--------Var--------"<<endl;    
    for(iter=VarDeclMap.begin();iter!=VarDeclMap.end();iter++){
        cout<<iter->first;
        for(iter2=iter->second.begin(); iter2!=iter->second.end(); iter2++){
            cout<<iter2->toString()<<endl;
        }
    }
    cout<<"--------Type--------"<<endl;    
    for(iter=TypeDeclMap.begin();iter!=TypeDeclMap.end();iter++){
        cout<<iter->first;
        for(iter2=iter->second.begin(); iter2!=iter->second.end(); iter2++){
            cout<<iter2->toString()<<endl;
        }
    }
     
}

