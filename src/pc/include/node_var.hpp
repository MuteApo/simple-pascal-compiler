#ifndef _NODE_VAR_H_
#define _NODE_VAR_H_

class VarDefNode;
class VarDefListNode;

#include "node_expr.hpp"
#include "node_type.hpp"
#include <string>
#include <vector>

extern std::vector<int> ar_lvars_length;
extern std::vector<int> ar_args_length;

class VarDefNode {
  private:
    std::string   name;
    TypeAttrNode *type;

  public:
    VarDefNode(std::string id, TypeAttrNode *t) : name(id), type(t) {}

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
    std::vector<VarDefNode *> var_defs;

  public:
    VarDefListNode() {
        var_defs.clear();
    }

    void addVarDef(VarDefNode *var_def) {
        var_defs.push_back(var_def);
    }

    void addVarDef(IdListNode *ids, TypeAttrNode *type);

    std::vector<VarDefNode *> &getVarList() {
        return var_defs;
    }

    void mergeVarDefList(VarDefListNode *defs) {
        for (VarDefNode *def : defs->getVarList()) addVarDef(def);
    }

    bool gen_sym_tab(void);

    std::string gen_asm_def(void);
};

#endif