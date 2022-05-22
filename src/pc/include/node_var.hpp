#ifndef _NODE_VAR_H_
#define _NODE_VAR_H_

class VarDefNode;
class VarDefListNode;

#include "node_expr.hpp"
#include "node_type.hpp"
#include "viz.hpp"
#include <string>
#include <vector>

extern int global_uid;

extern std::vector<int> ar_lvars_length;
extern std::vector<int> ar_args_length;

class VarDefNode {
  private:
    int           uid;
    bool          is_type_id;
    std::string   name;
    TypeAttrNode *type;

  public:
    VarDefNode(std::string id, TypeAttrNode *t);

    int getUid() {
        return uid;
    }

    std::string getName() {
        return name;
    }

    TypeAttrNode *getType() {
        return type;
    }

    // Find if "type_id" exists in variable symble table
    bool is_legal();

    int get_length(void);

    std::string gen_viz_code(int run);

    void translateId();

    bool gen_sym_tab(int ord);

    std::string gen_asm_def();
};

class VarDefListNode {
  private:
    int                       uid;
    std::vector<VarDefNode *> var_defs;

  public:
    VarDefListNode() : uid(++global_uid) {
        var_defs.clear();
    }

    int getUid() {
        return uid;
    }

    int getDim() {
        return var_defs.size();
    }

    std::vector<VarDefNode *> &getVarList() {
        return var_defs;
    }

    void addVarDef(VarDefNode *var_def) {
        var_defs.push_back(var_def);
    }
    void addVarDef(IdListNode *ids, TypeAttrNode *type);

    void mergeVarDefList(VarDefListNode *defs) {
        for (VarDefNode *def : defs->getVarList()) addVarDef(def);
    }

    std::string gen_viz_code(int run);

    void translateId();

    bool gen_sym_tab();

    std::string gen_asm_def();
};

#endif