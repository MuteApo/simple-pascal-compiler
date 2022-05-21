#include "include/node_const.hpp"
#include "include/symbol_table.hpp"

bool ConstDefNode::gen_sym_tab() {
    return symbol_table.addSymbol(name, this);
}

std::string ConstDefNode::gen_viz_code(void) {
    std::string result = vizNode(uid, "ConstDefNode\n" + name);
    result += vizChildEdge(uid, value->getUid());
    result += value->gen_viz_code();
    return result;
}

std::string ConstListNode::gen_viz_code() {
    std::string result = vizNode(uid, "ConstDefListNode");
    for (ExprNode *def : const_list) {
        result += vizChildEdge(uid, def->getUid());
        result += def->gen_viz_code();
    }
    return result;
}