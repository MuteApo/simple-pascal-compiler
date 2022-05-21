#ifndef _NODE_TYPE_H_
#define _NODE_TYPE_H_

class TypeDefNode;
class TypeDefListNode;
class TypeAttrNode;
class TypeAttrListNode;
class BasicAttrNode;
class OrdAttrNode;
class SubrangeAttrNode;
class EnumAttrNode;
class StructAttrNode;
class SetAttrNode;
class ArrayAttrNode;
class RecordAttrNode;
class PtrAttrNode;

#include "node_expr.hpp"
#include "node_var.hpp"
#include "viz.hpp"
#include <set>
#include <string>
#include <vector>

extern int global_uid;

#define BASIC_BOOL_LEN 1
#define BASIC_INT_LEN 2
#define BASIC_REAL_LEN 4
#define BASIC_CHAR_LEN 1
#define BASIC_PTR_LEN 4

typedef enum { basic = 101, ordinal, structured, pointer, type_identifier } type_kind;
typedef enum { boolean = 201, integer, real, character } basic_type_kind;

class TypeDefNode {
  private:
    int           uid;
    bool          is_type_id;
    std::string   name;
    TypeAttrNode *type;

  public:
    TypeDefNode(std::string id, TypeAttrNode *t);

    int getUid() {
        return uid;
    }

    bool gen_sym_tab();

    std::string gen_viz_code();
};

class TypeDefListNode {
  private:
    int                        uid;
    std::vector<TypeDefNode *> type_defs;

  public:
    TypeDefListNode() : uid(++global_uid) {
        type_defs.clear();
    }

    int getUid() {
        return uid;
    }

    void addTypeDef(TypeDefNode *type_def) {
        type_defs.push_back(type_def);
    }

    std::string gen_viz_code() {
        std::string result = vizNode(uid, "TypeDefListNode");
        for (int i = 0; i < type_defs.size(); i++) {
            result += vizChildEdge(uid,
                                   type_defs.at(i)->getUid(),
                                   "typedef" + to_string(i + 1),
                                   "Type Definition " + to_string(i + 1));
            result += type_defs.at(i)->gen_viz_code();
        }
        return result;
    }

    bool gen_sym_tab() {
        bool result = true;
        for (TypeDefNode *type : type_defs) result &= type->gen_sym_tab();
        return result;
    }
};

class TypeAttrNode {
  private:
    int             uid;
    type_kind       root_type;
    std::string     name;
    BasicAttrNode  *basic_attr;
    OrdAttrNode    *ord_attr;
    StructAttrNode *struct_attr;
    friend class BasicAttrNode;
    friend class OrdAttrNode;
    friend class SubrangeAttrNode;
    friend class EnumAttrNode;
    friend class StructAttrNode;
    friend class ArrayAttrNode;
    friend class RecordAttrNode;

  public:
    TypeAttrNode(type_kind       type,
                 std::string     id,
                 BasicAttrNode  *b_attr = nullptr,
                 OrdAttrNode    *o_attr = nullptr,
                 StructAttrNode *s_attr = nullptr)
            : uid(++global_uid),
              root_type(type),
              name(id),
              basic_attr(b_attr),
              ord_attr(o_attr),
              struct_attr(s_attr) {}
    TypeAttrNode(void) : TypeAttrNode(pointer, "") {}
    TypeAttrNode(std::string id) : TypeAttrNode(type_identifier, id) {}
    TypeAttrNode(BasicAttrNode *attr_node) : TypeAttrNode(basic, "", attr_node, nullptr, nullptr) {}
    TypeAttrNode(OrdAttrNode *attr_node) : TypeAttrNode(ordinal, "", nullptr, attr_node, nullptr) {}
    TypeAttrNode(StructAttrNode *attr_node)
            : TypeAttrNode(structured, "", nullptr, nullptr, attr_node) {}

    int getUid() {
        return uid;
    }

    std::string getName() {
        return name;
    }

    type_kind getType() {
        return root_type;
    }

    void translateId();

    int get_length(void);

    // -1: Fail to get the offset
    int get_offset(void);                            // For basic type (RealType) or pointer type
    int get_offset(std::vector<int> static_indexs);  // For array type
    int get_offset(std::string member);              // For record type

    bool is_type_equ(TypeAttrNode *type, bool use_struct = true);

    std::string getNodeInfo();

    std::string gen_viz_code();

    std::string gen_asm_def(void) {
        // TODO
        return "";
    }

    std::string toString() {
        return "";
    }
};

class TypeAttrListNode {
  private:
    int                         uid;
    std::vector<bool>           is_type_id;
    std::vector<TypeAttrNode *> type_attrs;

  public:
    TypeAttrListNode() : uid(++global_uid) {
        is_type_id.clear();
        type_attrs.clear();
    }

    int getUid() {
        return uid;
    }

    int getDim() {
        return type_attrs.size();
    }

    void addTypeAttr(TypeAttrNode *type_attr) {
        is_type_id.push_back(type_attr->getType() == type_identifier);
        type_attrs.push_back(type_attr);
    }

    void translateId();

    std::vector<TypeAttrNode *> getAttrList() {
        return type_attrs;
    }

    std::string gen_viz_code() {
        std::string result = vizNode(uid, "TypeAttrListNode");
        for (int i = 0; i < type_attrs.size(); i++) {
            result += vizChildEdge(uid,
                                   type_attrs.at(i)->getUid(),
                                   "index" + to_string(i + 1),
                                   "Type of Index " + to_string(i + 1));
            if (!is_type_id.at(i)) result += type_attrs.at(i)->gen_viz_code();
        }
        return result;
    }

