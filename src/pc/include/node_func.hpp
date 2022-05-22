#ifndef _NODE_FUNC_H_
#define _NODE_FUNC_H_

class ParamDefNode;
class ParamDefListNode;
class FuncDefNode;
class FuncDefListNode;

#include "node_block.hpp"
#include "node_type.hpp"
#include <string>
#include <vector>

class ParamDefNode {
  private:
    int         uid;
    bool        is_ref;
    VarDefNode *var_def;

  public:
    ParamDefNode(bool is_r, std::string id, TypeAttrNode *t);
    ~ParamDefNode();

    int getUid() {
        return uid;
    }

    std::string getNodeInfo();

    std::string gen_viz_code(int run);

    bool gen_sym_tab(int order);

    bool test_arg_type(TypeAttrNode *type);
};

class ParamDefListNode {
  private:
    int                         uid;
    std::vector<ParamDefNode *> param_defs;

  public:
    ParamDefListNode() : uid(++global_uid) {
        param_defs.clear();
    }

    int getUid() {
        return uid;
    }

    std::vector<ParamDefNode *> &getParamList() {
        return param_defs;
    }

    void addParamDef(ParamDefNode *param) {
        param_defs.push_back(param);
    }
    void addParamDef(bool is_ref, IdListNode *ids, TypeAttrNode *type);

    void mergeParamDefList(ParamDefListNode *defs) {
        for (ParamDefNode *def : defs->getParamList()) addParamDef(def);
    }

    std::string gen_viz_code(int run);

    bool gen_sym_tab();

    bool test_arg_type(std::vector<ExprNode *> args);
};

class FuncDefNode {
  private:
    int               uid;
    bool              is_func;
    std::string       name;
    ParamDefListNode *param_defs;
    TypeAttrNode     *retval_type;
    BlockNode        *block;

  public:
    FuncDefNode(std::string id, ParamDefListNode *p_d, BlockNode *b);
    FuncDefNode(std::string id, ParamDefListNode *p_d, TypeAttrNode *r_v, BlockNode *b);

    int getUid() {
        return uid;
    }

    TypeAttrNode *getRetValType() {
        return retval_type;
    }

    bool hasDecl();

    std::string gen_viz_code(int run);

    bool gen_sym_tab();

    bool test_arg_type(ExprListNode *args);

    std::string gen_asm_code();
};

class FuncDefListNode {
  private:
    int                        uid;
    std::vector<FuncDefNode *> func_defs;

  public:
    FuncDefListNode() : uid(++global_uid) {
        func_defs.clear();
    }

    int getUid() {
        return uid;
    }

    void addFunc(FuncDefNode *func_def) {
        func_defs.push_back(func_def);
    }

    std::string gen_viz_code(int run);

    bool gen_sym_tab();

    std::string gen_asm_code();
};

#endif