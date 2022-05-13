#ifndef __TREE__
#define __TREE__

#include "defs.hpp"
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

extern int global_uid;

template <class T> std::string to_string(T v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

class valueWrapper {
  private:
    int         _valid;  // serve as selector
    int         _ival;
    double      _dval;
    std::string _sval;

  public:
    valueWrapper(int v, int i, double d, std::string s)
            : _valid(v), _ival(i), _dval(d), _sval(s) {}
    valueWrapper() : valueWrapper(0, 0, 0, "") {}
    valueWrapper(int i) : valueWrapper(1, i, 0, "") {}
    valueWrapper(double d) : valueWrapper(2, 0, d, "") {}
    valueWrapper(std::string s) : valueWrapper(3, 0, 0, s) {}

    template <class T> T getValue() {
        switch (_valid) {
            case 1: return _ival;
            case 2: return _dval;
            case 3: return _sval;
        }
    }
    std::string toString() {
        switch (_valid) {
            case 1: return "ival=" + to_string(_ival);
            case 2: return "dval=" + to_string(_dval);
            case 3: return "sval=" + _sval;
        }
        return "";
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

    std::string toString() {
        return enum2str(_type) + "\\n" + _value.toString();
    }
};

class treeNode {
  private:
    int                    uid;        // used for graphviz
    std::vector<treeNode*> child;      // vector of children
    treeNode*              sibling;    // pointer to sibling
    int                    line_no;    // line number
    int                    node_kind;  // node kind
    nodeValue              value;      // node value

  public:
    treeNode(int nk, int ln) : node_kind(nk), line_no(ln) {
        uid = ++global_uid;
        child.clear();
        sibling = nullptr;
    }
    treeNode(treeNode* op1, treeNode* op2, opKind ok, int ln)
            : node_kind(ok), line_no(ln) {
        uid = ++global_uid;
        child.clear();
        sibling = nullptr;
        addChild(op1), addChild(op2);
    }

    void addChild(treeNode* c) {
        if (c != nullptr) child.push_back(c);
    }
    treeNode* firstChild() { return child.front(); }

    treeNode* getSibling() { return sibling; }
    void      setSibling(treeNode* s) { sibling = s; }
    treeNode* lastSibling() {
        treeNode* t = this;
        while (t->getSibling() != nullptr) t = t->getSibling();
        return t;
    }

    nodeValue& getValue() { return value; }
    void       setValue(nodeValue v) { value = v; }
    void       setValue(int i, exprType t = ET_Int) { value = nodeValue(t, i); }
    void setValue(double d, exprType t = ET_Real) { value = nodeValue(t, d); }
    void setValue(std::string s, exprType t = ET_String) {
        value = nodeValue(t, s);
    }

    std::string toString() {
        return "line#" + std::to_string(line_no) + "\\n" + enum2str(node_kind) +
               "\\n" + value.toString();
    }

    static void traverse(int d, treeNode* t) {
        // node declaration
        printf("\tnode%d [label=\"%s\"];\n", t->uid, t->toString().c_str());

        // ensure child-wise left-to-right layout using invisible edges
        for (int i = 1; i < t->child.size(); i++)
            printf("\t{rank=same; node%d -> node%d [style=invis];}\n",
                   t->child[i - 1]->lastSibling()->uid,
                   t->child[i]->uid);

        // draw child relationship in blue
        for (treeNode* c : t->child) {
            printf("\tnode%d -> node%d [color=blue];\n", t->uid, c->uid);
            traverse(d + 1, c);
        }

        // draw sibling relationship in red, if exists
        treeNode* s = t->getSibling();
        if (s != nullptr) {
            printf("\t{rank=same; node%d -> node%d [color=red];}\n",
                   t->uid,
                   s->uid);
            traverse(d, s);
        }
    }
};

extern treeNode* root;

#endif