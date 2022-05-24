#include "include/defs.hpp"

std::string vizNode(int uid, std::string info) {
    return "\tnode" + std::to_string(uid) + " [label=\"" + info + "\"];\n";
}

std::string vizEdge(int fa_uid, int son_uid, std::string info, std::string tip) {
    return "\tnode" + std::to_string(fa_uid) + " -> node" + std::to_string(son_uid) +
           " [color=blue, label = \"" + info + "\", labeltooltip = \"" + tip + "\"];\n";
}