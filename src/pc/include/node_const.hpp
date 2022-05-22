#ifndef _NODE_CONST_H_
#define _NODE_CONST_H_

class ConstDefNode;
class ConstDefListNode;
class ConstListNode;

#include "node_expr.hpp"
#include "symbol_table.hpp"
#include "viz.hpp"
#include <string>

extern int global_uid;

class ConstDefNode {
  private:
    int         uid;
    std::string name;
    ExprNode   *expr;

  public:
    ConstDefNode(std::string id, ExprNode *v) : uid(++global_uid), name(id), expr(v) {}

    int getUid() {
        return uid;
    }

    ExprNode *getExpr() {
        return expr;
    }

    std::string gen_viz_code(int run);

    bool gen_sym_tab();
};

class ConstDefListNode {
  private:
    int                         uid;
    std::vector<ConstDefNode *> const_defs;

  public:
    ConstDefListNode() : uid(++global_uid) {
        const_defs.clear();
    }

    int getUid() {
        return uid;
    }

    void addConstDef(ConstDefNode *const_def) {
        const_defs.push_back(const_def);
    }

    std::string gen_viz_code(int run);

    bool gen_sym_tab();

    std::string gen_asm_def();
};

class ConstListNode {
  private:
    int                     uid;
    std::vector<ExprNode *> const_list;

  public:
    ConstListNode() : uid(++global_uid) {
        const_list.clear();
    }

    int getUid() {
        return uid;
    }

    std::vector<ExprNode *> &getConstList() {
        return const_list;
    }

    void addConst(ExprNode *const_expr) {
        const_list.push_back(const_expr);
    }

    std::string gen_viz_code(int run);
};

#endif
