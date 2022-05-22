#include "include/defs.hpp"
#include <string>

std::string enum2str(int e) {
    switch (e) {
            //     case NK_Decl: return "NK_Decl";
            //     case NK_Type: return "NK_Type";
            //     case NK_Stmt: return "NK_Stmt";
            //     case NK_Expr: return "NK_Expr";

            //     case DK_Prog: return "DK_Prog";
            //     case DK_Block: return "DK_Block";
            //     case DK_Const: return "DK_Const";
            //     case DK_Type: return "DK_Type";
            //     case DK_Var: return "DK_Var";
            //     case DK_Func: return "DK_Func";
            //     case DK_Proc: return "DK_Proc";
            //     case DK_Ref: return "DK_Ref";

            //     case TK_Void: return "TK_Void";
            //     case TK_Int: return "TK_Int";
            //     case TK_Real: return "TK_Real";
            //     case TK_Char: return "TK_Char";
            //     case TK_Bool: return "TK_Bool";
            //     case TK_String: return "TK_String";
            //     case TK_Nil: return "TK_Nil";
            //     case TK_Enum: return "TK_Enum";
            //     case TK_Range: return "TK_Range";
            //     case TK_Set: return "TK_Set";
            //     case TK_Array: return "TK_Array";
            //     case TK_Record: return "TK_Record";
            //     case TK_Ptr: return "TK_Ptr";
            //     case TK_Id: return "TK_Id";

            //     case SK_Compound: return "SK_Compound";
            //     case SK_Assign: return "SK_Assign";
            //     case SK_If: return "SK_If";
            //     case SK_Then: return "SK_Then";
            //     case SK_Else: return "SK_Else";
            //     case SK_For: return "SK_For";
            //     case SK_While: return "SK_While";
            //     case SK_Repeat: return "SK_Repeat";
            //     case SK_Switch: return "SK_Switch";
            //     case SK_Case: return "SK_Case";
            //     case SK_Func: return "SK_Func";
            //     case SK_Chr: return "SK_Chr";
            //     case SK_Ord: return "SK_Ord";
            //     case SK_Abs: return "SK_Abs";
            //     case SK_Odd: return "SK_Odd";
            //     case SK_Sqr: return "SK_Sqr";
            //     case SK_Sqrt: return "SK_Sqrt";
            //     case SK_Succ: return "SK_Succ";
            //     case SK_Pred: return "SK_Pred";
            //     case SK_Read: return "SK_Read";
            //     case SK_Write: return "SK_Write";
            //     case SK_Writeln: return "SK_Writeln";

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