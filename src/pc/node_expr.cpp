#include "include/node_expr.hpp"
#include "include/symbol_table.hpp"

ExprNode::ExprNode(ExprNode *op1_, ExprListNode *ops)
        : ExprNode(el_nonleaf, EK_Access, op1_, nullptr, nullptr, nullptr, nullptr, nullptr) {
    ExprNode *tmp = op1_;
    for (ExprNode *op2_ : ops->getExprList()) {
        op1 = tmp;
        tmp = new ExprNode(EK_Access, op1, op2_);
        op2 = op2_;
    }
}

std::string ExprNode::getNodeInfo() {
    std::string result = "ExprNode\n";
    if (node_type == el_nonleaf) result += enum2str(eval_type);
    return result;
}

std::string ExprNode::gen_viz_code() {
    std::string result = vizNode(uid, getNodeInfo());
    switch (node_type) {
        case el_nonleaf:
            result += vizChildEdge(uid, op1->getUid());
            result += op1->gen_viz_code();
            if (op2 != nullptr) {
                result += vizChildEdge(uid, op2->getUid());
                result += op2->gen_viz_code();
            }
            break;
        case el_literal:
            result += vizChildEdge(uid, literal_attr->getUid());
            result += literal_attr->gen_viz_code();
            break;
        case el_var_access:
            result += vizChildEdge(uid, var_access_attr->getUid());
            result += var_access_attr->gen_viz_code();
            break;
        case el_id:
            result += vizChildEdge(uid, id_attr->getUid());
            result += id_attr->gen_viz_code();
            break;
        case el_fun_call:
            result += vizChildEdge(uid, func_attr->getUid());
            result += func_attr->gen_viz_code();
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

std::string LiteralNode::getNodeInfo() {
    std::string result = "LiteralNode\n";
    if (is_nil) return result + "NIL";
    result += type->getNodeInfo() + "\n";
    if (type->getType() == boolean) return result + to_string<bool>(bval);
    if (type->getType() == integer) return result + to_string<int>(ival);
    if (type->getType() == real) return result + to_string<double>(dval);
    if (type->getType() == character) return result + to_string<char>(cval);
    return result + "illegal literal";
}

template <class T> T LiteralNode::get_value() {
    switch (type->type) {
        case boolean: return bval;
        case integer: return ival;
        case real: return dval;
        case character: return cval;
    }
    return T();
}

std::string VarAccessNode::getNodeInfo() {
    std::string result = "VarAccessNode\n" + name->getName();
    if (type == va_pointer) result += "^";
    return result;
}

std::string VarAccessNode::gen_viz_code() {
    std::string result = vizNode(uid, getNodeInfo());
    if (type == va_array) {
        result += vizChildEdge(uid, index_list->getUid());
        result += index_list->gen_viz_code();
    }
    if (type == va_record) {
        result += vizChildEdge(uid, member_name->getUid());
        result += member_name->gen_viz_code();
    }
    return result;
}

ConstDefNode *IdNode::getConst() {
    return symbol_table.findConstSymbol(name);
}

TypeAttrNode *IdNode::getType() {
    return symbol_table.findTypeSymbol(name);
}