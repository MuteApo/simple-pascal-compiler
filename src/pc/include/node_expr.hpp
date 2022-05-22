#ifndef _NODE_EXPR_H_
#define _NODE_EXPR_H_

class ExprNode;
class ExprListNode;
class LiteralNode;
class VarAccessNode;
class IdNode;
class IdListNode;
class FuncNode;

#include "defs.hpp"
#include "node_const.hpp"
#include "node_type.hpp"
#include "viz.hpp"
#include <string>
#include <vector>

extern int global_uid;

enum expr_node_type { el_nonleaf = 30001, el_literal, el_var_access, el_id, el_fun_call };

class ExprNode {
  private:
    int            uid;
    expr_node_type node_type;
    ExprEvalType   eval_type;
    ExprNode      *op1, *op2;
    LiteralNode   *literal_attr;
    VarAccessNode *var_access_attr;
    IdNode        *id_attr;
    FuncNode      *func_attr;

  public:
    ExprNode(expr_node_type nt,
             ExprEvalType   et,
             ExprNode      *op_1,
             ExprNode      *op_2,
             LiteralNode   *l_a,
             VarAccessNode *v_a_a,
             IdNode        *i_a,
             FuncNode      *f_a);
    ExprNode(ExprEvalType et, ExprNode *op1_, ExprNode *op2_);
    ExprNode(LiteralNode *l_a);
    ExprNode(VarAccessNode *v_a_a);
    ExprNode(IdNode *i_a);
    ExprNode(FuncNode *f_a);

    int getUid();

    expr_node_type getExprType();

    IdNode *getIdNode();

    LiteralNode *getLiteralNode();

    VarAccessNode *getVarAccessNode();

    std::string getNodeInfo();

    std::string gen_viz_code(int run);

    bool is_value_equ(ExprNode *expr);

    std::string gen_asm_code(void);
};

class ExprListNode {
  private:
    int                     uid;
    std::vector<ExprNode *> exprs;

  public:
    ExprListNode();

    int getUid();

    std::vector<ExprNode *> &getExprList();

    void addExpr(ExprNode *expr);

    std::string gen_viz_code(int run);
};

class LiteralNode {
  private:
    int            uid;
    bool           is_nil;
    BasicAttrNode *type;
    bool           bval;
    int            ival;
    double         dval;
    char           cval;

  public:
    LiteralNode(bool is_n, BasicAttrNode *t, bool bv, int iv, double dv, char cv);
    LiteralNode(void);
    LiteralNode(bool b);
    LiteralNode(int i);
    LiteralNode(double d);
    LiteralNode(char c);
    ~LiteralNode();

    bool operator<(const LiteralNode &rhs) const;

    int getUid();

    BasicAttrNode *getType();

    std::string getNodeInfo();

    std::string gen_viz_code(int run);

    int diff(LiteralNode *rhs);

    bool is_value_equ(LiteralNode *expr);

    std::string gen_asm_code(void);

    std::string toString();
};

enum var_access_type { va_pointer = 40001, va_array, va_record };
class VarAccessNode {
  private:
    int             uid;
    var_access_type type;
    ExprNode       *host;
    ExprListNode   *index_list;  // array
    ExprNode       *member;      // record

  public:
    VarAccessNode(var_access_type t, ExprNode *h, ExprListNode *i_l, ExprNode *m);
    VarAccessNode(ExprNode *h);
    VarAccessNode(ExprNode *h, ExprListNode *indices);
    VarAccessNode(ExprNode *h, ExprNode *m);

    int getUid();

    std::string getNodeInfo();

    std::string gen_viz_code(int run);

    std::string gen_asm_code();
};

class IdNode {
  private:
    int         uid;
    std::string name;

  public:
    IdNode(std::string id);

    int getUid();

    std::string getName();

    ConstDefNode *getConst();

    TypeAttrNode *getType();

    std::string getNodeInfo();

    std::string gen_viz_code(int run);

    std::string gen_asm_code(void);
};

class IdListNode {
  private:
    int                   uid;
    std::vector<IdNode *> ids;

  public:
    IdListNode();

    int getUid();

    std::vector<IdNode *> &getIdList();

    void addId(IdNode *id);
};

class FuncNode {
  private:
    int           uid;
    std::string   func_name;
    ExprListNode *arg_list;

  public:
    FuncNode(std::string id, ExprListNode *a_l);

    int getUid();

    std::string getNodeInfo();

    std::string gen_viz_code(int run);
};

#endif