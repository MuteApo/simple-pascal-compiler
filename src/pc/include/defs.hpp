#ifndef __DEFS__
#define __DEFS__

#include <string>

enum nodeKind {
    NK_Decl = 1001,
    NK_Type,
    NK_Stmt,
    NK_Expr,
};
enum declKind { DK_Prog = 2001, DK_Block, DK_Const, DK_Type, DK_Var, DK_Func, DK_Proc, DK_Ref };
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
enum stmt_type {
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
enum ExprEvalType {
    EK_None = 5001,
    /* arithmetic */
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
    EK_In,   // in
    /* temporary(will be removed) */
    EK_Access,
    EK_Address
};
enum errorType {
    NO_PERIOD,        // 缺少句点
    ILLEGAL_CHAR,     // 非法字符
    NO_FINISH,        // 希望结束而没有结束
    FINISH,           // 程序尚不完整而文件结束
    NO_CONST,         // 缺少 const 关键字
    NO_SEMICOLON,     // 缺少分号
    NO_IDENTIFIER,    // 缺少标识符
    REDEFINE,         // 标识符重定义
    NO_EQUAL,         // 缺少等号
    NO_CHNUM,         // 缺少字符或整数（<常量>）
    NO_SINGLEQUOTE,   // 缺少单引号
    NO_DOUBLEQUOTE,   // 缺少双引号
    ILLEGAL_STRING,   // 非法字符串
    NO_NUMBER,        // 希望是数字而不是
    INT_OVERFLOW,     // 整数溢出
    NO_VAR,           // 缺少 var 关键字
    NO_COLON,         // 缺少冒号
    NO_LEFTSQUARE,    // 缺少左方括号
    NO_RIGHTSQUARE,   // 缺少右方括号
    NO_OF,            // 缺少 of 关键字
    NO_BASICTYPE,     // 缺少基本类型
    NO_PROCEDURE,     // 缺少 procedure 关键字
    NO_FUNCTION,      // 缺少 function 关键字
    NO_LEFTBRACKET,   // 缺少左括号
    NO_RIGHTBRACKET,  // 缺少右括号
    CONSTANT,         // 意外的常量标识符
    NO_ASSIGN,        // 缺少赋值符，即希望是赋值语句，实际不是
    NO_VALUE,         // 缺少值
    UNDEFINED,        // 标识符未定义
    CANNOT_CALL,      // 标识符不能用于函数或过程调用
    INT_TO_CHAR,      // 将整型数转化为字符型
    TOO_LESS_ARG,     // 参数太少
    TOO_MANY_ARG,     // 参数太多
    UNVARIABLE,       // 需要变量表达式
    NO_RELATION,      // 需要关系运算符而没有
    NO_IF,            // 缺少 if 关键字
    NO_THEN,          // 缺少 then 关键字
    NO_ELSE,          // 缺少 else 关键字
    NO_DO,            // 缺少 do 关键字
    NO_WHILE,         // 缺少 while 关键字
    NO_FOR,           // 缺少 for 关键字
    NO_DOWN_TO,       // 缺少 to 或 downto 关键字
    NO_BEGIN,         // 缺少 begin 关键字
    NO_END,           // 缺少 end 关键字
    NO_READ,          // 缺少 read 关键字
    NO_WRITE          // 缺少 write 关键字
};

std::string enum2str(int e);

#endif