#include "include/node_const.hpp"
#include "include/symbol_table.hpp"

ConstDefNode::ConstDefNode(std::string id, ExprNode *v)
        : uid(++global_uid), line_no(yylineno), name(id), expr(v) {}

int ConstDefNode::getUid() {
    return uid;
}

int ConstDefNode::getLineNumber() {
    return line_no;
}

ExprNode *ConstDefNode::getExpr() {
    return expr;
}

std::string ConstDefNode::genVizCode(int run) {
    std::string result = vizNode(uid, "ConstDefNode\n" + name);
    result += vizEdge(uid, expr->getUid(), "constdef", "Const Definition");
    result += expr->genVizCode(run);
    return result;
}

bool ConstDefNode::genSymbolTable() {
    if (symbol_table.existSymbol(name)) throw RedefineError(line_no, name);
    return symbol_table.addSymbol(name, this);
}

ConstDefListNode::ConstDefListNode() : uid(++global_uid), line_no(yylineno) {
    const_defs.clear();
}

int ConstDefListNode::getUid() {
    return uid;
}

void ConstDefListNode::addConstDef(ConstDefNode *const_def) {
    const_defs.push_back(const_def);
}

std::string ConstDefListNode::genVizCode(int run) {
    std::string result = vizNode(uid, "ConstDefListNode");
    for (int i = 0; i < const_defs.size(); i++) {
        result += vizEdge(uid,
                          const_defs.at(i)->getUid(),
                          "constdef" + to_string(i + 1),
                          "Const Definition " + to_string(i + 1));
        result += const_defs.at(i)->genVizCode(run);
    }
    return result;
}

bool ConstDefListNode::genSymbolTable() {
    for (ConstDefNode *def : const_defs) try {
            def->genSymbolTable();
        } catch (RedefineError &e) {
            error_handler.addMsg(e);
        }
    return true;
}

ConstListNode::ConstListNode() : uid(++global_uid), line_no(yylineno) {
    const_list.clear();
}

int ConstListNode::getUid() {
    return uid;
}

std::vector<ExprNode *> &ConstListNode::getConstList() {
    return const_list;
}

void ConstListNode::addConst(ExprNode *const_expr) {
    const_list.push_back(const_expr);
}

std::string ConstListNode::genVizCode(int run) {
    std::string result = vizNode(uid, "ConstDefListNode");
    for (int i = 0; i < const_list.size(); i++) {
        result += vizEdge(uid,
                          const_list.at(i)->getUid(),
                          "const" + to_string(i + 1),
                          "Const " + to_string(i + 1));
        result += const_list.at(i)->genVizCode(run);
    }
    return result;
}

void ConstListNode::testExprType(ExprNode *condition) {
    TypeAttrNode *type = condition->getResultType();
    for (ExprNode *expr : const_list)
        if (!type->isTypeEqual(expr->getResultType()))
            throw ExpressionTypeError(expr->getLineNumber(),
                                      type->getTypeString(),
                                      expr->getResultType()->getTypeString());
}