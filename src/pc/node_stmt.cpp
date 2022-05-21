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

std::string StmtListNode::gen_viz_code() {
    std::string result = vizNode(uid, "StmtListNode");
    for (StmtNode *stmt : stmts) {
        result += vizChildEdge(uid, stmt->getUid());
        result += stmt->gen_viz_code();
    }
    return result;
}

std::string AssignStmtNode::gen_viz_code() {
    std::string result = vizNode(uid, "AssignStmtNode");
    result += vizChildEdge(uid, dst->getUid());
    result += dst->gen_viz_code();
    result += vizChildEdge(uid, src->getUid());
    result += src->gen_viz_code();
    return result;
}

std::string IfStmtNode::gen_viz_code() {
    std::string result = vizNode(uid, "IfStmtNode");
    result += vizChildEdge(uid, expr->getUid());
    result += expr->gen_viz_code();
    result += vizChildEdge(uid, then_part->getUid());
    result += then_part->gen_viz_code();
    if (else_part != nullptr) {
        result += vizChildEdge(uid, else_part->getUid());
        result += else_part->gen_viz_code();
    }
    return result;
}

std::string ForStmtNode::gen_viz_code() {
    std::string result = vizNode(uid, getNodeInfo());
    result += vizChildEdge(uid, start_expr->getUid());
    result += start_expr->gen_viz_code();
    result += vizChildEdge(uid, end_expr->getUid());
    result += end_expr->gen_viz_code();
    result += vizChildEdge(uid, body_part->getUid());
    result += body_part->gen_viz_code();
    return result;
}

std::string WhileStmtNode::gen_viz_code() {
    std::string result = vizNode(uid, "WhileStmtNode");
    result += vizChildEdge(uid, condition->getUid());
    result += condition->gen_viz_code();
    result += vizChildEdge(uid, body_part->getUid());
    result += body_part->gen_viz_code();
    return result;
}

std::string RepeatStmtNode::gen_viz_code() {
    std::string result = vizNode(uid, "RepeatStmtNode");
    result += vizChildEdge(uid, body_part->getUid());
    result += body_part->gen_viz_code();
    result += vizChildEdge(uid, condition->getUid());
    result += condition->gen_viz_code();
    return result;
}

std::string SwitchStmtNode::gen_viz_code() {
    std::string result = vizNode(uid, "SwitchStmtNode");
    result += vizChildEdge(uid, condition->getUid());
    result += condition->gen_viz_code();
    result += vizChildEdge(uid, case_list->getUid());
    result += case_list->gen_viz_code();
    return result;
}

std::string CaseStmtNode::gen_viz_code() {
    std::string result = vizNode(uid, "CaseStmtNode");
    result += vizChildEdge(uid, const_list->getUid());
    result += const_list->gen_viz_code();
    result += vizChildEdge(uid, body_part->getUid());
    result += body_part->gen_viz_code();
    return result;
}

std::string FuncStmtNode::gen_viz_code() {
    std::string result = vizNode(uid, "FuncStmtNode");
    result += vizChildEdge(uid, func->getUid());
    result += func->gen_viz_code();
    return result;
}