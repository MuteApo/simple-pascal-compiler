#include "include/node_const.hpp"
#include "include/symbol_table.hpp"

void ConstDefNode::add_to_symtbl() {
    symbol_table.addSymbol(name, this);
}