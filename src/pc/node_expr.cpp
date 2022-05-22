#include "include/node_expr.hpp"
#include "include/symbol_table.hpp"

ExprNode::ExprNode(expr_node_type nt,
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
ExprNode::ExprNode(ExprEvalType et, ExprNode *op1_, ExprNode *op2_)
        : ExprNode(el_nonleaf, et, op1_, op2_, nullptr, nullptr, nullptr, nullptr) {}
ExprNode::ExprNode(LiteralNode *l_a)
        : ExprNode(el_literal, EK_None, nullptr, nullptr, l_a, nullptr, nullptr, nullptr) {}
ExprNode::ExprNode(VarAccessNode *v_a_a)
        : ExprNode(el_var_access, EK_None, nullptr, nullptr, nullptr, v_a_a, nullptr, nullptr) {}
ExprNode::ExprNode(IdNode *i_a)
        : ExprNode(el_id, EK_None, nullptr, nullptr, nullptr, nullptr, i_a, nullptr) {}
ExprNode::ExprNode(FuncNode *f_a)
        : ExprNode(el_fun_call, EK_None, nullptr, nullptr, nullptr, nullptr, nullptr, f_a) {}

std::string ExprNode::getNodeInfo() {
    std::string result = "ExprNode\n";
    if (node_type == el_nonleaf) result += enum2str(eval_type);
    return result;
}

std::string ExprNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    switch (node_type) {
        case el_nonleaf:
            result += vizChildEdge(uid, op1->getUid(), "op1", "Operand 1");
            result += op1->gen_viz_code(run);
            if (op2 != nullptr) {
                result += vizChildEdge(uid, op2->getUid(), "op2", "Operand 2");
                result += op2->gen_viz_code(run);
            }
            break;
        case el_literal:
            result += vizChildEdge(uid, literal_attr->getUid(), "literal", "Literal");
            result += literal_attr->gen_viz_code(run);
            break;
        case el_var_access:
            result += vizChildEdge(uid, var_access_attr->getUid(), "var access", "Variable Access");
            result += var_access_attr->gen_viz_code(run);
            break;
        case el_id:
            result += vizChildEdge(uid, id_attr->getUid(), "id", "Identifier");
            result += id_attr->gen_viz_code(run);
            break;
        case el_fun_call:
            result += vizChildEdge(uid, func_attr->getUid(), "fun call", "Function Call");
            result += func_attr->gen_viz_code(run);
            break;
    }
    return result;
}

bool ExprNode::is_value_equ(ExprNode *expr) {
    if (node_type != expr->node_type) return false;
    switch (node_type) {
        case el_literal:
            return literal_attr->is_value_equ(expr->literal_attr);
            // TODO: how to compare const expressions?
    }
    return false;
}

std::string ExprListNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "ExprListNode");
    for (int i = 0; i < exprs.size(); i++) {
        result += vizChildEdge(uid,
                               exprs.at(i)->getUid(),
                               "expr" + to_string(i + 1),
                               "Expression " + to_string(i + 1));
        result += exprs.at(i)->gen_viz_code(run);
    }
    return result;
}

LiteralNode::LiteralNode(bool is_n, BasicAttrNode *t, bool bv, int iv, double dv, char cv)
        : uid(++global_uid), is_nil(is_n), type(t), bval(bv), ival(iv), dval(dv), cval(cv) {}
LiteralNode::LiteralNode(void) : LiteralNode(true, nullptr, 0, 0, 0, 0) {}
LiteralNode::LiteralNode(bool b) : LiteralNode(false, new BasicAttrNode(boolean), b, 0, 0, 0) {}
LiteralNode::LiteralNode(int i) : LiteralNode(false, new BasicAttrNode(integer), 0, i, 0, 0) {}
LiteralNode::LiteralNode(double d) : LiteralNode(false, new BasicAttrNode(real), 0, 0, d, 0) {}
LiteralNode::LiteralNode(char c) : LiteralNode(false, new BasicAttrNode(character), 0, 0, 0, c) {}
LiteralNode::~LiteralNode() {
    if (type != nullptr) delete type;
}

bool LiteralNode::operator<(const LiteralNode &rhs) const {
    if (is_nil) return !rhs.is_nil;
    if (type->getType() != rhs.type->getType()) return false;
    switch (type->getType()) {
        case boolean: return bval < rhs.bval;
        case integer: return ival < rhs.ival;
        case real: return dval < rhs.dval;
        case character: return cval < rhs.cval;
    }
    return false;
}

std::string LiteralNode::getNodeInfo() {
    std::string result = "LiteralNode\n";
    if (is_nil) return result + "NIL";
    result += type->getNodeInfo() + "\n";
    if (type->getType() == boolean) return result + to_string(bval);
    if (type->getType() == integer) return result + to_string(ival);
    if (type->getType() == real) return result + to_string(dval);
    if (type->getType() == character) return result + to_string(cval);
    return result + "illegal literal";
}

std::string LiteralNode::gen_viz_code(int run) {
    return vizNode(uid, getNodeInfo());
}

bool LiteralNode::is_value_equ(LiteralNode *expr) {
    if (is_nil && expr->is_nil) return true;
    if (is_nil != expr->is_nil) return false;
    if (type->getType() != expr->type->getType()) return false;
    switch (type->getType()) {
        case boolean: return bval == expr->bval;
        case integer: return ival == expr->ival;
        case real: return dval == expr->dval;
        case character: return cval == expr->cval;
    }
    return false;
}

std::string LiteralNode::toString() {
    switch (type->type) {
        case boolean: return to_string(bval);
        case integer: return to_string(ival);
        case real: return to_string(dval);
        case character: return to_string(cval);
    }
    return "";
}

VarAccessNode::VarAccessNode(var_access_type t, ExprNode *h, ExprListNode *i_l, ExprNode *m)
        : uid(++global_uid), type(t), host(h), index_list(i_l), member(m) {}
VarAccessNode::VarAccessNode(ExprNode *h) : VarAccessNode(va_pointer, h, nullptr, nullptr) {}
VarAccessNode::VarAccessNode(ExprNode *h, ExprListNode *indices)
        : VarAccessNode(va_array, h, indices, nullptr) {}
VarAccessNode::VarAccessNode(ExprNode *h, ExprNode *m) : VarAccessNode(va_record, h, nullptr, m) {}

std::string VarAccessNode::getNodeInfo() {
    std::string result = "VarAccessNode\n";
    if (type == va_pointer) result += "^";
    return result;
}

std::string VarAccessNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    result += vizChildEdge(uid, host->getUid(), "host", "Host Name");
    result += host->gen_viz_code(run);
    if (type == va_array) {
        result += vizChildEdge(uid, index_list->getUid(), "array index", "Array Index");
        result += index_list->gen_viz_code(run);
    }
    if (type == va_record) {
        result += vizChildEdge(uid, member->getUid(), "record field", "Record Field");
        result += member->gen_viz_code(run);
    }
    return result;
}

ConstDefNode *IdNode::getConst() {
    return symbol_table.findConstSymbol(name);
}

TypeAttrNode *IdNode::getType() {
    return symbol_table.findTypeSymbol(name);
}

std::string IdNode::getNodeInfo() {
    return "IdNode\n" + name;
}

std::string IdNode::gen_viz_code(int run) {
    return vizNode(uid, getNodeInfo());
}

std::string FuncNode::getNodeInfo() {
    return "FuncNode\n" + func_name;
}

std::string FuncNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    if (arg_list != nullptr) {
        result += vizChildEdge(uid, arg_list->getUid(), "args", "Arguments of Function Call");
        result += arg_list->gen_viz_code(run);
    }
    return result;
}