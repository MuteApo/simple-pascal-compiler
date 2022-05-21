#include "include/node_func.hpp"

bool ParamDefNode::gen_sym_tab(int order) {
    return symbol_table.addSymbol(var_def->getName(), var_def, order);
}

bool FuncDefNode::hasDecl() {
    if (param_defs != nullptr) return true;
    if (block->hasDecl()) return true;
    return false;
}

bool FuncDefNode::gen_sym_tab() {
    symbol_table.addSymbol(name, this);
    if (hasDecl()) symbol_table.enterScope();
    if (param_defs != nullptr) param_defs->gen_sym_tab();
    block->visit();
    if (hasDecl()) symbol_table.leaveScope();
    return true;
}

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

bool FuncDefListNode::gen_sym_tab() {
    bool result = true;
    for (FuncDefNode *def : func_defs) result &= def->gen_sym_tab();
    return result;
}