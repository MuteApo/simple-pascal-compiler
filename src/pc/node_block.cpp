#include "include/node_block.hpp"
#include "include/asmgen.hpp"

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

void BlockNode::genSymbolTable(int param_bias) {
    if (const_defs != nullptr) const_defs->genSymbolTable();
    if (type_defs != nullptr) type_defs->genSymbolTable();
    if (var_defs != nullptr) var_defs->genSymbolTable(param_bias);
    if (func_defs != nullptr) func_defs->genSymbolTable();
}

std::string BlockNode::genAsmDef() {
    if (is_global && var_defs != nullptr) {
        return var_defs->genAsmDef();
    } else
        return "";
}

std::string BlockNode::genAsmCode() {
    std::string asm_code = "";
    if (stmts != nullptr) asm_code += stmts->genAsmCode();
    if (func_defs != nullptr) asm_code += func_defs->genAsmCode();
    return asm_code;
}

void BlockNode::visit(int param_bias) {
    try {
        genSymbolTable(param_bias);
        if (is_global) {
            std::string text_seg = genAsmCode();
            text_seg += get_exit();
            write_segment(text_seg, false);
            std::string data_seg = genAsmDef();
            if (data_seg != "") write_segment(data_seg, true);
        }
        if (func_defs != nullptr) func_defs->visit();
        if (stmts != nullptr) stmts->testExprType();
    } catch (Exception &e) {
        error_handler.addMsg(e);
    }
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

void ProgramNode::visit() {
    if (block == nullptr) return;
    symbol_table.enterScope();
    block->visit(0);
    symbol_table.leaveScope();
}