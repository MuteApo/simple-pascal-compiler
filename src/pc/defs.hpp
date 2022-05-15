#ifndef __DEFS__
#define __DEFS__

#include <string>

enum nodeKind {
    NK_Decl = 1001,
    NK_Type,
    NK_Stmt,
    NK_Expr,
};
enum declKind {
    DK_Prog = 2001,
    DK_Block,
    DK_Const,
    DK_Type,
    DK_Var,
    DK_Func,
    DK_Proc,
    DK_Ref
};
enum typeKind {
    TK_Void = 3001,
    TK_Int,
    TK_Real,
    TK_Char,
    TK_Bool,
    TK_String,
    TK_Nil,
    TK_Enum,
    TK_Range,
    TK_Set,
    TK_Array,
    TK_Record,
    TK_Ptr,
    TK_Id
};
enum stmtKind {
    SK_Compound = 4001,
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
    /* built-in func/proc */
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
enum exprKind {
    /* arithmetic */
    EK_Add = 5001,  // a+b
    EK_Sub,         // a-b
    EK_Mul,         // a*b
    EK_Div,         // a/b
    EK_Mod,         // a%b
    EK_Sign,        // +-a
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
    EK_In,   // in
    /* built-in */
    EK_Literal,
    EK_To,
    EK_Downto,
};

std::string enum2str(int e);

#endif