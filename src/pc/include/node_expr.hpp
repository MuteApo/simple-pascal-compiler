#ifndef _NODE_EXPR_H_
#define _NODE_EXPR_H_

class ExprNode;
class LiteralNode;
class VarAccessNode;
class IdNode;

#include "defs.hpp"
#include "node_type.hpp"
#include <string>

enum expr_node_type { el_nonleaf = 30001, el_literal, el_var_access, el_id };

class ExprNode {
  private:
    expr_node_type node_type;
    expr_eval_type eval_type;
    ExprNode      *op1, *op2;
    LiteralNode   *literal_attr;
    VarAccessNode *var_access_attr;
    IdNode        *id_attr;

  public:
    ExprNode(expr_node_type nt,
             expr_eval_type et,
             ExprNode      *op_1,
             ExprNode      *op_2,
             LiteralNode   *l_a,
             VarAccessNode *v_a_a,
             IdNode        *i_a)
            : node_type(nt),
              eval_type(et),
              op1(op_1),
              op2(op_2),
              literal_attr(l_a),
              var_access_attr(v_a_a),
              id_attr(i_a) {}
    ExprNode(expr_eval_type et, ExprNode *op1_, ExprNode *op2_)
            : ExprNode(el_nonleaf, et, op1_, op2_, nullptr, nullptr, nullptr) {}
    ExprNode(LiteralNode *l_a)
            : ExprNode(el_literal, EK_None, nullptr, nullptr, l_a, nullptr, nullptr) {}
    ExprNode(VarAccessNode *v_a_a)
            : ExprNode(el_var_access, EK_None, nullptr, nullptr, nullptr, v_a_a, nullptr) {}
    ExprNode(IdNode *i_a)
            : ExprNode(el_literal, EK_None, nullptr, nullptr, nullptr, nullptr, i_a) {}

    bool is_accessible() {
        return node_type == el_var_access || node_type == el_id;
    }

    std::string gen_asm_code(void);
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
    LiteralNode(bool is_n, BasicAttrNode *t, bool bv, int iv, double dv, char cv)
            : is_nil(is_n), type(t), bval(bv), ival(iv), dval(dv), cval(cv) {}
    LiteralNode(void) : LiteralNode(true, nullptr, 0, 0, 0, 0) {}
    LiteralNode(bool b) : LiteralNode(false, new BasicAttrNode(boolean), b, 0, 0, 0) {}
    LiteralNode(int i) : LiteralNode(false, new BasicAttrNode(integer), 0, i, 0, 0) {}
    LiteralNode(double d) : LiteralNode(false, new BasicAttrNode(real), 0, 0, d, 0) {}
    LiteralNode(char c) : LiteralNode(false, new BasicAttrNode(character), 0, 0, 0, c) {}
    ~LiteralNode() {
        if (type != nullptr) delete type;
    }

    BasicAttrNode *get_type() {
        return type;
    }

    template <class T> T get_value() {
        switch (type->type) {
            case boolean: return bval;
            case integer: return ival;
            case real: return dval;
            case character: return cval;
        }
        return "";
    }

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

    TypeAttrNode *get_type();

    std::string gen_asm_code(void);
};

#endif