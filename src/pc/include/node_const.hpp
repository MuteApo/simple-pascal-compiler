#ifndef _NODE_CONST_H_
#define _NODE_CONST_H_

class ConstDefNode;
class ConstDefListNode;

#include "node_expr.hpp"
#include "symbol_table.hpp"
#include <string>

class ConstDefNode {
  private:
    std::string  name;
    LiteralNode *value;

  public:
    ConstDefNode(std::string id) : name(id) {}

    template <class T> T get_value() {
        return value->get_value<T>();
    }

    void add_to_symtbl();

    std::string toString() {
        return "";
    }
};

class ConstDefListNode {
  private:
    std::vector<ConstDefNode *> const_defs;

  public:
    void append_const_def(ConstDefNode *const_def) {
        const_defs.push_back(const_def);
    }

    bool gen_sym_tab(void);

    std::string gen_asm_def(void);
};

#endif
