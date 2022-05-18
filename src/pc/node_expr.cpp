#include "include/node_expr.hpp"
#include "include/symbol_table.hpp"

TypeAttrNode *IdNode::get_type() {
    return symbol_table.findTypeSymbol(name);
}