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
    valueWrapper(int v, int i, double d, std::string s) : _valid(v), _ival(i), _dval(d), _sval(s) {}
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
            case 1: return to_string(_ival);
            case 2: return to_string(_dval);
            case 3: return _sval;
        }
        return "";
    }
};

class nodeValue {
  protected:
    typeKind _type;

  public:
    nodeValue(typeKind t) : _type(t) {}
    nodeValue() : nodeValue(TK_Void) {}

    void     setType(typeKind t) { _type = t; }
    typeKind getType() { return _type; }

    virtual valueWrapper getValue() { return valueWrapper(); }

    virtual std::string toString() { return _type == TK_Void ? "" : enum2str(_type); }
};

class typeBasic : public nodeValue {
  protected:
    valueWrapper _value;

  public:
    typeBasic(typeKind t, valueWrapper v) : nodeValue(t), _value(v) {}
    typeBasic() : typeBasic(TK_Void, valueWrapper()) {}
    typeBasic(typeKind t, int i) : typeBasic(t, valueWrapper(i)) {}
    typeBasic(typeKind t, double d) : typeBasic(t, valueWrapper(d)) {}
    typeBasic(typeKind t, std::string s) : typeBasic(t, valueWrapper(s)) {}

    valueWrapper getValue() { return _value; }

    std::string toString() { return (_type == TK_Void ? "" : enum2str(_type) + "\\n") + _value.toString(); }
};

class typeEnum : public nodeValue {
  protected:
    std::vector<valueWrapper> _enum;

  public:
    typeEnum(typeKind t, std::vector<valueWrapper> e) : nodeValue(t), _enum(e) {}
    typeEnum(typeKind t) : nodeValue(t) { _enum.clear(); }
    typeEnum() : typeEnum(TK_Void) {}

    void insert(valueWrapper v) { _enum.push_back(v); }

    std::string toString() {
        std::string str = "";
        for (valueWrapper v : _enum) str += v.toString() + ",";
        str.pop_back();
        return (_type == TK_Void ? "" : enum2str(_type) + "\\n") + "(" + str + ")";
    }
};

class typeRange : public nodeValue {
  protected:
    valueWrapper lower_bound;
    valueWrapper upper_bound;

  public:
    typeRange(typeKind t, valueWrapper l, valueWrapper u) : nodeValue(t), lower_bound(l), upper_bound(u) {}
    typeRange(typeKind t) : typeRange(t, valueWrapper(), valueWrapper()) {}
    typeRange() : typeRange(TK_Void) {}

    std::string toString() {
        return (_type == TK_Void ? "" : enum2str(_type) + "\\n") + lower_bound.toString() + ".." +
               upper_bound.toString();
    }
};

class typeSet : public nodeValue {
  protected:
    std::set<valueWrapper> _set;

  public:
    typeSet(typeKind t, std::set<valueWrapper> s) : nodeValue(t), _set(s) {}
    typeSet(typeKind t) : nodeValue(t) { _set.clear(); }
    typeSet() : typeSet(TK_Void) {}

    std::string toString() {
        std::string str = "";
        for (valueWrapper v : _set) str += v.toString() + ",";
        str.pop_back();
        return (_type == TK_Void ? "" : enum2str(_type) + "\\n") + "[" + str + "]";
    }
};

class typeArray : public nodeValue {
    // TODO
};

class typeRecord : public nodeValue {
    // TODO
};

class treeNode {
  private:
    int                    uid;        // used for graphviz
    std::vector<treeNode*> child;      // vector of children
    treeNode*              sibling;    // pointer to sibling
    int                    line_no;    // line number
    int                    node_kind;  // node kind
    nodeValue*             value;      // node value

  public:
    treeNode(int nk, int ln) : node_kind(nk), line_no(ln) {
        uid = ++global_uid;
        std::cout << "$" << uid << " " << toString() << std::endl;
        child.clear();
        sibling = nullptr;
        value   = nullptr;
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

    int         getNodeKind() { return node_kind; }
    int         getLine() { return line_no; }
    std::string getIdName() {
        std::string s = value->getValue().toString();
        return s;
    }

    nodeValue* getValue() { return value; }
    void       setValue(nodeValue* v) { value = v; }
    void       setValue(int i, typeKind t = TK_Int) { value = new typeBasic(t, i); }
    void       setValue(double d, typeKind t = TK_Real) { value = new typeBasic(t, d); }
    void       setValue(std::string s, typeKind t = TK_String) { value = new typeBasic(t, s); }

    std::string toString() {
        return "line#" + to_string(line_no) + "\\n" + enum2str(node_kind) +
               (value == nullptr ? "" : "\\n" + value->toString());
    }

    static void traverse(FILE* file, int d, treeNode* t) {
        if (t == nullptr) return;

        /* node declaration */
        fprintf(file, "\tnode%d [label=\"%s\"];\n", t->uid, t->toString().c_str());

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
            fprintf(file, "\t{rank=same; node%d -> node%d [color=red];}\n", t->uid, s->uid);
            traverse(file, d, s);
        }
    }
};

extern treeNode* root;

#endif