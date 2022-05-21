#include "include/node_const.hpp"
#include "include/symbol_table.hpp"

bool ConstDefNode::gen_sym_tab() {
    return symbol_table.addSymbol(name, this);
}

std::string ConstDefNode::gen_viz_code(void) {
    std::string result = vizNode(uid, "ConstDefNode\n" + name);
    result += vizChildEdge(uid, value->getUid(), "constdef", "Const Definition");
    result += value->gen_viz_code();
    return result;
}

std::string ConstListNode::gen_viz_code() {
    std::string result = vizNode(uid, "ConstDefListNode");
    for (int i = 0; i < const_list.size(); i++) {
        result += vizChildEdge(uid,
                               const_list.at(i)->getUid(),
                               "const" + to_string(i + 1),
                               "Const " + to_string(i + 1));
        result += const_list.at(i)->gen_viz_code();
    }
    return result;
}