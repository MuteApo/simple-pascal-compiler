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
             FuncNode      *f_a)
            : uid(++global_uid),
              node_type(nt),
              eval_type(et),
              op1(op_1),
              op2(op_2),
              literal_attr(l_a),
              var_access_attr(v_a_a),
              id_attr(i_a),
              func_attr(f_a) {}
    ExprNode(ExprEvalType et, ExprNode *op1_, ExprNode *op2_)
            : ExprNode(el_nonleaf, et, op1_, op2_, nullptr, nullptr, nullptr, nullptr) {}
    ExprNode(LiteralNode *l_a)
            : ExprNode(el_literal, EK_None, nullptr, nullptr, l_a, nullptr, nullptr, nullptr) {}
    ExprNode(VarAccessNode *v_a_a)
            : ExprNode(el_var_access, EK_None, nullptr, nullptr, nullptr, v_a_a, nullptr, nullptr) {
    }
    ExprNode(IdNode *i_a)
            : ExprNode(el_id, EK_None, nullptr, nullptr, nullptr, nullptr, i_a, nullptr) {}
    ExprNode(FuncNode *f_a)
            : ExprNode(el_fun_call, EK_None, nullptr, nullptr, nullptr, nullptr, nullptr, f_a) {}

    int getUid() {
        return uid;
    }

    expr_node_type getExprType() {
        return node_type;
    }

    IdNode *getIdNode() {
        return id_attr;
    }

    LiteralNode *getLiteralNode() {
        return literal_attr;
    }

    VarAccessNode *getVarAccessNode() {
        return var_access_attr;
    }

    std::string getNodeInfo();

    std::string gen_viz_code();

    bool is_accessible() {
        return node_type == el_var_access || node_type == el_id;
    }

    bool is_value_equ(ExprNode *expr);

    std::string gen_asm_code(void);
};

class ExprListNode {
  private:
    int                     uid;
    std::vector<ExprNode *> exprs;

  public:
    ExprListNode() : uid(++global_uid) {
        exprs.clear();
    }

    int getUid() {
        return uid;
    }

    void addExpr(ExprNode *expr) {
        exprs.push_back(expr);
    }

    std::vector<ExprNode *> &getExprList() {
        return exprs;
    }

    std::string gen_viz_code() {
        std::string result = vizNode(uid, "ExprListNode");
        for (int i = 0; i < exprs.size(); i++) {
            result += vizChildEdge(uid,
                                   exprs.at(i)->getUid(),
                                   "expr" + to_string(i + 1),
                                   "Expression " + to_string(i + 1));
            result += exprs.at(i)->gen_viz_code();
        }
        return result;
    }
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

    int getUid() {
        return uid;
    }

    BasicAttrNode *getType() {
        return type;
    }

    bool is_value_equ(LiteralNode *expr);

    std::string getNodeInfo();

    std::string gen_viz_code() {
        return vizNode(uid, getNodeInfo());
    }

    std::string get_value_string();

    std::string gen_asm_code(void);
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
    VarAccessNode(var_access_type t, ExprNode *h, ExprListNode *i_l, ExprNode *m)
            : uid(++global_uid), type(t), host(h), index_list(i_l), member(m) {}
    VarAccessNode(ExprNode *h) : VarAccessNode(va_pointer, h, nullptr, nullptr) {}
    VarAccessNode(ExprNode *h, ExprListNode *indices)
            : VarAccessNode(va_array, h, indices, nullptr) {}
    VarAccessNode(ExprNode *h, ExprNode *m) : VarAccessNode(va_record, h, nullptr, m) {}

    int getUid() {
        return uid;
    }

    std::string gen_asm_code(void);

    std::string getNodeInfo();

    std::string gen_viz_code();
};

class IdNode {
  private:
    int         uid;
    std::string name;

  public:
    IdNode(std::string id) : uid(++global_uid), name(id) {}

    int getUid() {
        return uid;
    }

    std::string getName() {
        return name;
    }
    ConstDefNode *getConst();

    TypeAttrNode *getType();

    std::string getNodeInfo() {
        return "IdNode\n" + name;
    }

    std::string gen_viz_code() {
        return vizNode(uid, getNodeInfo());
    }

    std::string gen_asm_code(void);
};

class IdListNode {
  private:
    int                   uid;
    std::vector<IdNode *> ids;

  public:
    IdListNode() : uid(++global_uid) {
        ids.clear();
    }

    int getUid() {
        return uid;
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
    int           uid;
    std::string   func_name;
    ExprListNode *arg_list;

  public:
    FuncNode(std::string id, ExprListNode *a_l) : uid(++global_uid), func_name(id), arg_list(a_l) {}

    int getUid() {
        return uid;
    }

    std::string getNodeInfo() {
        return "FuncNode\n" + func_name;
    }

    std::string gen_viz_code() {
        std::string result = vizNode(uid, getNodeInfo());
        if (arg_list != nullptr) {
            result += vizChildEdge(uid, arg_list->getUid(), "args", "Arguments of Function Call");
            result += arg_list->gen_viz_code();
        }
        return result;
    }
};

#endif