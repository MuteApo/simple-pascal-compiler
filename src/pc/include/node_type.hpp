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
    std::string   name;
    TypeAttrNode *type;

  public:
    TypeDefNode(std::string id, TypeAttrNode *t) : uid(++global_uid), name(id), type(t) {}

    int getUid() {
        return uid;
    }

    int gen_sym_tab(void);

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
        for (TypeDefNode *def : type_defs) {
            result += vizChildEdge(uid, def->getUid());
            result += def->gen_viz_code();
        }
        return result;
    }

    bool gen_sym_tab() {
        bool succeed = true;
        for (TypeDefNode *type : type_defs) succeed &= type->gen_sym_tab();
        return succeed;
    }
};

class TypeAttrNode {
  private:
    int             uid;
    type_kind       root_type;
    std::string     type_id;
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
              type_id(id),
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

    std::string getId() {
        return type_id;
    }

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
    std::vector<TypeAttrNode *> type_attrs;

  public:
    TypeAttrListNode() : uid(++global_uid) {
        type_attrs.clear();
    }

    int getUid() {
        return uid;
    }

    void addTypeAttr(TypeAttrNode *type_attr) {
        type_attrs.push_back(type_attr);
    }

    int getDim() {
        return type_attrs.size();
    }

    std::vector<TypeAttrNode *> getAttrList() {
        return type_attrs;
    }

    std::string gen_viz_code() {
        std::string result = vizNode(uid, "TypeAttrListNode");
        for (TypeAttrNode *def : type_attrs) {
            result += vizChildEdge(uid, def->getUid());
            result += def->gen_viz_code();
        }
        return result;
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

    bool is_type_equ(TypeAttrNode *type);
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

    int get_length(void);

    int get_size(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode *type);
    bool is_type_equ(OrdAttrNode *type);

    std::string gen_viz_code();
};

class SubrangeAttrNode {
  private:
    int uid;
    // bool      is_int_bound;
    ExprNode *low_bound, *up_bound;  // Integer or Char
    friend class OrdAttrNode;

  public:
    SubrangeAttrNode(ExprNode *lb, ExprNode *ub) : uid(++global_uid), low_bound(lb), up_bound(ub) {}

    int getUid() {
        return uid;
    }

    int get_length(void);

    int get_size(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode *type);
    bool is_type_equ(OrdAttrNode *type);
    bool is_type_equ(SubrangeAttrNode *type);

    std::string gen_viz_code();
};

class EnumAttrNode {
  private:
    int                     uid;
    std::vector<ExprNode *> items;
    friend class OrdAttrNode;

  public:
    EnumAttrNode(std::vector<ExprNode *> i) : uid(++global_uid), items(i) {}

    int getUid() {
        return uid;
    }

    int get_length(void);

    int get_size(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode *type);
    bool is_type_equ(OrdAttrNode *type);
    bool is_type_equ(EnumAttrNode *type);

    std::string gen_viz_code();
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

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode *type);

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
    TypeAttrListNode *index_type;
    TypeAttrNode     *element_type;
    friend class StructAttrNode;

  public:
    ArrayAttrNode(TypeAttrListNode *it, TypeAttrNode *et)
            : uid(++global_uid), index_type(it), element_type(et) {}

    int getUid() {
        return uid;
    }

    int get_dim();

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode *type);
    bool is_type_equ(StructAttrNode *type);
    bool is_type_equ(ArrayAttrNode *type);

    std::string gen_viz_code() {
        std::string result = vizNode(uid, "ArrayAttrNode");
        result += vizChildEdge(uid, index_type->getUid());
        result += index_type->gen_viz_code();
        result += vizChildEdge(uid, element_type->getUid());
        result += element_type->gen_viz_code();
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

    int get_dim();

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode *type);
    bool is_type_equ(StructAttrNode *type);
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