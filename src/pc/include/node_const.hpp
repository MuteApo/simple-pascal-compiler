#ifndef __NODE_CONST_H__
#define __NODE_CONST_H__

class ConstDefNode;
class ConstDefListNode;
class ConstListNode;

#include "defs.hpp"
#include "node_expr.hpp"
#include "symbol_table.hpp"
#include <string>

extern int global_uid;
extern int yylineno;

class ConstDefNode {
  private:
    int         uid;
    int         line_no;
    std::string name;
    ExprNode   *expr;

  public:
    ConstDefNode(std::string id, ExprNode *v);

    int getUid();

    int getLineNumber();

    ExprNode *getExpr();

    std::string genVizCode(int run);

    bool genSymbolTable();
};

class ConstDefListNode {
  private:
    int                         uid;
    int                         line_no;
    std::vector<ConstDefNode *> const_defs;

  public:
    ConstDefListNode();

    int getUid();

    void addConstDef(ConstDefNode *const_def);

    std::string genVizCode(int run);

    bool genSymbolTable();

    std::string genAsmDef();
};

class ConstListNode {
  private:
    int                     uid;
    int                     line_no;
    std::vector<ExprNode *> const_list;

  public:
    ConstListNode();

    int getUid();

    std::vector<ExprNode *> &getConstList();

    void addConst(ExprNode *const_expr);

    std::string genVizCode(int run);
};

#endif
