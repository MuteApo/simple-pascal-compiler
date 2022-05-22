#include "include/node_block.hpp"

std::vector<int> ar_lvars_length;
std::vector<int> ar_args_length;

BlockNode::BlockNode(bool              is_g,
                     ConstDefListNode *c_defs,
                     TypeDefListNode  *t_defs,
                     VarDefListNode   *v_defs,
                     FuncDefListNode  *f_defs,
                     StmtListNode     *s)
        : uid(++global_uid),
          is_global(is_g),
          const_defs(c_defs),
          type_defs(t_defs),
          var_defs(v_defs),
          func_defs(f_defs),
          stmts(s) {}

bool BlockNode::hasDecl() {
    if (const_defs != nullptr) return true;
    if (type_defs != nullptr) return true;
    if (var_defs != nullptr) return true;
    if (func_defs != nullptr) return true;
    return false;
}

std::string BlockNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "BlockNode");
    if (const_defs != nullptr) {
        result += vizChildEdge(uid, const_defs->getUid(), "const", "Const Part");
        result += const_defs->gen_viz_code(run);
    }
    if (type_defs != nullptr) {
        result += vizChildEdge(uid, type_defs->getUid(), "type", "Type Part");
        result += type_defs->gen_viz_code(run);
    }
    if (var_defs != nullptr) {
        result += vizChildEdge(uid, var_defs->getUid(), "var", "Var Part");
        result += var_defs->gen_viz_code(run);
    }
    if (func_defs != nullptr) {
        result += vizChildEdge(uid, func_defs->getUid(), "func", "Function/Procedure Part");
        result += func_defs->gen_viz_code(run);
    }
    if (stmts != nullptr) {
        result += vizChildEdge(uid, stmts->getUid(), "stmts", "Statement Part");
        result += stmts->gen_viz_code(run);
    }
    return result;
}

void BlockNode::gen_sym_tab() {
    if (const_defs != nullptr) const_defs->gen_sym_tab();
    if (type_defs != nullptr) type_defs->gen_sym_tab();
    if (var_defs != nullptr) var_defs->gen_sym_tab();
    if (func_defs != nullptr) func_defs->gen_sym_tab();
    if (stmts != nullptr && !stmts->test_result_type()) exit(-1);
}

std::string BlockNode::gen_asm_code() {
    std::string asm_code = is_global ? var_defs->gen_asm_def() : "";
    // asm_code += func_defs->gen_asm_code();
    asm_code += stmts->gen_asm_code();
    return asm_code;
}

std::string BlockNode::visit() {
    std::string asm_code;
    symbol_table.enterScope();
    gen_sym_tab();
    // asm_code = gen_asm_code();
    symbol_table.leaveScope();
    return asm_code;
}

std::string ProgramNode::getNodeInfo() {
    std::string result = "ProgramNode\n" + name;
    if (id_list.size()) {
        result += "(" + id_list.at(0)->getName();
        for (int i = 1; i < id_list.size(); i++) result += "," + id_list.at(i)->getName();
        return result + ")";
    }
    return result;
}

std::string ProgramNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    if (block != nullptr) {
        result += vizChildEdge(uid, block->getUid(), "block", "Program Block");
        result += block->gen_viz_code(run);
    }
    return "digraph \"Parse Tree\" {\n" + result + "}";
}

std::string ProgramNode::visit() {
    return block != nullptr ? block->visit() : "";
}