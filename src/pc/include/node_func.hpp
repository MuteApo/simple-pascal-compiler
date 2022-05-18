#ifndef _NODE_FUNC_H_
#define _NODE_FUNC_H_

class FuncDefNode;
class FuncDefListNode;

#include "node_block.hpp"
#include "node_type.hpp"
#include <string>
#include <vector>
class FuncDefNode {
  private:
    bool                        is_func;
    std::string                 name;
    BasicAttrNode              *retval_type;
    std::vector<std::string>    param_name;
    std::vector<TypeAttrNode *> param_type;
    BlockNode                  *block;

  public:
    std::string gen_asm_code(void);
};

class FuncDefListNode {
  private:
    std::vector<FuncDefNode *> func_defs;

  public:
    void append_stmt(FuncDefNode *func_def) {
        func_defs.push_back(func_def);
    }

    std::string gen_asm_code();
};

#endif