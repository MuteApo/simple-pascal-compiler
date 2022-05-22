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
class ReadStmtNode;
class WriteStmtNode;

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
    ReadStmtNode   *read_stmt;
    WriteStmtNode  *write_stmt;

  public:
    StmtNode(stmt_type       t,
             StmtListNode   *c_s,
             AssignStmtNode *a_s,
             IfStmtNode     *i_s,
             ForStmtNode    *f_s,
             WhileStmtNode  *wh_s,
             RepeatStmtNode *rep_s,
             SwitchStmtNode *s_s,
             FuncStmtNode   *p_s,
             ReadStmtNode   *rea_s,
             WriteStmtNode  *wr_s);
    StmtNode(StmtListNode *c_s);
    StmtNode(AssignStmtNode *a_s);
    StmtNode(IfStmtNode *i_s);
    StmtNode(ForStmtNode *f_s);
    StmtNode(WhileStmtNode *w_s);
    StmtNode(RepeatStmtNode *r_s);
    StmtNode(SwitchStmtNode *s_s);
    StmtNode(FuncStmtNode *p_s);
    StmtNode(ReadStmtNode *r_s);
    StmtNode(WriteStmtNode *w_s);

    int getUid() {
        return uid;
    }

    std::string gen_viz_code(int run);

    bool test_result_type();

    std::string gen_asm_code() {
        return "";
    }
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

    std::string gen_viz_code(int run);

    bool test_result_type();

    std::string gen_asm_code() {
        std::string asm_code = "";
        for (StmtNode *s : stmts) asm_code += s->gen_asm_code();
        return asm_code;
    }
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

    std::string gen_viz_code(int run);

    bool test_result_type();

    std::string gen_asm_code();
};

class IfStmtNode {
  private:
    int       uid;
    ExprNode *condition;
    StmtNode *then_part;
    StmtNode *else_part;

  public:
    IfStmtNode(ExprNode *e, StmtNode *t_p, StmtNode *e_p = nullptr)
            : uid(++global_uid), condition(e), then_part(t_p), else_part(e_p) {}

    int getUid() {
        return uid;
    }

    std::string gen_viz_code(int run);

    bool test_result_type();

    std::string gen_asm_code();
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

    std::string gen_viz_code(int run);

    bool test_result_type();
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

    std::string gen_viz_code(int run);

    bool test_result_type();

    std::string gen_asm_code();
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

    std::string gen_viz_code(int run);

    bool test_result_type();

    std::string gen_asm_code();
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

    std::string gen_viz_code(int run);

    bool test_result_type();

    std::string gen_asm_code();
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

    std::vector<CaseStmtNode *> &getCaseList() {
        return case_list;
    }

    void addCase(CaseStmtNode *c) {
        case_list.push_back(c);
    }

    std::string gen_viz_code(int run);

    bool test_result_type();
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

    std::string gen_viz_code(int run);

    bool test_result_type();
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

    std::string gen_viz_code(int run);

    bool test_result_type();
};

class ReadStmtNode {
  private:
    int           uid;
    ExprListNode *exprs;

  public:
    ReadStmtNode(ExprListNode *e) : uid(++global_uid), exprs(e) {}

    int getUid() {
        return uid;
    }

    std::string gen_viz_code(int run);

    bool test_result_type();
};

class WriteStmtNode {
  private:
    int           uid;
    bool          is_writeln;
    ExprListNode *exprs;

  public:
    WriteStmtNode(bool is_ln, ExprListNode *e) : uid(++global_uid), is_writeln(is_ln), exprs(e) {}

    int getUid() {
        return uid;
    }

    std::string gen_viz_code(int run);

    bool test_result_type();
};

#endif