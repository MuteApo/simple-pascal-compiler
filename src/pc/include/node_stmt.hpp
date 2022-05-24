#ifndef __NODE_STMT_H__
#define __NODE_STMT_H__

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
#include "node_expr.hpp"
#include "node_func.hpp"
#include <string>
#include <vector>

extern int global_uid;
extern int yylineno;

class StmtNode {
  private:
    int             uid;
    int             line_no;
    StmtType        type;
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
    StmtNode(StmtType        t,
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

    int getUid();

    std::string genVizCode(int run);

    bool testExprType();

    std::string genAsmCode();
};

class StmtListNode {
  private:
    int                     uid;
    int                     line_no;
    std::vector<StmtNode *> stmts;

  public:
    StmtListNode();

    int getUid();

    void addStmt(StmtNode *stmt);

    std::string genVizCode(int run);

    bool testExprType();

    std::string genAsmCode();
};

class AssignStmtNode {
  private:
    int       uid;
    int       line_no;
    ExprNode *dst;
    ExprNode *src;

  public:
    AssignStmtNode(ExprNode *d, ExprNode *s);

    int getUid();

    std::string genVizCode(int run);

    bool testExprType();

    std::string genAsmCode();
};

class IfStmtNode {
  private:
    int       uid;
    int       line_no;
    ExprNode *condition;
    StmtNode *then_part;
    StmtNode *else_part;

  public:
    IfStmtNode(ExprNode *e, StmtNode *t_p, StmtNode *e_p = nullptr);

    int getUid();

    std::string genVizCode(int run);

    bool testExprType();

    std::string genAsmCode();
};

class ForStmtNode {
  private:
    int         uid;
    int         line_no;
    std::string name;
    bool        is_to;
    ExprNode   *start_expr;
    ExprNode   *end_expr;
    StmtNode   *body_part;

  public:
    ForStmtNode(std::string id, bool is_t, ExprNode *s_e, ExprNode *e_e, StmtNode *b_p);

    int getUid();

    std::string getNodeInfo();

    std::string genVizCode(int run);

    bool testExprType();
};

class WhileStmtNode {
  private:
    int       uid;
    int       line_no;
    ExprNode *condition;
    StmtNode *body_part;

  public:
    WhileStmtNode(ExprNode *c, StmtNode *b_p);

    int getUid();

    std::string genVizCode(int run);

    bool testExprType();

    std::string genAsmCode();
};

class RepeatStmtNode {
  private:
    int           uid;
    int           line_no;
    StmtListNode *body_part;
    ExprNode     *condition;

  public:
    RepeatStmtNode(StmtListNode *b_p, ExprNode *c);

    int getUid();

    std::string genVizCode(int run);

    bool testExprType();

    std::string genAsmCode();
};

class CaseStmtNode {
  private:
    int            uid;
    int            line_no;
    ConstListNode *const_list;
    StmtNode      *body_part;

  public:
    CaseStmtNode(ConstListNode *c_l, StmtNode *b_p);

    int getUid();

    std::string genVizCode(int run);

    bool testExprType();

    std::string genAsmCode();
};

class CaseListNode {
  private:
    int                         uid;
    int                         line_no;
    std::vector<CaseStmtNode *> case_list;

  public:
    CaseListNode();

    int getUid();

    std::vector<CaseStmtNode *> &getCaseList();

    void addCase(CaseStmtNode *c);

    std::string genVizCode(int run);

    bool testExprType();
};

class SwitchStmtNode {
  private:
    int           uid;
    int           line_no;
    ExprNode     *condition;
    CaseListNode *case_list;

  public:
    SwitchStmtNode(ExprNode *c, CaseListNode *c_l);

    int getUid();

    std::string genVizCode(int run);

    bool testExprType();
};

class FuncStmtNode {
  private:
    int       uid;
    int       line_no;
    FuncNode *func;

  public:
    FuncStmtNode(FuncNode *f);

    int getUid();

    std::string genVizCode(int run);

    bool testExprType();
};

class ReadStmtNode {
  private:
    int           uid;
    int           line_no;
    ExprListNode *exprs;

  public:
    ReadStmtNode(ExprListNode *e);

    int getUid();

    std::string genVizCode(int run);

    bool testExprType();
};

class WriteStmtNode {
  private:
    int           uid;
    int           line_no;
    bool          is_writeln;
    ExprListNode *exprs;

  public:
    WriteStmtNode(bool is_ln, ExprListNode *e);

    int getUid();

    std::string genVizCode(int run);

    bool testExprType();
};

#endif