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
    bool is_legal();

    bool gen_sym_tab() {
        // TODO:How to create a var symtbl item?
        return false;
    }

    int get_length(void);

    std::string gen_asm_def(void);

    std::string toString();
};

class VarDefListNode {
  private:
    std::vector<VarDefNode*> var_defs;

  public:
    void append_var_def(VarDefNode* var_def);

    bool gen_sym_tab(void);

    std::string gen_asm_def(void);
};

#endif