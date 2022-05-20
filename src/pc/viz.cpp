#include "include/viz.hpp"

std::string vizNode(int uid, std::string info) {
    return "\tnode" + std::to_string(uid) + " [label=\"" + info + "\"];\n";
}

std::string vizChildEdge(int fa_uid, int son_uid) {
    return "\tnode" + std::to_string(fa_uid) + " -> node" + std::to_string(son_uid) +
           " [color=blue];\n";
}