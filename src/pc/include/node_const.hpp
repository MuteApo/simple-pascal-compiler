#ifndef _NODE_CONST_H_
#define _NODE_CONST_H_

class ConstDefNode;
class ConstDefListNode;
class ConstListNode;

#include "node_expr.hpp"
#include "symbol_table.hpp"
#include <string>

class ConstDefNode {
  private:
    std::string name;
    ExprNode   *value;

  public:
    ConstDefNode(std::string id, ExprNode *v) : name(id), value(v) {}

    void add_to_symtbl();

    std::string toString() {
        return "";
    }
};

class ConstDefListNode {
  private:
    std::vector<ConstDefNode *> const_defs;

  public:
    ConstDefListNode() {
        const_defs.clear();
    }

    void addConstDef(ConstDefNode *const_def) {
        const_defs.push_back(const_def);
    }

    bool gen_sym_tab(void);

    std::string gen_asm_def(void);
};

class ConstListNode {
  private:
    std::vector<ExprNode *> const_list;

  public:
    ConstListNode() {
        const_list.clear();
    }

    void addConst(ExprNode *const_expr) {
        const_list.push_back(const_expr);
    }

    std::vector<ExprNode *> &getConstList() {
        return const_list;
    }
};

#endif
