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

    void gen_sym_tab();

    std::string gen_asm_code();

  public:
    BlockNode(bool              is_g,
              ConstDefListNode *c_defs = nullptr,
              TypeDefListNode  *t_defs = nullptr,
              VarDefListNode   *v_defs = nullptr,
              FuncDefListNode  *f_defs = nullptr,
              StmtListNode     *s      = nullptr)
            : uid(++global_uid),
              is_global(is_g),
              const_defs(c_defs),
              type_defs(t_defs),
              var_defs(v_defs),
              func_defs(f_defs),
              stmts(s) {}

    int getUid() {
        return uid;
    }

    void setGlobal() {
        is_global = true;
    }

    std::string gen_viz_code();

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

    std::string getNodeInfo() {
        std::string result = "ProgramNode\n" + name + "(";
        if (id_list.size()) result += id_list.at(0)->getName();
        for (int i = 1; i < id_list.size(); i++) result += "," + id_list.at(i)->getName();
        return result + ")";
    }

    std::string gen_viz_code() {
        std::string result = vizNode(uid, getNodeInfo());
        if (block != nullptr) {
            result += vizChildEdge(uid, block->getUid());
            result += block->gen_viz_code();
        }
        return "digraph G {\n" + result + "}";
    }

    std::string visit() {
        return block != nullptr ? block->visit() : "";
    }
};

#endif