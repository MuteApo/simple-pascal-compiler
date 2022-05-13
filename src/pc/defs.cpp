#include "tree.hpp"
#include <string>

std::string enum2str(int e) {
    switch (e) {
        case DK_Prog: return "DK_Prog";
        case DK_Block: return "DK_Block";
        case DK_Const: return "DK_Const";
        case DK_Var: return "DK_Var";
        case DK_Type: return "DK_Type";
        case DK_Func: return "DK_Func";
        case DK_Proc: return "DK_Proc";

        case TK_Simple: return "TK_Simple";
        case TK_Def: return "TK_Def";
        case TK_Array: return "TK_Array";
        case TK_Record: return "TK_Record";

        case EK_Id: return "EK_Id";
        case EK_Literal: return "EK_Literal";

        case ET_Void: return "ET_Void";
        case ET_Id: return "ET_Id";
        case ET_Int: return "ET_Int";
        case ET_Real: return "ET_Real";
        case ET_Char: return "ET_Char";
        case ET_Bool: return "ET_Bool";
        case ET_String: return "ET_String";
        case ET_Nil: return "ET_Nil";
        case ET_Enum: return "ET_Enum";
        case ET_Range: return "ET_Range";
        case ET_Set: return "ET_Set";
        case ET_Array: return "ET_Array";
        case ET_Record: return "ET_Record";
        case ET_Addr: return "ET_Addr";

        case OK_Add: return "OK_Add";
        case OK_Sub: return "OK_Sub";
        case OK_Mul: return "OK_Mul";
        case OK_Div: return "OK_Div";
        case OK_Mod: return "OK_Mod";
        case OK_Sign: return "OK_Sign";
        case OK_Eq: return "OK_Eq";
        case OK_Ne: return "OK_Ne";
        case OK_Lt: return "OK_Lt";
        case OK_Gt: return "OK_Gt";
        case OK_Le: return "OK_Le";
        case OK_Ge: return "OK_Ge";
        case OK_Not: return "OK_Not";
        case OK_And: return "OK_And";
        case OK_Or: return "OK_Or";
        case OK_Xor: return "OK_Xor";
        case OK_Shl: return "OK_Shl";
        case OK_Shr: return "OK_Shr";
        case OK_Chr: return "OK_Chr";
        case OK_Ord: return "OK_Ord";
        case OK_Abs: return "OK_Abs";
        case OK_Odd: return "OK_Odd";
        case OK_Sqr: return "OK_Sqr";
        case OK_Sqrt: return "OK_Sqrt";
        case OK_Succ: return "OK_Succ";
        case OK_Pred: return "OK_Pred";
        case OK_Read: return "OK_Read";
        case OK_Write: return "OK_Write";
        case OK_Writeln: return "OK_Writeln";
        case OK_Id: return "OK_Id";
        case OK_Array: return "OK_Array";
        case OK_Record: return "OK_Record";

        case SK_Assign: return "SK_Assign";
        case SK_If: return "SK_If";
        case SK_For: return "SK_For";
        case SK_While: return "SK_While";
        case SK_Repeat: return "SK_Repeat";
        case SK_Case: return "SK_Case";
        case SK_Sys: return "SK_Sys";
    }
    return "Enum not found: " + std::to_string(e) + "\n";
}