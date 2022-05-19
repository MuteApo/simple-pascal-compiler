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
#include "node_type.hpp"
#include <string>
#include <vector>

enum expr_node_type { el_nonleaf = 30001, el_literal, el_var_access, el_id, el_fun_call };

class ExprNode {
  private:
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
             FuncNode      *f_a)
            : node_type(nt),
              eval_type(et),
              op1(op_1),
              op2(op_2),
              literal_attr(l_a),
              var_access_attr(v_a_a),
              id_attr(i_a),
              func_attr(f_a) {}
    ExprNode(ExprEvalType et, ExprNode *op1_, ExprNode *op2_)
            : ExprNode(el_nonleaf, et, op1_, op2_, nullptr, nullptr, nullptr, nullptr) {}
    ExprNode(ExprEvalType et, ExprNode *op1_, ExprListNode *ops);
    ExprNode(LiteralNode *l_a)
            : ExprNode(el_literal, EK_None, nullptr, nullptr, l_a, nullptr, nullptr, nullptr) {}
    ExprNode(VarAccessNode *v_a_a)
            : ExprNode(el_var_access, EK_None, nullptr, nullptr, nullptr, v_a_a, nullptr, nullptr) {
    }
    ExprNode(IdNode *i_a)
            : ExprNode(el_id, EK_None, nullptr, nullptr, nullptr, nullptr, i_a, nullptr) {}
    ExprNode(FuncNode *f_a)
            : ExprNode(el_fun_call, EK_None, nullptr, nullptr, nullptr, nullptr, nullptr, f_a) {}

    IdNode *getIdNode() {
        return id_attr;
    }

    bool is_accessible() {
        return node_type == el_var_access || node_type == el_id;
    }

    std::string gen_asm_code(void);
};

class ExprListNode {
  private:
    std::vector<ExprNode *> exprs;

  public:
    ExprListNode(std::vector<ExprNode *> v) : exprs(v) {}
    ExprListNode() {
        exprs.clear();
    }

    void addExpr(ExprNode *expr) {
        exprs.push_back(expr);
    }

    std::vector<ExprNode *> &getExprList() {
        return exprs;
    }
};

class LiteralNode {
  private:
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

    BasicAttrNode *get_type() {
        return type;
    }

    template <class T> T get_value();

    std::string gen_asm_code(void);
};

enum var_access_type { va_pointer = 40001, va_array, va_record };
class VarAccessNode {
  private:
    var_access_type         type;
    std::string             name;
    std::vector<ExprNode *> index_list;   // array
    std::string             member_name;  // record

  public:
    VarAccessNode(std::string id) : name(id) {}
    VarAccessNode(std::string id, std::vector<ExprNode *> indices)
            : name(id), index_list(indices) {}
    VarAccessNode(std::string id, std::string m_id) : name(id), member_name(m_id) {}

    std::string gen_asm_code(void);
};

class IdNode {
  private:
    std::string name;

  public:
    IdNode(std::string id) : name(id) {}

    std::string getName() {
        return name;
    }

    TypeAttrNode *get_type();

    std::string gen_asm_code(void);
};

class IdListNode {
  private:
    std::vector<IdNode *> ids;

  public:
    IdListNode(std::vector<IdNode *> v) : ids(v) {}
    IdListNode() {
        ids.clear();
    }

    void addId(IdNode *id) {
        ids.push_back(id);
    }

    std::vector<IdNode *> &getIdList() {
        return ids;
    }
};

class FuncNode {
  private:
    std::string   func_name;
    ExprListNode *arg_list;

  public:
    FuncNode(std::string id, ExprListNode *a_l) : func_name(id), arg_list(a_l) {}
};

#endif