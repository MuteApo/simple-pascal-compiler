#include "include/symboltable.hpp"
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
    if(iter->second.size()>0 && iter->second.front().level==currLevel){
        err.add(REDEFINE, name, t->getLine());
        return;
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
    printTable();
    currLevel+=1;
}
void SymbolTable::leaveScope(){
    printTable();
    unordered_map<string, list<TableItem> >::iterator iter;
    iter=ProcDeclMap.begin();
    while(iter!=ProcDeclMap.end()){
        if(iter->second.size()>0 && iter->second.front().level==currLevel){
            iter->second.pop_front();
        }
        if(iter->second.size()==0){
            iter=ProcDeclMap.erase(iter);
        }
        else{
            iter++;
        }
    }
    iter=FuncDeclMap.begin();
    while(iter!=FuncDeclMap.end()){
        if(iter->second.size()>0 && iter->second.front().level==currLevel){
            iter->second.pop_front();
        }
        if(iter->second.size()==0){
            iter=FuncDeclMap.erase(iter);
        }
        else{
            iter++;
        }
    }
    iter=VarDeclMap.begin();
    while(iter!=VarDeclMap.end()){
        if(iter->second.size()>0 && iter->second.front().level==currLevel){
            iter->second.pop_front();
        }
        if(iter->second.size()==0){
            iter=VarDeclMap.erase(iter);
        }
        else{
            iter++;
        }
    }
    iter=TypeDeclMap.begin();
    while(iter!=TypeDeclMap.end()){
        if(iter->second.size()>0 && iter->second.front().level==currLevel){
            iter->second.pop_front();
        }
        if(iter->second.size()==0){
            iter=TypeDeclMap.erase(iter);
        }
        else{
            iter++;
        }
    }
    currLevel-=1;
}

void SymbolTable::printTable(){
    cout<<"current level: "<<to_string(currLevel)<<endl;

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
    cout<<endl;
}

