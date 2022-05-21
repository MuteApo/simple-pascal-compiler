#ifndef _NODE_STMT_H_
#define _NODE_STMT_H_

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

#include "defs.hpp"
#include "node_const.hpp"
#include "viz.hpp"
#include <string>
#include <vector>

extern int global_uid;

class StmtNode {
  private:
    int             uid;
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
            : uid(++global_uid),
              type(t),
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
                  SK_Repeat, nullptr, nullptr, nullptr, nullptr, nullptr, r_s, nullptr, nullptr) {}
    StmtNode(SwitchStmtNode *s_s)
            : StmtNode(
                  SK_Switch, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, s_s, nullptr) {}
    StmtNode(FuncStmtNode *p_s)
            : StmtNode(
                  SK_Func, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, p_s) {}

    int getUid() {
        return uid;
    }

    std::string gen_asm_code() {
        return "";
    }

    std::string gen_viz_code();
};

class StmtListNode {
  private:
    int                     uid;
    std::vector<StmtNode *> stmts;

  public:
    StmtListNode() : uid(++global_uid) {
        stmts.clear();
    }

    int getUid() {
        return uid;
    }

    void addStmt(StmtNode *stmt) {
        stmts.push_back(stmt);
    }

    std::string gen_asm_code() {
        std::string asm_code = "";
        for (StmtNode *s : stmts) asm_code += s->gen_asm_code();
        return asm_code;
    }

    std::string gen_viz_code();
};

class AssignStmtNode {
  private:
    int       uid;
    ExprNode *dst;
    ExprNode *src;

  public:
    AssignStmtNode(ExprNode *d, ExprNode *s) : uid(++global_uid), dst(d), src(s) {}

    int getUid() {
        return uid;
    }

    std::string gen_asm_code();

    std::string gen_viz_code();
};

class IfStmtNode {
  private:
    int       uid;
    ExprNode *expr;
    StmtNode *then_part;
    StmtNode *else_part;

  public:
    IfStmtNode(ExprNode *e, StmtNode *t_p, StmtNode *e_p = nullptr)
            : uid(++global_uid), expr(e), then_part(t_p), else_part(e_p) {}

    int getUid() {
        return uid;
    }

    std::string gen_asm_code();

    std::string gen_viz_code();
};

class ForStmtNode {
  private:
    int         uid;
    std::string name;
    bool        is_to;
    ExprNode   *start_expr;
    ExprNode   *end_expr;
    StmtNode   *body_part;

  public:
    ForStmtNode(std::string id, bool is_t, ExprNode *s_e, ExprNode *e_e, StmtNode *b_p)
            : uid(++global_uid),
              name(id),
              is_to(is_t),
              start_expr(s_e),
              end_expr(e_e),
              body_part(b_p) {}

    int getUid() {
        return uid;
    }

    std::string getNodeInfo() {
        std::string result = "ForStmtNode\n" + name;
        return result + (is_to ? "\n→" : "\n←");
    }

    std::string gen_viz_code();
};

class WhileStmtNode {
  private:
    int       uid;
    ExprNode *condition;
    StmtNode *body_part;

  public:
    WhileStmtNode(ExprNode *c, StmtNode *b_p) : uid(++global_uid), condition(c), body_part(b_p) {}

    int getUid() {
        return uid;
    }

    std::string gen_asm_code();

    std::string gen_viz_code();
};

class RepeatStmtNode {
  private:
    int           uid;
    StmtListNode *body_part;
    ExprNode     *condition;

  public:
    RepeatStmtNode(StmtListNode *b_p, ExprNode *c)
            : uid(++global_uid), body_part(b_p), condition(c) {}

    int getUid() {
        return uid;
    }

    std::string gen_asm_code();

    std::string gen_viz_code();
};

class CaseStmtNode {
  private:
    int            uid;
    ConstListNode *const_list;
    StmtNode      *body_part;

  public:
    CaseStmtNode(ConstListNode *c_l, StmtNode *b_p)
            : uid(++global_uid), const_list(c_l), body_part(b_p) {}

    int getUid() {
        return uid;
    }

    std::string gen_asm_code();

    std::string gen_viz_code();
};

class CaseListNode {
  private:
    int                         uid;
    std::vector<CaseStmtNode *> case_list;

  public:
    CaseListNode() : uid(++global_uid) {
        case_list.clear();
    }

    int getUid() {
        return uid;
    }

    void addCase(CaseStmtNode *c) {
        case_list.push_back(c);
    }

    std::vector<CaseStmtNode *> &getCaseList() {
        return case_list;
    }

    std::string gen_viz_code() {
        std::string result = vizNode(uid, "CaseListNode");
        for (CaseStmtNode *stmt : case_list) {
            result += vizChildEdge(uid, stmt->getUid());
            result += stmt->gen_viz_code();
        }
        return result;
    }
};

class SwitchStmtNode {
  private:
    int           uid;
    ExprNode     *condition;
    CaseListNode *case_list;

  public:
    SwitchStmtNode(ExprNode *c, CaseListNode *c_l)
            : uid(++global_uid), condition(c), case_list(c_l) {}

    int getUid() {
        return uid;
    }

    std::string gen_viz_code();
};

class FuncStmtNode {
  private:
    int       uid;
    FuncNode *func;

  public:
    FuncStmtNode(FuncNode *f) : uid(++global_uid), func(f) {}

    int getUid() {
        return uid;
    }

    std::string gen_viz_code();
};

#endif