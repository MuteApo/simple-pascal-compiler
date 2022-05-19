#ifndef _NODE_FUNC_H_
#define _NODE_FUNC_H_

class FuncDefNode;
class FuncDefListNode;

#include "node_block.hpp"
#include "node_type.hpp"
#include <string>
#include <vector>

class ParamDefNode {
  private:
    bool          is_ref;
    std::string   name;
    TypeAttrNode *type;

  public:
    ParamDefNode(bool is_r, std::string id, TypeAttrNode *t) : is_ref(is_r), name(id), type(t) {}
};

class ParamDefListNode {
  private:
    std::vector<ParamDefNode *> param_defs;

  public:
    ParamDefListNode() {
        param_defs.clear();
    }

    void addParamDef(ParamDefNode *param) {
        param_defs.push_back(param);
    }
    void addParamDef(bool is_ref, IdListNode *ids, TypeAttrNode *type) {
        for (IdNode *id : ids->getIdList())
            addParamDef(new ParamDefNode(is_ref, id->getName(), type));
    }

    std::vector<ParamDefNode *> &getParamList() {
        return param_defs;
    }

    void mergeParamDefList(ParamDefListNode *defs) {
        for (ParamDefNode *def : defs->getParamList()) addParamDef(def);
    }
};

class FuncDefNode {
  private:
    bool              is_func;
    std::string       name;
    ParamDefListNode *param_defs;
    TypeAttrNode     *retval_type;
    BlockNode        *block;

  public:
    FuncDefNode(std::string id, ParamDefListNode *p_d, BlockNode *b)
            : is_func(false), name(id), param_defs(p_d), retval_type(nullptr), block(b) {}
    FuncDefNode(std::string id, ParamDefListNode *p_d, TypeAttrNode *r_v, BlockNode *b)
            : is_func(true), name(id), param_defs(p_d), retval_type(r_v), block(b) {}

    std::string gen_asm_code(void);
};

class FuncDefListNode {
  private:
    std::vector<FuncDefNode *> func_defs;

  public:
    FuncDefListNode() {
        func_defs.clear();
    }

    void addFunc(FuncDefNode *func_def) {
        func_defs.push_back(func_def);
    }

    std::string gen_asm_code();
};

#endif