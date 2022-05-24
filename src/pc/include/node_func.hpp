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

    std::string getNodeInfo();

    std::string genVizCode(int run);

    bool genSymbolTable(int order);

    bool test_arg_type(TypeAttrNode *type);
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

    bool genSymbolTable();

    bool test_arg_type(std::vector<ExprNode *> args);
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

    TypeAttrNode *getRetValType();

    bool hasDecl();

    std::string genVizCode(int run);

    bool genSymbolTable();

    bool test_arg_type(ExprListNode *args);

    std::string genAsmCode();
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

    std::string genAsmCode();
};

#endif