#ifndef __NODE__
#define __NODE__

#include <string>
#include <vector>

enum declKind { DK_Prog = 1001, DK_Var, DK_Type, DK_Func, DK_Proc };
enum typeKind { TK_Simple = 2001, TK_Array, TK_Record };
enum exprKind { EK_Id = 3001, EK_Literal, EK_Op };
enum exprType {
    ET_Void = 4001,
    ET_Int,
    ET_Real,
    ET_Char,
    ET_String,
    ET_Bool,
    ET_Array,
    ET_Record
};
enum opKind {
    /* arithmatic */
    OK_Add = 5001,  // a+b
    OK_Sub,         // a-b
    OK_Mul,         // a*b
    OK_Div,         // a/b
    OK_Mod,         // a%b
    OK_Neg,         // -a
    /* comparative */
    OK_Eq,   // =, equal to
    OK_Neq,  // <>, not equal to
    OK_Lt,   // <, less than
    OK_Gt,   // >, greater than
    OK_Leq,  // <=, less than or eqaul to
    OK_Geq,  // >=, greater than or equal to
    /* logical */
    OK_Not,  // not
    OK_And,  // and
    OK_Or,   // or
    OK_Xor,  // xor
    OK_Shl,  // shl
    OK_Shr,  // shr
    /* built-in */
    OK_Chr,   // chr(x), converts to char type of x
    OK_Ord,   // ord(x), original value of x, usually used for char/enum
    OK_Abs,   // abs(x), absolute value of x
    OK_Odd,   // odd(x), if x is odd(true) or even(false)
    OK_Sqr,   // sqr(x), equals to x*x
    OK_Sqrt,  // sqrt(x), squre root of x
    OK_Succ,  // succ(x), which succeeds x
    OK_Pred,  // pred(x), which precedes x
    OK_Write,
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
    SK_Write
};

class nodeValue {
  public:
    int         _ival;
    double      _dval;
    std::string _sval;
    // TODO array type
    // TODO record type
    nodeValue() : _ival(0), _dval(0), _sval("") {}
};

class syntaxNode {
  private:
    std::vector<syntaxNode *> child;
    syntaxNode               *sibling;
    int                       node_type;
    nodeValue                 value;
    exprType                  expr_type;
    int                       line_no;

  public:
    syntaxNode(declKind dk, int ln) : node_type(dk), line_no(ln) {
        child.clear();
        sibling = nullptr;
    }
    syntaxNode(typeKind tk, int ln) : node_type(tk), line_no(ln) {
        child.clear();
        sibling = nullptr;
    }
    syntaxNode(exprKind ek, int ln) : node_type(ek), line_no(ln) {
        child.clear();
        sibling = nullptr;
    }
    syntaxNode(stmtKind sk, int ln) : node_type(sk), line_no(ln) {
        child.clear();
        sibling = nullptr;
    }
    syntaxNode(syntaxNode *op1, syntaxNode *op2, opKind ok, int ln)
            : node_type(EK_Op), line_no(ln) {
        child.clear();
        addChild(op1);
        if (op2 != nullptr) addChild(op2);
        sibling = nullptr;
        setValue(ok);
    }

    void addChild(syntaxNode *c) { child.push_back(c); }

    syntaxNode *getSibling() { return sibling; }
    void        setSibling(syntaxNode *s) { sibling = s; }
    syntaxNode *lastSibling() {
        syntaxNode *t = this;
        while (t->getSibling() != nullptr) t = t->getSibling();
        return t;
    }

    nodeValue getValue() { return value; }
    void      setValue(nodeValue v) { value = v; }
    void      setValue(int i) { value._ival = i; }
    void      setValue(double d) { value._dval = d; }
    void      setValue(std::string s) { value._sval = s; }

    void setExprType(exprType et) { expr_type = et; }

    void        print() { printf("%d %d\n", node_type, line_no); }
    static void travel(syntaxNode *t) {
        if (t == nullptr) return;
        t->print();
        for (syntaxNode *c : t->child) travel(c);
        travel(t->getSibling());
    }
};

#endif