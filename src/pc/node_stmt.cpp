#include "include/node_stmt.hpp"

StmtNode::StmtNode(stmt_type       t,
                   StmtListNode   *c_s,
                   AssignStmtNode *a_s,
                   IfStmtNode     *i_s,
                   ForStmtNode    *f_s,
                   WhileStmtNode  *wh_s,
                   RepeatStmtNode *rep_s,
                   SwitchStmtNode *s_s,
                   FuncStmtNode   *p_s,
                   ReadStmtNode   *rea_s,
                   WriteStmtNode  *wr_s)
        : uid(++global_uid),
          type(t),
          compound_stmt(c_s),
          assign_stmt(a_s),
          if_stmt(i_s),
          for_stmt(f_s),
          while_stmt(wh_s),
          repeat_stmt(rep_s),
          switch_stmt(s_s),
          func_stmt(p_s),
          read_stmt(rea_s),
          write_stmt(wr_s) {}
StmtNode::StmtNode(StmtListNode *c_s)
        : StmtNode(SK_Compound,
                   c_s,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr) {}
StmtNode::StmtNode(AssignStmtNode *a_s)
        : StmtNode(SK_Assign,
                   nullptr,
                   a_s,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr) {}
StmtNode::StmtNode(IfStmtNode *i_s)
        : StmtNode(SK_If,
                   nullptr,
                   nullptr,
                   i_s,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr) {}
StmtNode::StmtNode(ForStmtNode *f_s)
        : StmtNode(SK_For,
                   nullptr,
                   nullptr,
                   nullptr,
                   f_s,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr) {}
StmtNode::StmtNode(WhileStmtNode *w_s)
        : StmtNode(SK_While,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   w_s,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr) {}
StmtNode::StmtNode(RepeatStmtNode *r_s)
        : StmtNode(SK_Repeat,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   r_s,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr) {}
StmtNode::StmtNode(SwitchStmtNode *s_s)
        : StmtNode(SK_Switch,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   s_s,
                   nullptr,
                   nullptr,
                   nullptr) {}
StmtNode::StmtNode(FuncStmtNode *p_s)
        : StmtNode(SK_Func,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   p_s,
                   nullptr,
                   nullptr) {}
StmtNode::StmtNode(ReadStmtNode *r_s)
        : StmtNode(SK_Read,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   r_s,
                   nullptr) {}
StmtNode::StmtNode(WriteStmtNode *w_s)
        : StmtNode(SK_Write,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   w_s) {}

std::string StmtNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "StmtNode");
    switch (type) {
        case SK_Compound:
            result += vizChildEdge(uid, compound_stmt->getUid(), "compound", "Compound Statement");
            result += compound_stmt->gen_viz_code(run);
            break;
        case SK_Assign:
            result += vizChildEdge(uid, assign_stmt->getUid(), "assign", "Assign Statement");
            result += assign_stmt->gen_viz_code(run);
            break;
        case SK_If:
            result += vizChildEdge(uid, if_stmt->getUid(), "if", "If Statement");
            result += if_stmt->gen_viz_code(run);
            break;
        case SK_For:
            result += vizChildEdge(uid, for_stmt->getUid(), "for", "For Statement");
            result += for_stmt->gen_viz_code(run);
            break;
        case SK_While:
            result += vizChildEdge(uid, while_stmt->getUid(), "while", "While Statement");
            result += while_stmt->gen_viz_code(run);
            break;
        case SK_Repeat:
            result += vizChildEdge(uid, repeat_stmt->getUid(), "repeat", "Repeat Statement");
            result += repeat_stmt->gen_viz_code(run);
            break;
        case SK_Switch:
            result += vizChildEdge(uid, switch_stmt->getUid(), "switch", "Switch Statement");
            result += switch_stmt->gen_viz_code(run);
            break;
        case SK_Func:
            result += vizChildEdge(uid, func_stmt->getUid(), "func", "Function Call Statement");
            result += func_stmt->gen_viz_code(run);
            break;
        case SK_Read:
            result += vizChildEdge(uid, read_stmt->getUid(), "read", "Read Statement");
            result += read_stmt->gen_viz_code(run);
            break;
        case SK_Write:
            result += vizChildEdge(uid, write_stmt->getUid(), "write", "Write Statement");
            result += write_stmt->gen_viz_code(run);
            break;
    }
    return result;
}

bool StmtNode::test_result_type() {
    switch (type) {
        case SK_Compound: return compound_stmt->test_result_type();
        case SK_Assign: return assign_stmt->test_result_type();
        case SK_If: return if_stmt->test_result_type();
        case SK_For: return for_stmt->test_result_type();
        case SK_While: return while_stmt->test_result_type();
        case SK_Repeat: return repeat_stmt->test_result_type();
        case SK_Switch: return switch_stmt->test_result_type();
        case SK_Func: return func_stmt->test_result_type();
        case SK_Read: return read_stmt->test_result_type();
        case SK_Write: return write_stmt->test_result_type();
    }
    return false;
}

std::string StmtListNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "StmtListNode");
    for (int i = 0; i < stmts.size(); i++) {
        result += vizChildEdge(
            uid, stmts.at(i)->getUid(), "stmt" + to_string(i + 1), "Statement " + to_string(i + 1));
        result += stmts.at(i)->gen_viz_code(run);
    }
    return result;
}

