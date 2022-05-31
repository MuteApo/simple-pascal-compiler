#include "include/node_block.hpp"
#include "include/asmgen.hpp"

std::vector<std::string> func_body;

BlockNode::BlockNode(ConstDefListNode *c_defs,
                     TypeDefListNode  *t_defs,
                     VarDefListNode   *v_defs,
                     FuncDefListNode  *f_defs,
                     StmtListNode     *s)
        : uid(++global_uid),
          line_no(yylineno),
          const_defs(c_defs),
          type_defs(t_defs),
          var_defs(v_defs),
          func_defs(f_defs),
          stmts(s) {}

int BlockNode::getUid() {
    return uid;
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
    if (const_defs != nullptr) const_defs->genSymbolTable();
    if (type_defs != nullptr) type_defs->genSymbolTable();
    if (var_defs != nullptr) var_defs->genSymbolTable();
    if (func_defs != nullptr) func_defs->genSymbolTable();
}

void BlockNode::visit(std::string block_name) {
    std::string asm_code  = "";
    bool        is_global = symbol_table.getLevel() == 0;
    try {
        genSymbolTable();
        if (is_global) {
            asm_code += block_name + ":\n";
        } else {
            int                              level      = symbol_table.getLevel();
            std::set<VarTableItem>           local_vars = symbol_table.getVarScope(level);
            std::set<VarTableItem>::iterator iter       = local_vars.begin();
            uint32_t                         param_len  = 0;
            while (iter != local_vars.end()) {
                param_len += iter->var_def->getType()->getLength();
                iter++;
            }
            asm_code += get_func_framework(true, block_name, param_len);
        }
        if (stmts != nullptr) {
            stmts->testExprType();
            asm_code += stmts->genAsmCode();
        }
        if (is_global) {
            asm_code += get_exit();
            write_segment(asm_code, false);
        } else {
            asm_code += get_func_framework(false);
        }
        if (func_defs != nullptr) func_defs->visit();
        if (is_global) {
            for (int i = 0; i < func_body.size(); i++) {
                write_segment(func_body[i], false);
            }
            if (var_defs != nullptr) {
                write_segment(var_defs->genAsmDef(), true);
            }
        } else {
            func_body.push_back(asm_code);
        }
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
    block->visit("main");
    symbol_table.leaveScope();
}