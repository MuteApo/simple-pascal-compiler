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
#include "viz.hpp"

extern int global_uid;

class BlockNode {
  private:
    int               uid;
    bool              is_global;
    ConstDefListNode *const_defs;
    TypeDefListNode  *type_defs;
    VarDefListNode   *var_defs;
    FuncDefListNode  *func_defs;
    StmtListNode     *stmts;

  public:
    BlockNode(bool              is_g,
              ConstDefListNode *c_defs = nullptr,
              TypeDefListNode  *t_defs = nullptr,
              VarDefListNode   *v_defs = nullptr,
              FuncDefListNode  *f_defs = nullptr,
              StmtListNode     *s      = nullptr);

    int getUid() {
        return uid;
    }

    void setGlobal() {
        is_global = true;
    }

    bool hasDecl();

    std::string gen_viz_code(int run);

    void gen_sym_tab();

    std::string gen_asm_code();

    std::string visit();
};

class ProgramNode {
  private:
    int                   uid;
    std::string           name;
    std::vector<IdNode *> id_list;
    BlockNode            *block;

  public:
    ProgramNode(std::string id, BlockNode *b) : uid(++global_uid), name(id), block(b) {}
    ProgramNode(std::string id, std::vector<IdNode *> i_l, BlockNode *b)
            : uid(++global_uid), name(id), id_list(i_l), block(b) {}

    int getUid() {
        return uid;
    }

    std::string getNodeInfo();

    std::string gen_viz_code(int run);

    std::string visit();
};

#endif