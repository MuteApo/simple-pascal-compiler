#include "include/node_stmt.hpp"

std::string StmtNode::gen_viz_code() {
    std::string result = vizNode(uid, "StmtNode");
    switch (type) {
        case SK_Compound:
            result += vizChildEdge(uid, compound_stmt->getUid());
            result += compound_stmt->gen_viz_code();
            break;
        case SK_Assign:
            result += vizChildEdge(uid, assign_stmt->getUid());
            result += assign_stmt->gen_viz_code();
            break;
        case SK_If:
            result += vizChildEdge(uid, if_stmt->getUid());
            result += if_stmt->gen_viz_code();
            break;
        case SK_For:
            result += vizChildEdge(uid, for_stmt->getUid());
            result += for_stmt->gen_viz_code();
            break;
        case SK_While:
            result += vizChildEdge(uid, while_stmt->getUid());
            result += while_stmt->gen_viz_code();
            break;
        case SK_Repeat:
            result += vizChildEdge(uid, repeat_stmt->getUid());
            result += repeat_stmt->gen_viz_code();
            break;
        case SK_Switch:
            result += vizChildEdge(uid, switch_stmt->getUid());
            result += switch_stmt->gen_viz_code();
            break;
        case SK_Func:
            result += vizChildEdge(uid, func_stmt->getUid());
            result += func_stmt->gen_viz_code();
            break;
    }
    return result;
}