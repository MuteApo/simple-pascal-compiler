#ifndef __NODE_EXPR_H__
#define __NODE_EXPR_H__

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
#include <string>
#include <vector>

extern int global_uid;
extern int yylineno;

class ExprNode {
  private:
    int            uid;
    int            line_no;
    ExprNodeType   node_type;
    ExprEvalType   eval_type;
    ExprNode      *op1, *op2;
    LiteralNode   *literal_attr;
    VarAccessNode *var_access_attr;
    IdNode        *id_attr;
    FuncNode      *func_attr;
    TypeAttrNode  *res_type;

  public:
    ExprNode(ExprNodeType   nt,
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

    int getLineNumber();

    ExprNodeType getNodeType();

    std::string getExprTypeString(ExprEvalType e);

    IdNode *getIdNode();

    LiteralNode *getLiteralNode();

    VarAccessNode *getVarAccessNode();

    std::string getNodeInfo();

    std::string genVizCode(int run);

    TypeAttrNode *getResultType();

    bool isValueEqual(ExprNode *expr);

    bool isMemAccessRoot();

    std::string genAsmCodeMemAccess(bool access_write);

    // For continuous expression evaluation
    std::string genAsmCodeRHS();

    // Only for memory write operation
    std::string genAsmCodeLHS();
};

class ExprListNode {
  private:
    int                     uid;
    int                     line_no;
    std::vector<ExprNode *> exprs;

  public:
    ExprListNode();

    int getUid();

    int getDim();

    int getLineNumber();

    std::vector<ExprNode *> &getExprList();

    void addExpr(ExprNode *expr);

    std::string genVizCode(int run);
};

class LiteralNode {
  private:
    int            uid;
    int            line_no;
    LiteralType    literal_type;
    BasicAttrNode *type;
    bool           bval;
    int            ival;
    double         dval;
    char           cval;
    std::string    sval;
    int            slen;
    std::string    ilabel;  // string literal internal label

  public:
    LiteralNode(LiteralType    l_t,
                BasicAttrNode *t,
                bool           bv,
                int            iv,
                double         dv,
                char           cv,
                std::string    sv,
                std::string    il);
    LiteralNode(void);
    LiteralNode(bool b);
    LiteralNode(int i);
    LiteralNode(double d);
    LiteralNode(char c);
    LiteralNode(std::string s, std::string l);
    ~LiteralNode();

    bool operator<(const LiteralNode &rhs) const;

    int getUid();

    BasicAttrNode *getType();

    std::string getNodeInfo();

    std::string genVizCode(int run);

    TypeAttrNode *getResultType();

    int diff(LiteralNode *rhs);

    bool isValueEqual(LiteralNode *expr);

    // Load immediate into register
    std::string genAsmCode(void);

    std::string toString();
};

class VarAccessNode {
  private:
    int           uid;
    int           line_no;
    VarAccessType type;
    ExprNode     *host;
    ExprListNode *index_list;  // array
    ExprNode     *member;      // record
    TypeAttrNode *res_type;

  public:
    VarAccessNode(VarAccessType t, ExprNode *h, ExprListNode *i_l, ExprNode *m);
    VarAccessNode(ExprNode *h);
    VarAccessNode(ExprNode *h, ExprListNode *indices);
    VarAccessNode(ExprNode *h, ExprNode *m);

    int getUid();

    std::string getNodeInfo();

    std::string genVizCode(int run);

    TypeAttrNode *getResultType();

    bool testAccess();

    std::string genAsmCode(void);
};

class IdNode {
  private:
    int           uid;
    int           line_no;
    std::string   name;
    TypeAttrNode *res_type;

  public:
    IdNode(std::string id);

    int getUid();

    std::string getName();

    ConstDefNode *getConst();

    TypeAttrNode *getType();

    std::string getNodeInfo();

    std::string genVizCode(int run);

    TypeAttrNode *getResultType();

    std::string genAsmCode(void);
};

class IdListNode {
  private:
    int                   uid;
    int                   line_no;
    std::vector<IdNode *> ids;

  public:
    IdListNode();

    int getUid();

    std::vector<IdNode *> &getIdList();

    std::string getIdString(std::string sep);

    void addId(IdNode *id);
};

class FuncNode {
  private:
    int           uid;
    int           line_no;
    std::string   func_name;
    ExprListNode *arg_list;
    TypeAttrNode *res_type;

  public:
    FuncNode(std::string id, ExprListNode *a_l);

    int getUid();

    std::string getName();

    std::string getNodeInfo();

    std::string genVizCode(int run);

    bool testArgType();

    TypeAttrNode *getResultType();
};

#endif