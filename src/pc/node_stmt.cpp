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

void StmtNode::testExprType() {
    switch (type) {
        case SK_Compound: compound_stmt->testExprType(); break;
        case SK_Assign: assign_stmt->testExprType(); break;
        case SK_If: if_stmt->testExprType(); break;
        case SK_For: for_stmt->testExprType(); break;
        case SK_While: while_stmt->testExprType(); break;
        case SK_Repeat: repeat_stmt->testExprType(); break;
        case SK_Switch: switch_stmt->testExprType(); break;
        case SK_Func: func_stmt->testExprType(); break;
        case SK_Read: read_stmt->testExprType(); break;
        case SK_Write: write_stmt->testExprType(); break;
    }
}

std::string StmtNode::genAsmCode() {
    if (error_handler.getErrorCount() != 0) return "";
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

void StmtListNode::testExprType() {
    for (StmtNode *stmt : stmts) try {
            stmt->testExprType();
        } catch (Exception &e) {
            error_handler.addMsg(e);
        }
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

void AssignStmtNode::testExprType() {
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
    std::string cond_snippet, then_snippet, else_snippet, asm_code = "";
    cond_snippet = condition->genAsmCodeRHS();
    cond_snippet += get_reg_xchg(t_table[1], t_table[0]);
    then_snippet = then_part->genAsmCode();
    if (else_part != nullptr) else_snippet = else_part->genAsmCode();
    return get_stmt_cond(cond_snippet, then_snippet, else_snippet);
}

void IfStmtNode::testExprType() {
    TypeAttrNode *type = nullptr;
    try {
        type = condition->getResultType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    try {
        then_part->testExprType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    try {
        if (else_part != nullptr) else_part->testExprType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    if (type != nullptr)
        if (type->getType() != basic || type->getBasicAttrNode()->getType() != boolean)
            throw ExpressionTypeError(line_no, "boolean", type->getName());
}

ForStmtNode::ForStmtNode(bool is_t, ExprNode *i, ExprNode *s_e, ExprNode *e_e, StmtNode *b_p)
        : uid(++global_uid),
          line_no(yylineno),
          is_to(is_t),
          iter(i),
          start_expr(s_e),
          end_expr(e_e),
          body_part(b_p) {}

int ForStmtNode::getUid() {
    return uid;
}

std::string ForStmtNode::getNodeInfo() {
    std::string result = "ForStmtNode\n" + iter->getIdNode()->getName();
    return result + (is_to ? "\n→" : "\n←");
}

std::string ForStmtNode::genVizCode(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    result += vizEdge(uid, start_expr->getUid(), "start", "Start Expression");
    result += start_expr->genVizCode(run);
    result += vizEdge(uid, end_expr->getUid(), "end", "End Expression");
    result += end_expr->genVizCode(run);
    if (body_part != nullptr) {
        result += vizEdge(uid, body_part->getUid(), "body", "Body Part");
        result += body_part->genVizCode(run);
    }
    return result;
}

std::string ForStmtNode::genAsmCode() {
    std::string asm_code = "";
    std::string init_snippet, inc_dec_snippet, cmp_snippet, body_snippet = "";
    init_snippet = start_expr->genAsmCodeRHS();
    init_snippet += get_reg_xchg(t_table[2], t_table[0]);
    init_snippet += iter->genAsmCodeLHS();
    inc_dec_snippet = iter->genAsmCodeRHS();
    inc_dec_snippet += get_reg_xchg(t_table[1], t_table[0]);
    if (is_to) {
        inc_dec_snippet += get_load_imm(1);
    } else {
        inc_dec_snippet += get_load_imm(-1);
    }
    inc_dec_snippet += get_reg_xchg(t_table[2], t_table[0]);
    inc_dec_snippet += get_integer_calc("add", false);
    inc_dec_snippet += get_reg_xchg(t_table[2], t_table[0]);
    inc_dec_snippet += iter->genAsmCodeLHS();
    cmp_snippet = end_expr->genAsmCodeRHS();
    cmp_snippet += get_reg_save(t_table[0]);
    cmp_snippet += iter->genAsmCodeRHS();
    cmp_snippet += get_reg_xchg(t_table[1], t_table[0]);
    cmp_snippet += get_reg_restore(t_table[2]);
    if (is_to) {
        cmp_snippet += get_integer_calc("cmp_le", false);
    } else {
        cmp_snippet += get_integer_calc("cmp_ge", false);
    }
    cmp_snippet += get_reg_xchg(t_table[1], t_table[0]);
    if (body_part != nullptr) body_snippet = body_part->genAsmCode();
    asm_code = init_snippet + get_stmt_while(cmp_snippet, body_snippet + inc_dec_snippet);
    return asm_code;
}

void ForStmtNode::testExprType() {
    TypeAttrNode *iter_type = nullptr;
    try {
        iter_type = iter->getResultType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    TypeAttrNode *start_type = nullptr;
    try {
        start_type = start_expr->getResultType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    TypeAttrNode *end_type = nullptr;
    try {
        end_type = end_expr->getResultType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    try {
        body_part->testExprType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    if (symbol_table.findVarSymbol(iter->getIdNode()->getName()) == nullptr)
        throw UndefineError(start_expr->getLineNumber(), iter->getIdNode()->getName());
    if (iter_type != nullptr && start_type != nullptr && !iter_type->isTypeEqual(start_type))
        throw ExpressionTypeError(line_no, iter_type->getTypeString(), start_type->getTypeString());
    if (iter_type != nullptr && end_type != nullptr && !iter_type->isTypeEqual(end_type))
        throw ExpressionTypeError(line_no, iter_type->getTypeString(), end_type->getTypeString());
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
    std::string cond_snippet, body_snippet;
    cond_snippet = condition->genAsmCodeRHS();
    cond_snippet += get_reg_xchg(t_table[1], t_table[0]);
    body_snippet = body_part->genAsmCode();
    return get_stmt_while(cond_snippet, body_snippet);
}

void WhileStmtNode::testExprType() {
    TypeAttrNode *type = nullptr;
    try {
        type = condition->getResultType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    try {
        body_part->testExprType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    if (type != nullptr)
        if (type->getType() != basic || type->getBasicAttrNode()->getType() != boolean)
            throw ExpressionTypeError(line_no, "boolean", type->getTypeString());
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
    std::string cond_snippet, body_snippet;
    cond_snippet = condition->genAsmCodeRHS();
    cond_snippet += get_reg_xchg(t_table[1], t_table[0]);
    body_snippet = body_part->genAsmCode();
    return get_stmt_until(cond_snippet, body_snippet);
}

void RepeatStmtNode::testExprType() {
    try {
        body_part->testExprType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    TypeAttrNode *type = nullptr;
    try {
        type = condition->getResultType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    if (type != nullptr)
        if (type->getType() != basic || type->getBasicAttrNode()->getType() != boolean)
            throw ExpressionTypeError(line_no, "boolean", type->getTypeString());
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

void CaseStmtNode::testExprType(ExprNode *condition) {
    try {
        const_list->testExprType(condition);
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
    try {
        body_part->testExprType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
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

void CaseListNode::testExprType(ExprNode *condition) {
    for (CaseStmtNode *stmt : case_list) try {
            stmt->testExprType(condition);
        } catch (Exception &e) {
            error_handler.addMsg(e);
        }
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

void SwitchStmtNode::testExprType() {
    try {
        case_list->testExprType(condition);
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
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

void FuncStmtNode::testExprType() {
    try {
        func->testArgType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
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

void ReadStmtNode::testExprType() {
    // TODO
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
    std::string             asm_code  = "";
    std::vector<ExprNode *> expr_list = exprs->getExprList();
    if (is_writeln) expr_list.push_back(new ExprNode(new LiteralNode('\n')));
    for (int i = 0; i < expr_list.size(); i++) {
        asm_code += expr_list.at(i)->genAsmCodeRHS();
        TypeKind tk = expr_list.at(i)->getResultType()->getType();
        if (tk == basic) {
            BasicTypeKind type = expr_list.at(i)->getResultType()->getBasicAttrNode()->getType();
            if (type == integer) {
                asm_code += get_reg_xchg(t_table[1], t_table[0]);
                asm_code += get_write("int");
            } else if (type == boolean) {
                asm_code += get_reg_xchg(t_table[1], t_table[0]);
                asm_code += get_write("bool");
            } else if (type == character) {
                asm_code += get_reg_xchg(t_table[1], t_table[0]);
                asm_code += get_write("char");
            } else if (type == real) {
                // TODO
            }
        } else if (tk == ordinal) {
            // TODO
        }
    }
    return asm_code;
}

void WriteStmtNode::testExprType() {
    // TODO
}