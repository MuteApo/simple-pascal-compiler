#ifndef __DEFS__
#define __DEFS__

#include <string>

enum declKind {
    DK_Prog = 1001,
    DK_Block,
    DK_Const,
    DK_Type,
    DK_Var,
    DK_Func,
    DK_Proc
};
enum typeKind { TK_Simple = 2001, TK_Def, TK_Array, TK_Record };
enum exprKind { EK_Id = 3001, EK_Literal };
enum exprType {
    ET_Void = 4001,
    ET_Id,
    ET_Int,
    ET_Real,
    ET_Char,
    ET_Bool,
    ET_String,
    ET_Nil,
    ET_Enum,
    ET_Range,
    ET_Set,
    ET_Array,
    ET_Record,
    ET_Addr
};
enum opKind {
    /* arithmetic */
    OK_Add = 5001,  // a+b
    OK_Sub,         // a-b
    OK_Mul,         // a*b
    OK_Div,         // a/b
    OK_Mod,         // a%b
    OK_Sign,        // +-a
    /* comparative */
    OK_Eq,  // =, equal to
    OK_Ne,  // <>, not equal to
    OK_Lt,  // <, less than
    OK_Gt,  // >, greater than
    OK_Le,  // <=, less than or equal to
    OK_Ge,  // >=, greater than or equal to
    /* logical */
    OK_Not,  // not
    OK_And,  // and
    OK_Or,   // or
    OK_Xor,  // xor
    OK_Shl,  // shl
    OK_Shr,  // shr
    OK_In,   // in
    /* built-in */
    OK_Chr,      // chr(x), converts to char type of x
    OK_Ord,      // ord(x), original value of x, usually used for char/enum
    OK_Abs,      // abs(x), absolute value of x
    OK_Odd,      // odd(x), if x is odd(true) or even(false)
    OK_Sqr,      // sqr(x), equals to x*x
    OK_Sqrt,     // sqrt(x), square root of x
    OK_Succ,     // succ(x), which succeeds x
    OK_Pred,     // pred(x), which precedes x
    OK_Read,     // read(x,y), read from input
    OK_Write,    // write(x,y), write to output
    OK_Writeln,  // writeln(x,y), write to output with an extra line feed
    /* assignment */
    OK_Id,
    OK_Array,
    OK_Record
};
enum stmtKind {
    SK_Assign = 6001,
    SK_If,
    SK_For,
    SK_While,
    SK_Repeat,
    SK_Case,
    SK_Sys
};

std::string enum2str(int e);

#endif