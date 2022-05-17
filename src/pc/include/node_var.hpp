#ifndef _NODE_VAR_H_
#define _NODE_VAR_H_

#include <string>
#include <vector>

class VarDefListNode {
  private:
    std::vector<VarDefNode *> var_defs;

  public:
    void append_var_def(VarDefNode *var_def) { var_defs.push_back(var_def); }
    bool gen_sym_tab(void) {
        bool succeed = true;
        int lvars_length = 0;
        for (int i = 0; i < var_defs.size(); i++) {
            lvars_length += var_defs[i]->get_length();
            succeed &= var_defs[i]->gen_sym_tab();
        }
        ar_lvars_length.push_back(lvars_length);
        return succeed;
    }
    std::string gen_asm_def(void) {
        std::string asm_def = "";
        for (int i = 0; i < var_defs.size(); i++)
            asm_def += var_defs[i]->gen_asm_def();
    }
};

class VarDefNode {
  private:
    std::string name;
    std::string type_id;

  public:
    VarDefNode(std::string name, std::string type_id)
        : name(name), type_id(type_id) {}
    // Find if "type_id" exists in variable symble table
    // bool is_legal(SymbolTable &symtbl){
    //     return (symtbl.findSymbol() != nullptr);
    // }
    bool gen_sym_tab() {
        // TODO:How to create a var symtbl item?
    }
    int get_length(void) {
        TypeAttrNode *type_def = symbol_table.findSymbol(type_id);
        if (type_def == nullptr)
            return type_def->get_length();
        return -1;
    }
    std::string gen_asm_def(void) {
        TypeAttrNode *type_def = symbol_table.findSymbol(type_id);
        if (type_def != nullptr)
            return type_def->gen_asm_def();
        return "";
    }
};

#endif