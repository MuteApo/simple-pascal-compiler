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

    int getUid();

    std::string getName();

    TypeAttrNode *getType();

    // Find if "type_id" exists in variable symble table
    bool is_legal();

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
    VarDefListNode();

    int getUid();

    int getDim();

    std::vector<VarDefNode *> &getVarList();

    VarDefNode *getVarDef(std::string id);

    int getLength();

    int getOffset(std::string member);

    void addVarDef(VarDefNode *var_def);
    void addVarDef(IdListNode *ids, TypeAttrNode *type);

    void mergeVarDefList(VarDefListNode *defs);

    std::string gen_viz_code(int run);

    void translateId();

    bool gen_sym_tab();

    bool is_type_equ(VarDefListNode *rhs);

    std::string gen_asm_def();
};

#endif