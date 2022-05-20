#include "include/node_block.hpp"

std::vector<int> ar_lvars_length;
std::vector<int> ar_args_length;

std::string BlockNode::gen_viz_code(void) {
    std::string result = vizNode(uid, "BlockNode");
    if (const_defs != nullptr) {
        result += vizChildEdge(uid, const_defs->getUid());
        result += const_defs->gen_viz_code();
    }
    if (type_defs != nullptr) {
        result += vizChildEdge(uid, type_defs->getUid());
        result += type_defs->gen_viz_code();
    }
    if (var_defs != nullptr) {
        result += vizChildEdge(uid, var_defs->getUid());
        result += var_defs->gen_viz_code();
    }
    if (func_defs != nullptr) {
        result += vizChildEdge(uid, func_defs->getUid());
        result += func_defs->gen_viz_code();
    }
    if (stmts != nullptr) {
        result += vizChildEdge(uid, stmts->getUid());
        result += stmts->gen_viz_code();
    }
    return result;
}