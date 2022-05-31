#ifndef __NODE_BLOCK_H__
#define __NODE_BLOCK_H__

class BlockNode;
class ProgramNode;

#include "defs.hpp"
#include "node_expr.hpp"
#include "node_func.hpp"
#include "node_stmt.hpp"
#include "node_type.hpp"
#include "node_var.hpp"
#include "symbol_table.hpp"

extern std::vector<std::string> func_body;

extern int global_uid;
extern int yylineno;

class BlockNode {
  private:
    int               uid;
    int               line_no;
    ConstDefListNode *const_defs;
    TypeDefListNode  *type_defs;
    VarDefListNode   *var_defs;
    FuncDefListNode  *func_defs;
    StmtListNode     *stmts;

  public:
    BlockNode(ConstDefListNode *c_defs = nullptr,
              TypeDefListNode  *t_defs = nullptr,
              VarDefListNode   *v_defs = nullptr,
              FuncDefListNode  *f_defs = nullptr,
              StmtListNode     *s      = nullptr);

    int getUid();

    bool hasDecl();

    std::string genVizCode(int run);

    void genSymbolTable();

    void visit(std::string block_name);
};

class ProgramNode {
  private:
    int         uid;
    int         line_no;
    std::string name;
    IdListNode *id_list;
    BlockNode  *block;

  public:
    ProgramNode(std::string id, BlockNode *b);
    ProgramNode(std::string id, IdListNode *i_l, BlockNode *b);

    int getUid();

    std::string getNodeInfo();

    std::string genVizCode(int run);

    void visit();
};

#endif