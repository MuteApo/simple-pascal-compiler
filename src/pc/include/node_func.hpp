#ifndef __NODE_FUNC_H__
#define __NODE_FUNC_H__

class ParamDefNode;
class ParamDefListNode;
class FuncDefNode;
class FuncDefListNode;

#include "node_block.hpp"
#include "node_expr.hpp"
#include "node_type.hpp"
#include <string>
#include <vector>

extern int global_uid;
extern int yylineno;

class ParamDefNode {
  private:
    int         uid;
    int         line_no;
    bool        is_ref;
    VarDefNode *var_def;

  public:
    ParamDefNode(bool is_r, std::string id, TypeAttrNode *t);
    ~ParamDefNode();

    int getUid();

    VarDefNode *getVarDef();

    std::string getNodeInfo();

    std::string genVizCode(int run);

    bool genSymbolTable(int order);

    bool testArgType(TypeAttrNode *type);
};

class ParamDefListNode {
  private:
    int                         uid;
    int                         line_no;
    std::vector<ParamDefNode *> param_defs;

  public:
    ParamDefListNode();

    int getUid();

    std::vector<ParamDefNode *> &getParamList();

    void addParamDef(ParamDefNode *param);
    void addParamDef(bool is_ref, IdListNode *ids, TypeAttrNode *type);

    void mergeParamDefList(ParamDefListNode *defs);

    std::string genVizCode(int run);

    bool genSymbolTable(int param_bias);

    bool testArgType(ExprListNode *args);
};

class FuncDefNode {
  private:
    int               uid;
    int               line_no;
    bool              is_func;
    std::string       name;
    ParamDefListNode *param_defs;
    TypeAttrNode     *retval_type;
    BlockNode        *block;

  public:
    FuncDefNode(std::string id, ParamDefListNode *p_d, BlockNode *b);
    FuncDefNode(std::string id, ParamDefListNode *p_d, TypeAttrNode *r_v, BlockNode *b);

    int getUid();

    int getLineNumber();

    TypeAttrNode *getRetValType();

    bool hasDecl();

    std::string genVizCode(int run);

    bool genSymbolTable();

    bool testArgType(ExprListNode *args);

    void visit();
};

class FuncDefListNode {
  private:
    int                        uid;
    int                        line_no;
    std::vector<FuncDefNode *> func_defs;

  public:
    FuncDefListNode();

    int getUid();

    void addFunc(FuncDefNode *func_def);

    std::string genVizCode(int run);

    bool genSymbolTable();

    void visit();
};

#endif