#include "include/node_stmt.hpp"
#include "include/asmgen.hpp"

StmtNode::StmtNode(StmtType        t,
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
          line_no(yylineno),
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

int StmtNode::getUid() {
    return uid;
}

std::string StmtNode::genVizCode(int run) {
    std::string result = vizNode(uid, "StmtNode");
    switch (type) {
        case SK_Compound:
            result += vizEdge(uid, compound_stmt->getUid(), "compound", "Compound Statement");
            result += compound_stmt->genVizCode(run);
            break;
        case SK_Assign:
            result += vizEdge(uid, assign_stmt->getUid(), "assign", "Assign Statement");
            result += assign_stmt->genVizCode(run);
            break;
        case SK_If:
            result += vizEdge(uid, if_stmt->getUid(), "if", "If Statement");
            result += if_stmt->genVizCode(run);
            break;
        case SK_For:
            result += vizEdge(uid, for_stmt->getUid(), "for", "For Statement");
            result += for_stmt->genVizCode(run);
            break;
        case SK_While:
            result += vizEdge(uid, while_stmt->getUid(), "while", "While Statement");
            result += while_stmt->genVizCode(run);
            break;
        case SK_Repeat:
            result += vizEdge(uid, repeat_stmt->getUid(), "repeat", "Repeat Statement");
            result += repeat_stmt->genVizCode(run);
            break;
        case SK_Switch:
            result += vizEdge(uid, switch_stmt->getUid(), "switch", "Switch Statement");
            result += switch_stmt->genVizCode(run);
            break;
        case SK_Func:
            result += vizEdge(uid, func_stmt->getUid(), "func", "Function Call Statement");
            result += func_stmt->genVizCode(run);
            break;
        case SK_Read:
            result += vizEdge(uid, read_stmt->getUid(), "read", "Read Statement");
            result += read_stmt->genVizCode(run);
            break;
        case SK_Write:
            result += vizEdge(uid, write_stmt->getUid(), "write", "Write Statement");
            result += write_stmt->genVizCode(run);
            break;
    }
    return result;
}

bool StmtNode::testExprType() {
    switch (type) {
        case SK_Compound: return compound_stmt->testExprType();
        case SK_Assign: return assign_stmt->testExprType();
        case SK_If: return if_stmt->testExprType();
        case SK_For: return for_stmt->testExprType();
        case SK_While: return while_stmt->testExprType();
        case SK_Repeat: return repeat_stmt->testExprType();
        case SK_Switch: return switch_stmt->testExprType();
        case SK_Func: return func_stmt->testExprType();
        case SK_Read: return read_stmt->testExprType();
        case SK_Write: return write_stmt->testExprType();
    }
    return true;
}

std::string StmtNode::genAsmCode() {
    switch (type) {
        case SK_Compound: return compound_stmt->genAsmCode();
        case SK_Assign: return assign_stmt->genAsmCode();
        case SK_If: return if_stmt->genAsmCode();
        case SK_For: return for_stmt->genAsmCode();
        case SK_While: return while_stmt->genAsmCode();
        case SK_Repeat: return repeat_stmt->genAsmCode();
        case SK_Switch: return switch_stmt->genAsmCode();
        case SK_Func: return func_stmt->genAsmCode();
        case SK_Read: return read_stmt->genAsmCode();
        case SK_Write: return write_stmt->genAsmCode();
    }
}

StmtListNode::StmtListNode() : uid(++global_uid), line_no(yylineno) {
    stmts.clear();
}

int StmtListNode::getUid() {
    return uid;
}

void StmtListNode::addStmt(StmtNode *stmt) {
    stmts.push_back(stmt);
}

std::string StmtListNode::genVizCode(int run) {
    std::string result = vizNode(uid, "StmtListNode");
    for (int i = 0; i < stmts.size(); i++) {
        result += vizEdge(
            uid, stmts.at(i)->getUid(), "stmt" + to_string(i + 1), "Statement " + to_string(i + 1));
        result += stmts.at(i)->genVizCode(run);
    }
    return result;
}

bool StmtListNode::testExprType() {
    for (StmtNode *stmt : stmts) try {
            stmt->testExprType();
        } catch (Exception &e) {
            error_handler.addMsg(e);
        }
    return true;
}

std::string StmtListNode::genAsmCode() {
    std::string asm_code = "";
    for (StmtNode *s : stmts) asm_code += s->genAsmCode();
    return asm_code;
}

AssignStmtNode::AssignStmtNode(ExprNode *d, ExprNode *s)
        : uid(++global_uid), line_no(yylineno), dst(d), src(s) {}

int AssignStmtNode::getUid() {
    return uid;
}

std::string AssignStmtNode::genVizCode(int run) {
    std::string result = vizNode(uid, "AssignStmtNode");
    result += vizEdge(uid, dst->getUid(), "lval", "Left Value");
    result += dst->genVizCode(run);
    result += vizEdge(uid, src->getUid(), "rval", "Right Value");
    result += src->genVizCode(run);
    return result;
}

std::string AssignStmtNode::genAsmCode() {
    std::string asm_code = "";
    asm_code += src->genAsmCodeRHS();
    asm_code += get_reg_xchg(t_table[2], t_table[0]);
    asm_code += dst->genAsmCodeLHS();
    return asm_code;
}

bool AssignStmtNode::testExprType() {
    TypeAttrNode *dst_type = nullptr, *src_type = nullptr;
    try {
        dst_type = dst->getResultType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    try {
        src_type = src->getResultType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    if (dst_type != nullptr && src_type != nullptr && !dst_type->isTypeEqual(src_type))
        throw ExpressionTypeError(dst->getLineNumber(),
                                  to_string(dst_type->getTypeString()),
                                  to_string(src_type->getTypeString()));
    return true;
}

IfStmtNode::IfStmtNode(ExprNode *e, StmtNode *t_p, StmtNode *e_p)
        : uid(++global_uid), line_no(yylineno), condition(e), then_part(t_p), else_part(e_p) {}

int IfStmtNode::getUid() {
    return uid;
}

std::string IfStmtNode::genVizCode(int run) {
    std::string result = vizNode(uid, "IfStmtNode");
    result += vizEdge(uid, condition->getUid(), "condition", "Condition");
    result += condition->genVizCode(run);
    result += vizEdge(uid, then_part->getUid(), "then", "Then Part");
    result += then_part->genVizCode(run);
    if (else_part != nullptr) {
        result += vizEdge(uid, else_part->getUid(), "else", "Else Part");
        result += else_part->genVizCode(run);
    }
    return result;
}

std::string IfStmtNode::genAsmCode() {
    return "";  // TODO
}

bool IfStmtNode::testExprType() {
    try {
        then_part->testExprType();
    } catch (ExpressionTypeError &e) {
        error_handler.addMsg(e);
    }
    try {
        if (else_part != nullptr) else_part->testExprType();
    } catch (ExpressionTypeError &e) {
        error_handler.addMsg(e);
    }
    TypeAttrNode *type = condition->getResultType();
    if (type->getType() != basic || type->getBasicAttrNode()->getType() != boolean)
        throw ExpressionTypeError(line_no, "boolean", type->getName());
    return true;
}

ForStmtNode::ForStmtNode(std::string id, bool is_t, ExprNode *s_e, ExprNode *e_e, StmtNode *b_p)
        : uid(++global_uid),
          line_no(yylineno),
          name(id),
          is_to(is_t),
          start_expr(s_e),
          end_expr(e_e),
          body_part(b_p) {}

int ForStmtNode::getUid() {
    return uid;
}

std::string ForStmtNode::getNodeInfo() {
    std::string result = "ForStmtNode\n" + name;
    return result + (is_to ? "\n→" : "\n←");
}

std::string ForStmtNode::genVizCode(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    result += vizEdge(uid, start_expr->getUid(), "start", "Start Expression");
    result += start_expr->genVizCode(run);
    result += vizEdge(uid, end_expr->getUid(), "end", "End Expression");
    result += end_expr->genVizCode(run);
    result += vizEdge(uid, body_part->getUid(), "body", "Body Part");
    result += body_part->genVizCode(run);
    return result;
}

std::string ForStmtNode::genAsmCode() {
    return "";  // TODO
}

bool ForStmtNode::testExprType() {
    try {
        body_part->testExprType();
    } catch (ExpressionTypeError &e) {
        error_handler.addMsg(e);
    }
    TypeAttrNode *start_type = start_expr->getResultType();
    TypeAttrNode *end_type   = end_expr->getResultType();
    if (!start_type->isTypeEqual(end_type))
        throw ExpressionTypeError(line_no, start_type->getName(), end_type->getName());
    VarDefNode *var_def = symbol_table.findVarSymbol(name);
    if (var_def == nullptr) throw UndefineError(start_expr->getLineNumber(), name);
    return true;
}

WhileStmtNode::WhileStmtNode(ExprNode *c, StmtNode *b_p)
        : uid(++global_uid), line_no(yylineno), condition(c), body_part(b_p) {}

int WhileStmtNode::getUid() {
    return uid;
}

std::string WhileStmtNode::genVizCode(int run) {
    std::string result = vizNode(uid, "WhileStmtNode");
    result += vizEdge(uid, condition->getUid(), "condition", "Condition");
    result += condition->genVizCode(run);
    result += vizEdge(uid, body_part->getUid(), "body", "Body Part");
    result += body_part->genVizCode(run);
    return result;
}

std::string WhileStmtNode::genAsmCode() {
    return "";  // TODO
}

bool WhileStmtNode::testExprType() {
    try {
        body_part->testExprType();
    } catch (ExpressionTypeError &e) {
        error_handler.addMsg(e);
    }
    TypeAttrNode *type = condition->getResultType();
    if (type != nullptr &&
        (type->getType() != basic || type->getBasicAttrNode()->getType() != boolean))
        throw ExpressionTypeError(line_no, "boolean", type->getName());
    return true;
}

RepeatStmtNode::RepeatStmtNode(StmtListNode *b_p, ExprNode *c)
        : uid(++global_uid), line_no(yylineno), body_part(b_p), condition(c) {}

int RepeatStmtNode::getUid() {
    return uid;
}

std::string RepeatStmtNode::genVizCode(int run) {
    std::string result = vizNode(uid, "RepeatStmtNode");
    result += vizEdge(uid, body_part->getUid(), "body", "Body Part");
    result += body_part->genVizCode(run);
    result += vizEdge(uid, condition->getUid(), "condition", "Condition");
    result += condition->genVizCode(run);
    return result;
}

std::string RepeatStmtNode::genAsmCode() {
    return "";  // TODO
}

bool RepeatStmtNode::testExprType() {
    try {
        body_part->testExprType();
    } catch (ExpressionTypeError &e) {
        error_handler.addMsg(e);
    }
    TypeAttrNode *type = condition->getResultType();
    if (type != nullptr &&
        (type->getType() != basic || type->getBasicAttrNode()->getType() != boolean))
        throw ExpressionTypeError(line_no, "boolean", type->getName());
    return true;
}

CaseStmtNode::CaseStmtNode(ConstListNode *c_l, StmtNode *b_p)
        : uid(++global_uid), line_no(yylineno), const_list(c_l), body_part(b_p) {}

int CaseStmtNode::getUid() {
    return uid;
}

std::string CaseStmtNode::genVizCode(int run) {
    std::string result = vizNode(uid, "CaseStmtNode");
    result += vizEdge(uid, const_list->getUid(), "candicate", "Candicate");
    result += const_list->genVizCode(run);
    result += vizEdge(uid, body_part->getUid(), "body", "Body Part");
    result += body_part->genVizCode(run);
    return result;
}

bool CaseStmtNode::testExprType() {
    // TODO incomplete, what about const_list?
    try {
        body_part->testExprType();
    } catch (ExpressionTypeError &e) {
        error_handler.addMsg(e);
    }
    return true;
}

CaseListNode::CaseListNode() : uid(++global_uid), line_no(yylineno) {
    case_list.clear();
}

int CaseListNode::getUid() {
    return uid;
}

std::vector<CaseStmtNode *> &CaseListNode::getCaseList() {
    return case_list;
}

void CaseListNode::addCase(CaseStmtNode *c) {
    case_list.push_back(c);
}

std::string CaseListNode::genVizCode(int run) {
    std::string result = vizNode(uid, "CaseListNode");
    for (int i = 0; i < case_list.size(); i++) {
        result += vizEdge(
            uid, case_list.at(i)->getUid(), "case" + to_string(i + 1), "Case " + to_string(i + 1));
        result += case_list.at(i)->genVizCode(run);
    }
    return result;
}

bool CaseListNode::testExprType() {
    for (CaseStmtNode *stmt : case_list) try {
            stmt->testExprType();
        } catch (ExpressionTypeError &e) {
            error_handler.addMsg(e);
        }
    return true;
}

SwitchStmtNode::SwitchStmtNode(ExprNode *c, CaseListNode *c_l)
        : uid(++global_uid), line_no(yylineno), condition(c), case_list(c_l) {}

int SwitchStmtNode::getUid() {
    return uid;
}

std::string SwitchStmtNode::genVizCode(int run) {
    std::string result = vizNode(uid, "SwitchStmtNode");
    result += vizEdge(uid, condition->getUid(), "condition", "Condition");
    result += condition->genVizCode(run);
    result += vizEdge(uid, case_list->getUid(), "cases", "Case List");
    result += case_list->genVizCode(run);
    return result;
}

std::string SwitchStmtNode::genAsmCode() {
    return "";  // TODO
}

bool SwitchStmtNode::testExprType() {
    try {
        case_list->testExprType();
    } catch (ExpressionTypeError &e) {
        error_handler.addMsg(e);
    }
    return true;
}

FuncStmtNode::FuncStmtNode(FuncNode *f) : uid(++global_uid), line_no(yylineno), func(f) {}

int FuncStmtNode::getUid() {
    return uid;
}

std::string FuncStmtNode::genVizCode(int run) {
    std::string result = vizNode(uid, "FuncStmtNode");
    result += vizEdge(uid, func->getUid(), "fun call", "Function Call");
    result += func->genVizCode(run);
    return result;
}

std::string FuncStmtNode::genAsmCode() {
    return "";  // TODO
}

bool FuncStmtNode::testExprType() {
    try {
        func->testArgType();
    } catch (ArgumentNumberError &e) {
        error_handler.addMsg(e);
    } catch (ArgumentTypeError &e) {
        error_handler.addMsg(e);
    }
    return true;
}

ReadStmtNode::ReadStmtNode(ExprListNode *e) : uid(++global_uid), line_no(yylineno), exprs(e) {}

int ReadStmtNode::getUid() {
    return uid;
}

std::string ReadStmtNode::genVizCode(int run) {
    std::string result = vizNode(uid, "ReadStmtNode");
    result += vizEdge(uid, exprs->getUid(), "args", "Argument List");
    result += exprs->genVizCode(run);
    return result;
}

std::string ReadStmtNode::genAsmCode() {
    return "";  // TODO
}

bool ReadStmtNode::testExprType() {
    // TODO
    return true;
}

WriteStmtNode::WriteStmtNode(bool is_ln, ExprListNode *e)
        : uid(++global_uid), line_no(yylineno), is_writeln(is_ln), exprs(e) {}

int WriteStmtNode::getUid() {
    return uid;
}

std::string WriteStmtNode::genVizCode(int run) {
    std::string result = vizNode(uid, is_writeln ? "WritelnStmtNode" : "WriteStmtNode");
    result += vizEdge(uid, exprs->getUid(), "args", "Argument List");
    result += exprs->genVizCode(run);
    return result;
}

std::string WriteStmtNode::genAsmCode() {
    return "";  // TODO
}

bool WriteStmtNode::testExprType() {
    // TODO
    return true;
}