#include "include/node_func.hpp"

std::string FuncDefNode::gen_viz_code() {
    std::string result = vizNode(uid, "FuncDefNode\n" + name);
    if (is_func) {
        result += vizChildEdge(uid, retval_type->getUid());
        result += retval_type->gen_viz_code();
    }
    if (param_defs != nullptr) {
        result += vizChildEdge(uid, param_defs->getUid());
        result += param_defs->gen_viz_code();
    }
    if (block != nullptr) {
        result += vizChildEdge(uid, block->getUid());
        result += block->gen_viz_code();
    }
    return result;
}