    bool is_type_equ(TypeAttrListNode *type) {
        if (type_attrs.size() != type->type_attrs.size()) return false;
        for (int i = 0; i < type_attrs.size(); i++)
            if (!type_attrs.at(i)->is_type_equ(type->type_attrs.at(i))) return false;
        return true;
    }
};

class BasicAttrNode {
  private:
    int             uid;
    basic_type_kind type;
    friend class LiteralNode;

  public:
    BasicAttrNode(basic_type_kind t) : uid(++global_uid), type(t) {}

    int getUid() {
        return uid;
    }

    std::string getNodeInfo() {
        if (type == boolean) return "boolean";
        if (type == integer) return "integer";
        if (type == real) return "real";
        if (type == character) return "character";
        return "unkown type";
    }

    std::string gen_viz_code() {
        return vizNode(uid, "BasicAttrNode\n" + getNodeInfo());
    }

    basic_type_kind getType();

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(BasicAttrNode *type);
};

class OrdAttrNode {
  private:
    int               uid;
    bool              is_subrange;
    SubrangeAttrNode *subrange_attr;
    EnumAttrNode     *enum_attr;
    friend class TypeAttrNode;
    friend class SubrangeAttrNode;
    friend class EnumAttrNode;
    friend class ArrayAttrNode;

  public:
    OrdAttrNode(SubrangeAttrNode *s_a)
            : uid(++global_uid), is_subrange(true), subrange_attr(s_a), enum_attr(nullptr) {}
    OrdAttrNode(EnumAttrNode *e_a)
            : uid(++global_uid), is_subrange(false), subrange_attr(nullptr), enum_attr(e_a) {}

    int getUid() {
        return uid;
    }

    void translateId();

    int get_length(void);

    int get_size(void);

    int get_offset(void);

    bool is_type_equ(OrdAttrNode *type);

    std::string gen_viz_code();
};

class SubrangeAttrNode {
  private:
    int       uid;
    ExprNode *low_bound, *up_bound;  // Integer or Char
    bool      is_low_id, is_up_id;
    friend class OrdAttrNode;

  public:
    SubrangeAttrNode(ExprNode *lb, ExprNode *ub);

    int getUid() {
        return uid;
    }

    void translateId();

    int get_length(void);

    int get_size(void);

    int get_offset(void);

    bool is_type_equ(SubrangeAttrNode *type);

    std::string gen_viz_code();
};

class EnumAttrNode {
  private:
    int                      uid;
    std::vector<std::string> items;
    friend class OrdAttrNode;

  public:
    EnumAttrNode(std::vector<ExprNode *> exprs);

    int getUid() {
        return uid;
    }

    int get_length(void);

    int get_size(void);

    int get_offset(void);

    bool is_type_equ(EnumAttrNode *type);

    std::string getNodeInfo();

    std::string gen_viz_code() {
        return vizNode(uid, getNodeInfo());
    }
};

class StructAttrNode {
  private:
    int             uid;
    bool            is_array;
    ArrayAttrNode  *array_attr;
    RecordAttrNode *record_attr;
    friend class TypeAttrNode;
    friend class ArrayAttrNode;
    friend class RecordAttrNode;

  public:
    StructAttrNode(ArrayAttrNode *a_a) : uid(++global_uid), is_array(true), array_attr(a_a) {}
    StructAttrNode(RecordAttrNode *r_a) : uid(++global_uid), is_array(false), record_attr(r_a) {}

    int getUid() {
        return uid;
    }

    void translateId();

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(StructAttrNode *type);

    std::string gen_viz_code();
};

class SetAttrNode {  // TODO
  private:
    int           uid;
    TypeAttrNode *basic_type;

  public:
};

class ArrayAttrNode {
  private:
    int               uid;
    bool              is_ele_type_id;
    TypeAttrListNode *index_type;
    TypeAttrNode     *element_type;
    friend class StructAttrNode;

  public:
    ArrayAttrNode(TypeAttrListNode *it, TypeAttrNode *et);

    int getUid() {
        return uid;
    }

    void translateId();

    int get_dim();

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(ArrayAttrNode *type);

    std::string gen_viz_code() {
        std::string result = vizNode(uid, "ArrayAttrNode");
        result += vizChildEdge(uid, index_type->getUid(), "indices", "Type of Indices ");
        result += index_type->gen_viz_code();
        result += vizChildEdge(uid, element_type->getUid(), "element", "Type of Element");
        if (!is_ele_type_id) result += element_type->gen_viz_code();
        return result;
    }
};

class RecordAttrNode {
  private:
    int             uid;
    VarDefListNode *defs;

  public:
    RecordAttrNode(VarDefListNode *d) : uid(++global_uid), defs(d) {}

    int getUid() {
        return uid;
    }

    void translateId();

    int get_dim();

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(RecordAttrNode *type);

    std::string gen_viz_code();
};

class PtrAttrNode {
  private:
    int            uid;
    bool           is_basic_type;
    BasicAttrNode *basic_attr;
    std::string    name;

  public:
    PtrAttrNode(BasicAttrNode *b_a)
            : uid(++global_uid), is_basic_type(true), basic_attr(b_a), name("") {}
    PtrAttrNode(std::string id)
            : uid(++global_uid), is_basic_type(false), basic_attr(nullptr), name(id) {}

    int getUid() {
        return uid;
    }
};

#endif