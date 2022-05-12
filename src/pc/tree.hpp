#ifndef __TREE__
#define __TREE__

#include <iostream>
#include <set>
#include <string>
#include <vector>

enum declKind { DK_Prog = 1001, DK_Const, DK_Var, DK_Type, DK_Func, DK_Proc };
enum typeKind { TK_Simple = 2001, TK_Array, TK_Record };
enum exprKind { EK_Id = 3001, EK_Literal };
enum exprType {
    ET_Void = 4001,
    ET_Int,
    ET_Real,
    ET_Char,
    ET_Bool,
    ET_String,
    ET_Enum,
    ET_Range,
    ET_Array,
    ET_Record
};
enum opKind {
    /* arithmetic */
    OK_Add = 5001,  // a+b
    OK_Sub,         // a-b
    OK_Mul,         // a*b
    OK_Div,         // a/b
    OK_Mod,         // a%b
    OK_Neg,         // -a
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
    /* built-in */
    OK_Chr,   // chr(x), converts to char type of x
    OK_Ord,   // ord(x), original value of x, usually used for char/enum
    OK_Abs,   // abs(x), absolute value of x
    OK_Odd,   // odd(x), if x is odd(true) or even(false)
    OK_Sqr,   // sqr(x), equals to x*x
    OK_Sqrt,  // sqrt(x), square root of x
    OK_Succ,  // succ(x), which succeeds x
    OK_Pred,  // pred(x), which precedes x
    OK_Read,
    OK_Write,
    OK_Writeln,
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

class valueWrapper {
  public:
    int         _valid;
    int         _ival;
    double      _dval;
    std::string _sval;
    valueWrapper(int v, int i, double d, std::string s)
            : _valid(v), _ival(i), _dval(d), _sval(s) {}
    valueWrapper() : valueWrapper(0, 0, 0, "") {}
    valueWrapper(int i) : valueWrapper(1, i, 0, "") {}
    valueWrapper(double d) : valueWrapper(2, 0, d, "") {}
    valueWrapper(std::string s) : valueWrapper(3, 0, 0, s) {}
    void print() {
        switch (_valid) {
            case 0: break;
            case 1: std::cout << "ival=" << _ival << " "; break;
            case 2: std::cout << "dval=" << _dval << " "; break;
            case 3: std::cout << "sval=" << _sval << " "; break;
        }
        std::cout << std::endl;
    }
};

class typeEnum {
  private:
    std::set<valueWrapper> set;

  public:
    typeEnum(std::set<valueWrapper> s) : set(s) {}
    typeEnum() { set.clear(); }
};

class typeRange {
  private:
    exprType     range_type;
    valueWrapper lower_bound;
    valueWrapper upper_bound;

  public:
    typeRange(exprType t, valueWrapper l, valueWrapper u)
            : range_type(t), lower_bound(l), upper_bound(u) {}
    typeRange() : typeRange(ET_Void, valueWrapper(), valueWrapper()) {}
};

class typeSet {
  private:
    exprType               set_type;
    std::set<valueWrapper> set;

  public:
    typeSet(exprType t, std::set<valueWrapper> s) : set_type(t), set(s) {}
    typeSet() : set_type(ET_Void) { set.clear(); }
};

class typeArray {
  private:
    exprType ele_type;
    typeSet  indices;

  public:
    typeArray(exprType t, typeSet s) : ele_type(t), indices(s) {}
    typeArray() : ele_type(ET_Void), indices(typeSet()) {}
};

class nodeValue {
  private:
    exprType     _type;
    valueWrapper _value;
    typeEnum     _enum;
    typeRange    _range;
    typeSet      _set;
    typeArray    _array;
    // TODO record type
  public:
    nodeValue(exprType     t,
              valueWrapper v,
              typeEnum     e,
              typeRange    r,
              typeSet      s,
              typeArray    a)
            : _type(t), _value(v), _enum(e), _range(r), _set(s), _array(a) {}
    nodeValue()
            : nodeValue(ET_Void,
                        valueWrapper(),
                        typeEnum(),
                        typeRange(),
                        typeSet(),
                        typeArray()) {}
    nodeValue(exprType t, valueWrapper v)
            : nodeValue(t, v, typeEnum(), typeRange(), typeSet(), typeArray()) {
    }
    nodeValue(exprType t, int i) : nodeValue(t, valueWrapper(i)) {}
    nodeValue(exprType t, double d) : nodeValue(t, valueWrapper(d)) {}
    nodeValue(exprType t, std::string s) : nodeValue(t, valueWrapper(s)) {}
    nodeValue(exprType t, typeEnum e)
            : nodeValue(
                  t, valueWrapper(), e, typeRange(), typeSet(), typeArray()) {}
    nodeValue(exprType t, typeRange r)
            : nodeValue(
                  t, valueWrapper(), typeEnum(), r, typeSet(), typeArray()) {}
    nodeValue(exprType t, typeSet s)
            : nodeValue(
                  t, valueWrapper(), typeEnum(), typeRange(), s, typeArray()) {}
    nodeValue(exprType t, typeArray a)
            : nodeValue(
                  t, valueWrapper(), typeEnum(), typeRange(), typeSet(), a) {}
    void setType(exprType t) { _type = t; }
    void print() {
        printf("%d ", _type);
        _value.print();
    }
};

class treeNode {
  private:
    std::vector<treeNode*> child;
    treeNode*              sibling;
    int                    line_no;
    int                    node_kind;
    nodeValue              value;

  public:
    treeNode(int nk, int ln) : node_kind(nk), line_no(ln) {
        child.clear();
        sibling = nullptr;
    }
    treeNode(treeNode* op1, treeNode* op2, opKind ok, int ln)
            : node_kind(ok), line_no(ln) {
        child.clear();
        sibling = nullptr;
        addChild(op1), addChild(op2);
    }

    void addChild(treeNode* c) {
        if (c != nullptr) child.push_back(c);
    }

    treeNode* getSibling() { return sibling; }
    void      setSibling(treeNode* s) { sibling = s; }
    treeNode* lastSibling() {
        treeNode* t = this;
        while (t->getSibling() != nullptr) t = t->getSibling();
        return t;
    }

    nodeValue getValue() { return value; }
    void      setValue(nodeValue v) { value = v; }
    void      setValue(int i, exprType t = ET_Int) { value = nodeValue(t, i); }
    void setValue(double d, exprType t = ET_Real) { value = nodeValue(t, d); }
    void setValue(std::string s, exprType t = ET_String) {
        value = nodeValue(t, s);
    }

    void print() {
        printf("line#%d nkind=%d child=%d ",
               line_no,
               node_kind,
               (int)child.size());
        value.print();
    }
    static void travel(treeNode* t) {
        if (t == nullptr) return;
        t->print();
        for (treeNode* c : t->child) travel(c);
        travel(t->getSibling());
    }
};

extern treeNode* root;

#endif