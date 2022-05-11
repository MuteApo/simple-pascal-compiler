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
    OK_Add = 5001,  // +
    OK_Sub,         // -
    OK_Mul,         // *
    OK_Div,         // /
    OK_Mod,         // %
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
enum stmtKind { SK_Assign = 6001, SK_If, SK_For, SK_While, SK_Repeat, SK_Sys };

class nodeAttr {
  public:
    int         _ival;
    double      _dval;
    std::string _sval;
    // TODO
    // array type
    // record type
    nodeAttr() : _ival(0), _dval(0), _sval("") {}
};

typedef class syntaxNode *ptrSyntaxNode;
class syntaxNode {
  private:
    std::vector<ptrSyntaxNode> child;
    ptrSyntaxNode              sibling;
    nodeAttr                   attr;
    int                        node_type;
    exprType                   expr_type;
    int                        line_no;

  public:
    syntaxNode(declKind dk, int ln) : node_type(dk), line_no(ln) {
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
    syntaxNode(typeKind tk, int ln) : node_type(tk), line_no(ln) {
        child.clear();
        sibling = nullptr;
    }
    syntaxNode(ptrSyntaxNode op1, ptrSyntaxNode op2, opKind ok, int ln)
            : node_type(EK_Op), line_no(ln) {
        child.clear();
        addChild(op1);
        addChild(op2);
        sibling = nullptr;
        setAttr(ok);
    }

    void addChild(ptrSyntaxNode c) { child.push_back(c); }

    ptrSyntaxNode getSibling() { return sibling; }
    void          setSibling(ptrSyntaxNode s) { sibling = s; }
    ptrSyntaxNode lastSibling() {
        ptrSyntaxNode t = this;
        while (t->getSibling() != nullptr) t = t->getSibling();
        return t;
    }

    nodeAttr getAttr() { return attr; }
    void     setAttr(nodeAttr a) { attr = a; }
    void     setAttr(int i) { attr._ival = i; }
    void     setAttr(double d) { attr._dval = d; }
    void     setAttr(std::string s) { attr._sval = s; }

    void setExprType(exprType et) { expr_type = et; }
};

#endif