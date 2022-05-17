#ifndef _NODE_VAR_H_
#define _NODE_VAR_H_

#include <string>

class VarDefNode{
private:
    std::string name;
    std::string type_id;
    bool is_global;
public:
    VarDefNode(std::string name, std::string type_id):name(name), type_id(type_id){}
     // Find if "type_id" exists in variable symble table
    // bool is_legal(SymbolTable &symtbl){
    //     return (symtbl.findSymbol() != nullptr);
    // }
    std::string generate_asm_define(){
        
    }
};

#endif