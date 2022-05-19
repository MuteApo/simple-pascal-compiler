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
class CaseListNode;
class SwitchStmtNode;
class FuncStmtNode;

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
    FuncStmtNode   *func_stmt;

  public:
    StmtNode(stmt_type       t,
             StmtListNode   *c_s,
             AssignStmtNode *a_s,
             IfStmtNode     *i_s,
             ForStmtNode    *f_s,
             WhileStmtNode  *w_s,
             RepeatStmtNode *r_s,
             SwitchStmtNode *s_s,
             FuncStmtNode   *p_s)
            : type(t),
              compound_stmt(c_s),
              assign_stmt(a_s),
              if_stmt(i_s),
              for_stmt(f_s),
              while_stmt(w_s),
              repeat_stmt(r_s),
              switch_stmt(s_s),
              func_stmt(p_s) {}
    StmtNode(StmtListNode *c_s)
            : StmtNode(
                  SK_Compound, c_s, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) {
    }
    StmtNode(AssignStmtNode *a_s)
            : StmtNode(
                  SK_Assign, nullptr, a_s, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) {}
    StmtNode(IfStmtNode *i_s)
            : StmtNode(SK_If, nullptr, nullptr, i_s, nullptr, nullptr, nullptr, nullptr, nullptr) {}
    StmtNode(ForStmtNode *f_s)
            : StmtNode(SK_For, nullptr, nullptr, nullptr, f_s, nullptr, nullptr, nullptr, nullptr) {
    }
    StmtNode(WhileStmtNode *w_s)
            : StmtNode(
                  SK_While, nullptr, nullptr, nullptr, nullptr, w_s, nullptr, nullptr, nullptr) {}
    StmtNode(RepeatStmtNode *r_s)
            : StmtNode(
                  SK_While, nullptr, nullptr, nullptr, nullptr, nullptr, r_s, nullptr, nullptr) {}
    StmtNode(SwitchStmtNode *s_s)
            : StmtNode(
                  SK_While, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, s_s, nullptr) {}
    StmtNode(FuncStmtNode *p_s)
            : StmtNode(
                  SK_While, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, p_s) {}

    std::string gen_asm_code();
};

class StmtListNode {
  private:
    std::vector<StmtNode *> stmts;

  public:
    StmtListNode() {
        stmts.clear();
    }

    void addStmt(StmtNode *stmt) {
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
    ExprNode *expr;
    StmtNode *then_part;
    StmtNode *else_part;

  public:
    IfStmtNode(ExprNode *e, StmtNode *t_p, StmtNode *e_p = nullptr)
            : expr(e), then_part(t_p), else_part(e_p) {}

    std::string gen_asm_code();
};

class ForStmtNode {  // TODO
  private:
    std::string name;
    bool        is_to;
    ExprNode   *start_expr;
    ExprNode   *end_expr;
    StmtNode   *body_part;

  public:
    ForStmtNode(std::string id, bool is_t, ExprNode *s_e, ExprNode *e_e, StmtNode *b_p)
            : name(id), is_to(is_t), start_expr(s_e), end_expr(e_e), body_part(b_p) {}
};

class WhileStmtNode {
  private:
    ExprNode *condition;
    StmtNode *body_part;

  public:
    WhileStmtNode(ExprNode *c, StmtNode *b_p) : condition(c), body_part(b_p) {}

    std::string gen_asm_code();
};

class RepeatStmtNode {
  private:
    StmtListNode *body_part;
    ExprNode     *condition;

  public:
    RepeatStmtNode(StmtListNode *b_p, ExprNode *c) : body_part(b_p), condition(c) {}

    std::string gen_asm_code();
};

class CaseStmtNode {
  private:
    ConstListNode *const_list;
    StmtNode      *body_part;

  public:
    CaseStmtNode(ConstListNode *c_l, StmtNode *b_p) : const_list(c_l), body_part(b_p) {}

    std::string gen_asm_code();
};

class CaseListNode {
  private:
    std::vector<CaseStmtNode *> case_list;

  public:
    CaseListNode() {
        case_list.clear();
    }

    void addCase(CaseStmtNode *c) {
        case_list.push_back(c);
    }

    std::vector<CaseStmtNode *> &getCaseList() {
        return case_list;
    }
};

class SwitchStmtNode {
  private:
    ExprNode     *condition;
    CaseListNode *case_list;

  public:
    SwitchStmtNode(ExprNode *c, CaseListNode *c_l) : condition(c), case_list(c_l) {}
};

class FuncStmtNode {
  private:
    FuncNode *func;

  public:
    FuncStmtNode(FuncNode *f) : func(f) {}
};

#endif