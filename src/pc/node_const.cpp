#include "include/node_const.hpp"
#include "include/symbol_table.hpp"

std::string ConstDefNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "ConstDefNode\n" + name);
    result += vizChildEdge(uid, expr->getUid(), "constdef", "Const Definition");
    result += expr->gen_viz_code(run);
    return result;
}

bool ConstDefNode::gen_sym_tab() {
    return symbol_table.addSymbol(name, this);
}

std::string ConstDefListNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "ConstDefListNode");
    for (int i = 0; i < const_defs.size(); i++) {
        result += vizChildEdge(uid,
                               const_defs.at(i)->getUid(),
                               "constdef" + to_string(i + 1),
                               "Const Definition " + to_string(i + 1));
        result += const_defs.at(i)->gen_viz_code(run);
    }
    return result;
}

bool ConstDefListNode::gen_sym_tab() {
    bool result = true;
    for (ConstDefNode *def : const_defs) result &= def->gen_sym_tab();
    return result;
}

std::string ConstListNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "ConstDefListNode");
    for (int i = 0; i < const_list.size(); i++) {
        result += vizChildEdge(uid,
                               const_list.at(i)->getUid(),
                               "const" + to_string(i + 1),
                               "Const " + to_string(i + 1));
        result += const_list.at(i)->gen_viz_code(run);
    }
    return result;
}