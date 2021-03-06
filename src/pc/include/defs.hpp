#ifndef __DEFS_H__
#define __DEFS_H__

#include <sstream>
#include <string>

enum ExprNodeType { el_nonleaf = 101, el_literal, el_var_access, el_id, el_fun_call };
enum ResultType { rt_unknown = 201, rt_integer, rt_real, rt_boolean, rt_pointer, rt_record };
enum LiteralType { lt_pointer = 301, lt_basic, lt_string };
enum VarAccessType { va_pointer = 401, va_array, va_record };
enum ExprEvalType {
    EK_None = 501,
    /* arithmetic */
    EK_Pos,   // +a
    EK_Neg,   //-a
    EK_Add,   // a+b
    EK_Sub,   // a-b
    EK_Mul,   // a*b
    EK_Div,   // a/b
    EK_Mod,   // a%b
    EK_Fdiv,  // a/b
    /* comparative */
    EK_Eq,  // =, equal to
    EK_Ne,  // <>, not equal to
    EK_Lt,  // <, less than
    EK_Gt,  // >, greater than
    EK_Le,  // <=, less than or equal to
    EK_Ge,  // >=, greater than or equal to
    /* logical */
    EK_Not,  // not
    EK_And,  // and
    EK_Or,   // or
    EK_Xor,  // xor
    EK_Shl,  // shl
    EK_Shr,  // shr
    EK_In    // in
};

enum TypeKind { basic = 1001, ordinal, structured, pointer, type_identifier };
enum BasicTypeKind { boolean = 2001, integer, real, character };
enum StructTypeKind { struct_array = 3001, struct_string, struct_record };

enum StmtType {
    SK_Compound = 10001,
    SK_Assign,
    SK_If,
    SK_Then,
    SK_Else,
    SK_For,
    SK_While,
    SK_Repeat,
    SK_Switch,
    SK_Case,
    SK_Func,
    /* built-in (optional) */
    SK_Chr,      // chr(x), converts to char type of x
    SK_Ord,      // ord(x), original value of x, usually used for char/enum
    SK_Abs,      // abs(x), absolute value of x
    SK_Odd,      // odd(x), if x is odd(true) or even(false)
    SK_Sqr,      // sqr(x), equals to x*x
    SK_Sqrt,     // sqrt(x), square root of x
    SK_Succ,     // succ(x), which succeeds x
    SK_Pred,     // pred(x), which precedes x
    SK_Read,     // read(x,y), read from input
    SK_Write,    // write(x,y), write to output
    SK_Writeln,  // writeln(x,y), write to output with an extra line feed
};

typedef enum { VTI_Global = 20001, VTI_ArgVal, VTI_ArgVar, VTI_RetVal, VTI_Local } VarTableItemType;

std::string vizNode(int uid, std::string info);

std::string vizEdge(int fa_uid, int son_uid, std::string info = "", std::string tip = "");

template <class T> std::string to_string(T v) {
    std::ostringstream oss;
    oss << std::boolalpha << v;
    return oss.str();
}

#endif