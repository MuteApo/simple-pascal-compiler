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
    ExprNode   *value;

  public:
    ConstDefNode(std::string id, ExprNode *v) : uid(++global_uid), name(id), value(v) {}

    int getUid() {
        return uid;
    }

    ExprNode *getValue() {
        return value;
    }

    bool gen_sym_tab();

    std::string gen_viz_code(void);

    std::string toString() {
        return "";
    }
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

    std::string gen_viz_code() {
        std::string result = vizNode(uid, "ConstDefListNode");
        for (int i = 0; i < const_defs.size(); i++) {
            result += vizChildEdge(uid,
                                   const_defs.at(i)->getUid(),
                                   "constdef" + to_string(i + 1),
                                   "Const Definition " + to_string(i + 1));
            result += const_defs.at(i)->gen_viz_code();
        }
        return result;
    }

    bool gen_sym_tab() {
        bool result = true;
        for (ConstDefNode *def : const_defs) result &= def->gen_sym_tab();
        return result;
    }

    std::string gen_asm_def(void);
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

    void addConst(ExprNode *const_expr) {
        const_list.push_back(const_expr);
    }

    std::vector<ExprNode *> &getConstList() {
        return const_list;
    }

    std::string gen_viz_code();
};

#endif
