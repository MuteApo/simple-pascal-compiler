#include "include/tree.hpp"
#include <string>

std::string enum2str(int e) {
    switch (e) {
        case NK_Decl: return "NK_Decl";
        case NK_Type: return "NK_Type";
        case NK_Stmt: return "NK_Stmt";
        case NK_Expr: return "NK_Expr";

        case DK_Prog: return "DK_Prog";
        case DK_Block: return "DK_Block";
        case DK_Const: return "DK_Const";
        case DK_Type: return "DK_Type";
        case DK_Var: return "DK_Var";
        case DK_Func: return "DK_Func";
        case DK_Proc: return "DK_Proc";
        case DK_Ref: return "DK_Ref";

        case TK_Void: return "TK_Void";
        case TK_Int: return "TK_Int";
        case TK_Real: return "TK_Real";
        case TK_Char: return "TK_Char";
        case TK_Bool: return "TK_Bool";
        case TK_String: return "TK_String";
        case TK_Nil: return "TK_Nil";
        case TK_Enum: return "TK_Enum";
        case TK_Range: return "TK_Range";
        case TK_Set: return "TK_Set";
        case TK_Array: return "TK_Array";
        case TK_Record: return "TK_Record";
        case TK_Ptr: return "TK_Ptr";
        case TK_Id: return "TK_Id";

        case SK_Compound: return "SK_Compound";
        case SK_Assign: return "SK_Assign";
        case SK_If: return "SK_If";
        case SK_Then: return "SK_Then";
        case SK_Else: return "SK_Else";
        case SK_For: return "SK_For";
        case SK_While: return "SK_While";
        case SK_Repeat: return "SK_Repeat";
        case SK_Switch: return "SK_Switch";
        case SK_Case: return "SK_Case";
        case SK_Func: return "SK_Func";
        case SK_Chr: return "SK_Chr";
        case SK_Ord: return "SK_Ord";
        case SK_Abs: return "SK_Abs";
        case SK_Odd: return "SK_Odd";
        case SK_Sqr: return "SK_Sqr";
        case SK_Sqrt: return "SK_Sqrt";
        case SK_Succ: return "SK_Succ";
        case SK_Pred: return "SK_Pred";
        case SK_Read: return "SK_Read";
        case SK_Write: return "SK_Write";
        case SK_Writeln: return "SK_Writeln";

        case EK_Add: return "EK_Add";
        case EK_Sub: return "EK_Sub";
        case EK_Mul: return "EK_Mul";
        case EK_Div: return "EK_Div";
        case EK_Mod: return "EK_Mod";
        case EK_Sign: return "EK_Sign";
        case EK_Eq: return "EK_Eq";
        case EK_Ne: return "EK_Ne";
        case EK_Lt: return "EK_Lt";
        case EK_Gt: return "EK_Gt";
        case EK_Le: return "EK_Le";
        case EK_Ge: return "EK_Ge";
        case EK_Not: return "EK_Not";
        case EK_And: return "EK_And";
        case EK_Or: return "EK_Or";
        case EK_Xor: return "EK_Xor";
        case EK_Shl: return "EK_Shl";
        case EK_Shr: return "EK_Shr";
        case EK_In: return "EK_In";
        case EK_Literal: return "EK_Literal";
        case EK_To: return "EK_To";
        case EK_Downto: return "EK_Downto";
    }
    return "Enum not found: " + std::to_string(e) + "\n";
}