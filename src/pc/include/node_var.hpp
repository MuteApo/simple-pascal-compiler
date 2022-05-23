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
extern int yylineno;

extern std::vector<int> ar_lvars_length;
extern std::vector<int> ar_args_length;

class VarDefNode {
  private:
    int           uid;
    int           line_no;
    bool          is_type_id;
    std::string   name;
    TypeAttrNode *type;

  public:
    VarDefNode(std::string id, TypeAttrNode *t);

    int getUid();

    std::string getName();

    TypeAttrNode *getType();

    // Find if "type_id" exists in variable symble table
    bool isLegal();

    std::string genVizCode(int run);

    void translateId();

    bool genSymbolTable(int ord);

    std::string genAsmDef();
};

class VarDefListNode {
  private:
    int                       uid;
    int                       line_no;
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

    std::string genVizCode(int run);

    void translateId();

    bool genSymbolTable();

    bool isTypeEqual(VarDefListNode *rhs);

    std::string genAsmDef();
};

#endif