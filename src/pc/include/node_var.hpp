#ifndef _NODE_VAR_H_
#define _NODE_VAR_H_

#include <string>
#include <vector>

extern std::vector<int> ar_lvars_length;
extern std::vector<int> ar_args_length;

class VarDefNode {
  private:
    std::string name;
    std::string type_id;

  public:
    VarDefNode(std::string name, std::string type_id) : name(name), type_id(type_id) {}

    // Find if "type_id" exists in variable symble table
    // bool is_legal(SymbolTable &symtbl){
    //     return (symtbl.findSymbol() != nullptr);
    // }

    bool gen_sym_tab() {
        // TODO:How to create a var symtbl item?
        return false;
    }

    int get_length(void) {
        TypeAttrNode* type_def = symbol_table.findSymbol(type_id);
        if (type_def == nullptr) return -1;
        return type_def->get_length();
    }

    std::string gen_asm_def(void) {
        TypeAttrNode* type_def = symbol_table.findSymbol(type_id);
        if (type_def == nullptr) return "";
        return type_def->gen_asm_def();
    }

    std::string toString() {
        return "";
    }
};

class VarDefListNode {
  private:
    std::vector<VarDefNode*> var_defs;

  public:
    void append_var_def(VarDefNode* var_def) {
        var_defs.push_back(var_def);
    }

    bool gen_sym_tab(void) {
        bool succeed      = true;
        int  lvars_length = 0;
        for (VarDefNode* var : var_defs) {
            succeed &= var->gen_sym_tab();
            lvars_length += var->get_length();
        }
        ar_lvars_length.push_back(lvars_length);
        return succeed;
    }

    std::string gen_asm_def(void) {
        std::string asm_def = "";
        for (VarDefNode* var : var_defs) asm_def += var->gen_asm_def();
        return asm_def;
    }
};

#endif