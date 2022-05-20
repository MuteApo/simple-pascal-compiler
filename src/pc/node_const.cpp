#include "include/node_const.hpp"
#include "include/symbol_table.hpp"

bool ConstDefNode::gen_sym_tab() {
    return symbol_table.addSymbol(name, this);
}