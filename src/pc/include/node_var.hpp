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

    void translateId();

    // Find if "type_id" exists in variable symble table
    bool is_legal();

    bool gen_sym_tab(int ord);

    int get_length(void);

    std::string gen_viz_code();

    std::string gen_asm_def(void);

    std::string toString() {
        return "";
    }
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

    void translateId();

    bool gen_sym_tab();

    std::string gen_asm_def(void);

    std::string gen_viz_code() {
        std::string result = vizNode(uid, "VarDefListNode");
        for (int i = 0; i < var_defs.size(); i++) {
            result += vizChildEdge(uid,
                                   var_defs.at(i)->getUid(),
                                   "field" + to_string(i + 1),
                                   "Type of Field " + to_string(i + 1));
            result += var_defs.at(i)->gen_viz_code();
        }
        return result;
    }
};

#endif