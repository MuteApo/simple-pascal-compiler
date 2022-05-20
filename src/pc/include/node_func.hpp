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
    int           uid;
    bool          is_ref;
    std::string   name;
    TypeAttrNode *type;

  public:
    ParamDefNode(bool is_r, std::string id, TypeAttrNode *t)
            : uid(++global_uid), is_ref(is_r), name(id), type(t) {}

    int getUid() {
        return uid;
    }

    std::string getNodeInfo() {
        std::string result = "ParamDefNode\n" + name;
        if (is_ref) result += "(ref)";
        return result;
    }

    std::string gen_viz_code() {
        std::string result = vizNode(uid, getNodeInfo());
        result += vizChildEdge(uid, type->getUid());
        result += type->gen_viz_code();
        return result;
    }
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

    std::string gen_viz_code() {
        std::string result = vizNode(uid, "ParamDefListNode");
        for (ParamDefNode *def : param_defs) {
            result += vizChildEdge(uid, def->getUid());
            result += def->gen_viz_code();
        }
        return result;
    }
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
    FuncDefNode(std::string id, ParamDefListNode *p_d, BlockNode *b)
            : uid(++global_uid),
              is_func(false),
              name(id),
              param_defs(p_d),
              retval_type(nullptr),
              block(b) {}
    FuncDefNode(std::string id, ParamDefListNode *p_d, TypeAttrNode *r_v, BlockNode *b)
            : uid(++global_uid),
              is_func(true),
              name(id),
              param_defs(p_d),
              retval_type(r_v),
              block(b) {}

    int getUid() {
        return uid;
    }

    std::string gen_asm_code(void);

    std::string gen_viz_code();
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

    std::string gen_asm_code();

    std::string gen_viz_code() {
        std::string result = vizNode(uid, "FuncDefListNode");
        for (FuncDefNode *def : func_defs) {
            result += vizChildEdge(uid, def->getUid());
            result += def->gen_viz_code();
        }
        return result;
    }
};

#endif