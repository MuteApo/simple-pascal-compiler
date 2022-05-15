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
            case 1: return "[i]" + to_string(_ival);
            case 2: return "[d]" + to_string(_dval);
            case 3: return "[s]" + _sval;
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
    typeKind     range_type;
    valueWrapper lower_bound;
    valueWrapper upper_bound;

  public:
    typeRange(typeKind t, valueWrapper l, valueWrapper u)
            : range_type(t), lower_bound(l), upper_bound(u) {}
    typeRange() : typeRange(TK_Void, valueWrapper(), valueWrapper()) {}
};

class typeSet {
  private:
    typeKind               set_type;
    std::set<valueWrapper> set;

  public:
    typeSet(typeKind t, std::set<valueWrapper> s) : set_type(t), set(s) {}
    typeSet() : set_type(TK_Void) { set.clear(); }
};

class typeArray {
  private:
    typeKind ele_type;
    typeSet  indices;

  public:
    typeArray(typeKind t, typeSet s) : ele_type(t), indices(s) {}
    typeArray() : ele_type(TK_Void), indices(typeSet()) {}
};

class nodeValue {
  private:
    typeKind     _type;
    valueWrapper _value;
    typeEnum     _enum;
    typeRange    _range;
    typeSet      _set;
    typeArray    _array;
    // TODO record type
  public:
    nodeValue(typeKind     t,
              valueWrapper v,
              typeEnum     e,
              typeRange    r,
              typeSet      s,
              typeArray    a)
            : _type(t), _value(v), _enum(e), _range(r), _set(s), _array(a) {}
    nodeValue()
            : nodeValue(TK_Void,
                        valueWrapper(),
                        typeEnum(),
                        typeRange(),
                        typeSet(),
                        typeArray()) {}
    nodeValue(typeKind t, valueWrapper v)
            : nodeValue(t, v, typeEnum(), typeRange(), typeSet(), typeArray()) {
    }
    nodeValue(typeKind t, int i) : nodeValue(t, valueWrapper(i)) {}
    nodeValue(typeKind t, double d) : nodeValue(t, valueWrapper(d)) {}
    nodeValue(typeKind t, std::string s) : nodeValue(t, valueWrapper(s)) {}
    nodeValue(typeKind t, typeEnum e)
            : nodeValue(
                  t, valueWrapper(), e, typeRange(), typeSet(), typeArray()) {}
    nodeValue(typeKind t, typeRange r)
            : nodeValue(
                  t, valueWrapper(), typeEnum(), r, typeSet(), typeArray()) {}
    nodeValue(typeKind t, typeSet s)
            : nodeValue(
                  t, valueWrapper(), typeEnum(), typeRange(), s, typeArray()) {}
    nodeValue(typeKind t, typeArray a)
            : nodeValue(
                  t, valueWrapper(), typeEnum(), typeRange(), typeSet(), a) {}

    void setType(typeKind t) { _type = t; }

    valueWrapper& getValue() { return _value; }

    std::string toString() {
        return (_type == TK_Void ? "" : enum2str(_type) + "\\n") +
               _value.toString();
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
    treeNode(treeNode* tn1, treeNode* tn2, int nk, int ln) : treeNode(nk, ln) {
        addChild(tn1);
        addChild(tn2);
    }

    std::vector<treeNode*>& getChild() { return child; }
    void                    addChild(treeNode* c) {
        if (c != nullptr) child.push_back(c);
    }
    treeNode* firstChild() { return child.front(); }

    treeNode* getSibling() { return sibling; }
    void      setSibling(treeNode* s) {
        if (s != nullptr) sibling = s;
    }
    treeNode* lastSibling() {
        treeNode* t = this;
        while (t->getSibling() != nullptr) t = t->getSibling();
        return t;
    }
    /* add tn2 to the end of the sibling linked-list started at tn1 */
    static treeNode* linkSibling(treeNode* tn1, treeNode* tn2) {
        if (tn1 != nullptr) {
            tn1->lastSibling()->setSibling(tn2);
            return tn1;
        } else
            return tn2;
    }

    nodeValue& getValue() { return value; }
    void       setValue(nodeValue v) { value = v; }
    void       setValue(int i, typeKind t = TK_Int) { value = nodeValue(t, i); }
    void setValue(double d, typeKind t = TK_Real) { value = nodeValue(t, d); }
    void setValue(std::string s, typeKind t = TK_String) {
        value = nodeValue(t, s);
    }

    std::string toString() {
        return "line#" + std::to_string(line_no) + "\\n" + enum2str(node_kind) +
               "\\n" + value.toString();
    }

    static void traverse(FILE* file, int d, treeNode* t) {
        if (t == nullptr) return;

        /* node declaration */
        fprintf(
            file, "\tnode%d [label=\"%s\"];\n", t->uid, t->toString().c_str());

        /* ensure child-wise left-to-right layout using invisible edges */
        for (int i = 1; i < t->child.size(); i++)
            fprintf(file,
                    "\t{rank=same; node%d -> node%d [style=invis];}\n",
                    t->child[i - 1]->lastSibling()->uid,
                    t->child[i]->uid);

        /* draw child relationship in blue */
        for (treeNode* c : t->child) {
            fprintf(file, "\tnode%d -> node%d [color=blue];\n", t->uid, c->uid);
            traverse(file, d + 1, c);
        }

        /* draw sibling relationship in red, if exists */
        treeNode* s = t->getSibling();
        if (s != nullptr) {
            fprintf(file,
                    "\t{rank=same; node%d -> node%d [color=red];}\n",
                    t->uid,
                    s->uid);
            traverse(file, d, s);
        }
    }
};

extern treeNode* root;

#endif