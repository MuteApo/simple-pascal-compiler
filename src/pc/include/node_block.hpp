#ifndef _NODE_BLOCK_H_
#define _NODE_BLOCK_H_

class BlockNode;
class ProgramNode;

#include "node_expr.hpp"
#include "node_func.hpp"
#include "node_stmt.hpp"
#include "node_type.hpp"
#include "node_var.hpp"
#include "symbol_table.hpp"

class BlockNode {
  private:
    bool              is_global;
    ConstDefListNode *const_defs;
    TypeDefListNode  *type_defs;
    VarDefListNode   *var_defs;
    FuncDefListNode  *func_defs;
    StmtListNode     *stmts;

    void gen_sym_tab(void) {
        if (const_defs != nullptr) const_defs->gen_sym_tab();
        if (type_defs != nullptr) type_defs->gen_sym_tab();
        if (var_defs != nullptr) var_defs->gen_sym_tab();
        // if (func_defs != nullptr) func_defs->gen_sym_tab();
    }

    std::string gen_asm_code(void) {
        std::string asm_code = is_global ? var_defs->gen_asm_def() : "";
        // asm_code += func_defs->gen_asm_code();
        asm_code += stmts->gen_asm_code();
        return asm_code;
    }

  public:
    BlockNode(bool              is_g,
              ConstDefListNode *c_defs = nullptr,
              TypeDefListNode  *t_defs = nullptr,
              VarDefListNode   *v_defs = nullptr,
              FuncDefListNode  *f_defs = nullptr,
              StmtListNode     *s      = nullptr)
            : is_global(is_g),
              const_defs(c_defs),
              type_defs(t_defs),
              var_defs(v_defs),
              func_defs(f_defs),
              stmts(s) {}

    void setGlobal() {
        is_global = true;
    }

    std::string visit(void) {
        std::string asm_code;
        symbol_table.enterScope();
        gen_sym_tab();
        asm_code = gen_asm_code();
        symbol_table.leaveScope();
        return asm_code;
    }
};

class ProgramNode {
  private:
    std::string           name;
    std::vector<IdNode *> id_list;
    BlockNode            *block;

  public:
    ProgramNode(std::string id, BlockNode *b) : name(id), block(b) {}
    ProgramNode(std::string id, std::vector<IdNode *> i_l, BlockNode *b)
            : name(id), id_list(i_l), block(b) {}
};

#endif