bool StmtListNode::test_result_type() {
    for (StmtNode *stmt : stmts)
        if (!stmt->test_result_type()) return false;
    return true;
}

std::string AssignStmtNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "AssignStmtNode");
    result += vizChildEdge(uid, dst->getUid(), "lval", "Left Value");
    result += dst->gen_viz_code(run);
    result += vizChildEdge(uid, src->getUid(), "rval", "Right Value");
    result += src->gen_viz_code(run);
    return result;
}

bool AssignStmtNode::test_result_type() {
    return dst->getResultType()->is_type_equ(src->getResultType());
}

std::string IfStmtNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "IfStmtNode");
    result += vizChildEdge(uid, condition->getUid(), "condition", "Condition");
    result += condition->gen_viz_code(run);
    result += vizChildEdge(uid, then_part->getUid(), "then", "Then Part");
    result += then_part->gen_viz_code(run);
    if (else_part != nullptr) {
        result += vizChildEdge(uid, else_part->getUid(), "else", "Else Part");
        result += else_part->gen_viz_code(run);
    }
    return result;
}

bool IfStmtNode::test_result_type() {
    TypeAttrNode *type = condition->getResultType();
    if (type->getType() != basic) return false;
    if (type->getBasicAttrNode()->getType() != boolean) return false;
    if (!then_part->test_result_type()) return false;
    if (else_part != nullptr && !else_part->test_result_type()) return false;
    return true;
}

std::string ForStmtNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    result += vizChildEdge(uid, start_expr->getUid(), "start", "Start Expression");
    result += start_expr->gen_viz_code(run);
    result += vizChildEdge(uid, end_expr->getUid(), "end", "End Expression");
    result += end_expr->gen_viz_code(run);
    result += vizChildEdge(uid, body_part->getUid(), "body", "Body Part");
    result += body_part->gen_viz_code(run);
    return result;
}

bool ForStmtNode::test_result_type() {
    if (!start_expr->getResultType()->is_type_equ(end_expr->getResultType())) return false;
    return body_part->test_result_type();
}

std::string WhileStmtNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "WhileStmtNode");
    result += vizChildEdge(uid, condition->getUid(), "condition", "Condition");
    result += condition->gen_viz_code(run);
    result += vizChildEdge(uid, body_part->getUid(), "body", "Body Part");
    result += body_part->gen_viz_code(run);
    return result;
}

bool WhileStmtNode::test_result_type() {
    TypeAttrNode *type = condition->getResultType();
    if (type->getType() != basic) return false;
    if (type->getBasicAttrNode()->getType() != boolean) return false;
    return body_part->test_result_type();
}

std::string RepeatStmtNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "RepeatStmtNode");
    result += vizChildEdge(uid, body_part->getUid(), "body", "Body Part");
    result += body_part->gen_viz_code(run);
    result += vizChildEdge(uid, condition->getUid(), "condition", "Condition");
    result += condition->gen_viz_code(run);
    return result;
}

bool RepeatStmtNode::test_result_type() {
    TypeAttrNode *type = condition->getResultType();
    if (type->getType() != basic) return false;
    if (type->getBasicAttrNode()->getType() != boolean) return false;
    return body_part->test_result_type();
}

std::string CaseStmtNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "CaseStmtNode");
    result += vizChildEdge(uid, const_list->getUid(), "candicate", "Candicate");
    result += const_list->gen_viz_code(run);
    result += vizChildEdge(uid, body_part->getUid(), "body", "Body Part");
    result += body_part->gen_viz_code(run);
    return result;
}

bool CaseStmtNode::test_result_type() {
    // TODO incomplete, what about const_list?
    return body_part->test_result_type();
}

std::string CaseListNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "CaseListNode");
    for (int i = 0; i < case_list.size(); i++) {
        result += vizChildEdge(
            uid, case_list.at(i)->getUid(), "case" + to_string(i + 1), "Case " + to_string(i + 1));
        result += case_list.at(i)->gen_viz_code(run);
    }
    return result;
}

bool CaseListNode::test_result_type() {
    for (CaseStmtNode *stmt : case_list)
        if (!stmt->test_result_type()) return false;
    return true;
}

std::string SwitchStmtNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "SwitchStmtNode");
    result += vizChildEdge(uid, condition->getUid(), "condition", "Condition");
    result += condition->gen_viz_code(run);
    result += vizChildEdge(uid, case_list->getUid(), "cases", "Case List");
    result += case_list->gen_viz_code(run);
    return result;
}

bool SwitchStmtNode::test_result_type() {
    return case_list->test_result_type();
}

std::string FuncStmtNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "FuncStmtNode");
    result += vizChildEdge(uid, func->getUid(), "fun call", "Function Call");
    result += func->gen_viz_code(run);
    return result;
}

bool FuncStmtNode::test_result_type() {
    return func->test_arg_type();
}

std::string ReadStmtNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "ReadStmtNode");
    result += vizChildEdge(uid, exprs->getUid(), "args", "Argument List");
    result += exprs->gen_viz_code(run);
    return result;
}

bool ReadStmtNode::test_result_type() {
    return true;
}

std::string WriteStmtNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, is_writeln ? "WritelnStmtNode" : "WriteStmtNode");
    result += vizChildEdge(uid, exprs->getUid(), "args", "Argument List");
    result += exprs->gen_viz_code(run);
    return result;
}

bool WriteStmtNode::test_result_type() {
    return true;
}