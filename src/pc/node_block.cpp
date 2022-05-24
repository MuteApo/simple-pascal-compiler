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
          line_no(yylineno),
          is_global(is_g),
          const_defs(c_defs),
          type_defs(t_defs),
          var_defs(v_defs),
          func_defs(f_defs),
          stmts(s) {}

int BlockNode::getUid() {
    return uid;
}

void BlockNode::setGlobal() {
    is_global = true;
}

bool BlockNode::hasDecl() {
    if (const_defs != nullptr) return true;
    if (type_defs != nullptr) return true;
    if (var_defs != nullptr) return true;
    if (func_defs != nullptr) return true;
    return false;
}

std::string BlockNode::genVizCode(int run) {
    std::string result = vizNode(uid, "BlockNode");
    if (const_defs != nullptr) {
        result += vizEdge(uid, const_defs->getUid(), "const", "Const Part");
        result += const_defs->genVizCode(run);
    }
    if (type_defs != nullptr) {
        result += vizEdge(uid, type_defs->getUid(), "type", "Type Part");
        result += type_defs->genVizCode(run);
    }
    if (var_defs != nullptr) {
        result += vizEdge(uid, var_defs->getUid(), "var", "Var Part");
        result += var_defs->genVizCode(run);
    }
    if (func_defs != nullptr) {
        result += vizEdge(uid, func_defs->getUid(), "func", "Function/Procedure Part");
        result += func_defs->genVizCode(run);
    }
    if (stmts != nullptr) {
        result += vizEdge(uid, stmts->getUid(), "stmts", "Statement Part");
        result += stmts->genVizCode(run);
    }
    return result;
}

void BlockNode::genSymbolTable() {
    try {
        if (const_defs != nullptr) const_defs->genSymbolTable();
        if (type_defs != nullptr) type_defs->genSymbolTable();
        if (var_defs != nullptr) var_defs->genSymbolTable();
        if (func_defs != nullptr) func_defs->genSymbolTable();
    } catch (RedefineError &e) {
        error_handler.addMsg(e);
    }
    if (stmts != nullptr && !stmts->testExprType()) exit(-1);  // TODO
}

std::string BlockNode::genAsmCode() {
    std::string asm_code = is_global ? var_defs->genAsmDef() : "";
    // asm_code += func_defs->genAsmCode();
    asm_code += stmts->genAsmCode();
    return asm_code;
}

std::string BlockNode::visit() {
    std::string asm_code = "";
    symbol_table.enterScope();
    try {
        genSymbolTable();
    } catch (RedefineError &e) {
        // TODO what to do?
    }
    // asm_code = genAsmCode();
    symbol_table.leaveScope();
    return asm_code;
}

ProgramNode::ProgramNode(std::string id, BlockNode *b)
        : uid(++global_uid), line_no(yylineno), name(id), block(b) {}
ProgramNode::ProgramNode(std::string id, IdListNode *i_l, BlockNode *b)
        : uid(++global_uid), line_no(yylineno), name(id), id_list(i_l), block(b) {}

int ProgramNode::getUid() {
    return uid;
}

std::string ProgramNode::getNodeInfo() {
    std::string result = "ProgramNode\n" + name;
    if (id_list != nullptr) result += id_list->getIdString(",");
    return result;
}

std::string ProgramNode::genVizCode(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    if (block != nullptr) {
        result += vizEdge(uid, block->getUid(), "block", "Program Block");
        result += block->genVizCode(run);
    }
    return "digraph \"Parse Tree\" {\n" + result + "}";
}

std::string ProgramNode::visit() {
    return block != nullptr ? block->visit() : "";
}