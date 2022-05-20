#include "include/node_expr.hpp"
#include "include/symbol_table.hpp"

ExprNode::ExprNode(ExprNode *op1_, ExprListNode *ops)
        : ExprNode(el_nonleaf, EK_Access, op1_, nullptr, nullptr, nullptr, nullptr, nullptr) {
    for (ExprNode *op2_ : ops->getExprList()) op1 = new ExprNode(EK_Access, op1, op2_);
}

LiteralNode::LiteralNode(bool is_n, BasicAttrNode *t, bool bv, int iv, double dv, char cv)
        : is_nil(is_n), type(t), bval(bv), ival(iv), dval(dv), cval(cv) {}
LiteralNode::LiteralNode(void) : LiteralNode(true, nullptr, 0, 0, 0, 0) {}
LiteralNode::LiteralNode(bool b) : LiteralNode(false, new BasicAttrNode(boolean), b, 0, 0, 0) {}
LiteralNode::LiteralNode(int i) : LiteralNode(false, new BasicAttrNode(integer), 0, i, 0, 0) {}
LiteralNode::LiteralNode(double d) : LiteralNode(false, new BasicAttrNode(real), 0, 0, d, 0) {}
LiteralNode::LiteralNode(char c) : LiteralNode(false, new BasicAttrNode(character), 0, 0, 0, c) {}
LiteralNode::~LiteralNode() {
    if (type != nullptr) delete type;
}

template <class T> T LiteralNode::get_value() {
    switch (type->type) {
        case boolean: return bval;
        case integer: return ival;
        case real: return dval;
        case character: return cval;
    }
    return "";
}

TypeAttrNode *IdNode::get_type() {
    return symbol_table.findTypeSymbol(name);
}