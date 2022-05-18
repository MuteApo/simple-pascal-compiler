#ifndef _NODE_STMT_H_
#define _NODE_STMT_H_

#include "defs.hpp"
#include "node_const.hpp"
#include <string>
#include <vector>

class StmtListNode;
class AssignStmtNode;
class IfStmtNode;
class ForStmtNode;
class WhileStmtNode;
class RepeatStmtNode;
class CaseStmtNode;
class SwitchStmtNode;

class StmtNode {
  private:
    stmt_type       type;
    StmtListNode   *compound_stmt;
    AssignStmtNode *assign_stmt;
    IfStmtNode     *if_stmt;
    ForStmtNode    *for_stmt;
    WhileStmtNode  *while_stmt;
    RepeatStmtNode *repeat_stmt;
    SwitchStmtNode *switch_stmt;

  public:
    StmtNode(stmt_type       t,
             StmtListNode   *c_s,
             AssignStmtNode *a_s,
             IfStmtNode     *i_s,
             ForStmtNode    *f_s,
             WhileStmtNode  *w_s,
             RepeatStmtNode *r_s,
             SwitchStmtNode *s_s)
            : type(t),
              compound_stmt(c_s),
              assign_stmt(a_s),
              if_stmt(i_s),
              for_stmt(f_s),
              while_stmt(w_s),
              repeat_stmt(r_s),
              switch_stmt(s_s) {}
    StmtNode(StmtListNode *c_s)
            : StmtNode(SK_Compound, c_s, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) {}
    StmtNode(AssignStmtNode *a_s)
            : StmtNode(SK_Assign, nullptr, a_s, nullptr, nullptr, nullptr, nullptr, nullptr) {}
    StmtNode(IfStmtNode *i_s)
            : StmtNode(SK_If, nullptr, nullptr, i_s, nullptr, nullptr, nullptr, nullptr) {}
    StmtNode(ForStmtNode *f_s)
            : StmtNode(SK_For, nullptr, nullptr, nullptr, f_s, nullptr, nullptr, nullptr) {}
    StmtNode(WhileStmtNode *w_s)
            : StmtNode(SK_While, nullptr, nullptr, nullptr, nullptr, w_s, nullptr, nullptr) {}
    StmtNode(RepeatStmtNode *r_s)
            : StmtNode(SK_While, nullptr, nullptr, nullptr, nullptr, nullptr, r_s, nullptr) {}
    StmtNode(SwitchStmtNode *s_s)
            : StmtNode(SK_While, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, s_s) {}

    std::string gen_asm_code();
};

class StmtListNode {
  private:
    std::vector<StmtNode *> stmts;

  public:
    void append_stmt(StmtNode *stmt) {
        stmts.push_back(stmt);
    }

    std::string gen_asm_code() {
        std::string asm_code = "";
        for (StmtNode *s : stmts) asm_code += s->gen_asm_code();
        return asm_code;
    }
};

class AssignStmtNode {
  private:
    ExprNode *dst;
    ExprNode *src;

  public:
    AssignStmtNode(ExprNode *d, ExprNode *s) : dst(d), src(s) {}

    std::string gen_asm_code();
};

class IfStmtNode {
  private:
    ExprNode     *expr;
    StmtListNode *then_part;
    StmtListNode *else_part;

  public:
    IfStmtNode(ExprNode *e, StmtListNode *t_p, StmtListNode *e_p = nullptr)
            : expr(e), then_part(t_p), else_part(e_p) {}

    std::string gen_asm_code();
};

class ForStmtNode {  // TODO
  private:
    std::string   name;
    ExprNode     *start_expr;
    ExprNode     *end_expr;
    StmtListNode *body_part;

  public:
};

class WhileStmtNode {
  private:
    ExprNode     *condition;
    StmtListNode *body_part;

  public:
    WhileStmtNode(ExprNode *c, StmtListNode *b_p) : condition(c), body_part(b_p) {}

    std::string gen_asm_code();
};

class RepeatStmtNode {
  private:
    ExprNode     *condition;
    StmtListNode *body_part;

  public:
    RepeatStmtNode(ExprNode *c, StmtListNode *b_p) : condition(c), body_part(b_p) {}

    std::string gen_asm_code();
};

class CaseStmtNode {
  private:
    ConstDefListNode *const_list;
    StmtListNode     *body_part;

  public:
    CaseStmtNode(ConstDefListNode *c_l, StmtListNode *b_p) : const_list(c_l), body_part(b_p) {}

    std::string gen_asm_code();
};

class SwitchStmtNode {
  private:
    ExprNode                   *condition;
    std::vector<CaseStmtNode *> case_list;

  public:
    SwitchStmtNode(ExprNode *c, std::vector<CaseStmtNode *> c_l) : condition(c), case_list(c_l) {}
};

#endif