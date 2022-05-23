#include "include/viz.hpp"

std::string enum2str(int e) {
    switch (e) {
        case EK_Add: return "Add";
        case EK_Sub: return "Sub";
        case EK_Mul: return "Mul";
        case EK_Div: return "Div";
        case EK_Mod: return "Mod";
        case EK_Fdiv: return "Float Div";
        case EK_Eq: return "=";
        case EK_Ne: return "<>";
        case EK_Lt: return "<";
        case EK_Gt: return ">";
        case EK_Le: return "<=";
        case EK_Ge: return ">=";
        case EK_Not: return "!";
        case EK_And: return "&";
        case EK_Or: return "|";
        case EK_Xor: return "^";
        case EK_Shl: return "<<";
        case EK_Shr: return ">>";
        case EK_In: return "In";
        case EK_Access: return "Access";
    }
    return "Enum not found: " + std::to_string(e) + "\n";
}

std::string vizNode(int uid, std::string info) {
    return "\tnode" + std::to_string(uid) + " [label=\"" + info + "\"];\n";
}

std::string vizEdge(int fa_uid, int son_uid, std::string info, std::string tip) {
    return "\tnode" + std::to_string(fa_uid) + " -> node" + std::to_string(son_uid) +
           " [color=blue, label = \"" + info + "\", labeltooltip = \"" + tip + "\"];\n";
}