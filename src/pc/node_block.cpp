#include "include/node_block.hpp"

std::vector<int> ar_lvars_length;
std::vector<int> ar_args_length;

void BlockNode::gen_sym_tab() {
    if (const_defs != nullptr) const_defs->gen_sym_tab();
    if (type_defs != nullptr) type_defs->gen_sym_tab();
    if (var_defs != nullptr) var_defs->gen_sym_tab();
    if (func_defs != nullptr) func_defs->gen_sym_tab();
}

std::string BlockNode::gen_asm_code() {
    std::string asm_code = is_global ? var_defs->gen_asm_def() : "";
    // asm_code += func_defs->gen_asm_code();
    asm_code += stmts->gen_asm_code();
    return asm_code;
}

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

std::string BlockNode::visit() {
    std::string asm_code;
    symbol_table.enterScope();
    gen_sym_tab();
    // asm_code = gen_asm_code();
    symbol_table.leaveScope();
    return asm_code;
}