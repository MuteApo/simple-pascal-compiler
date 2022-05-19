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

#include "node_expr.hpp"
#include "node_var.hpp"
#include <set>
#include <string>
#include <vector>

#define BASIC_BOOL_LEN 1
#define BASIC_INT_LEN 2
#define BASIC_REAL_LEN 4
#define BASIC_CHAR_LEN 1
#define BASIC_PTR_LEN 4

typedef enum { basic = 101, ordinal, structured, pointer, type_identifier } type_kind;
typedef enum { boolean = 201, integer, real, character } basic_type_kind;
typedef enum { subrange = 301, enumerate } ord_type_kind;
typedef enum { array = 401, record } struct_type_kind;

class TypeDefNode {
  private:
    std::string   identifier;
    TypeAttrNode *type;

  public:
    TypeDefNode(std::string id, TypeAttrNode *t) : identifier(id), type(t) {}

    int gen_sym_tab(void);
};

class TypeDefListNode {
  private:
    std::vector<TypeDefNode *> type_defs;

  public:
    TypeDefListNode() {
        type_defs.clear();
    }

    void addTypeDef(TypeDefNode *type_def) {
        type_defs.push_back(type_def);
    }

    bool gen_sym_tab() {
        bool succeed = true;
        for (TypeDefNode *type : type_defs) succeed &= type->gen_sym_tab();
        return succeed;
    }
};

class TypeAttrNode {
  private:
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
            : root_type(type),
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

    std::string getId() {
        return type_id;
    }

    int get_length(void);

    // -1: Fail to get the offset
    int get_offset(void);                            // For basic type (RealType) or pointer type
    int get_offset(std::vector<int> static_indexs);  // For array type
    int get_offset(std::string member);              // For record type

    bool is_type_equ(TypeAttrNode *type, bool use_struct = true);

    std::string gen_asm_def(void) {
        // TODO
        return "";
    }

    std::string toString() {
        return "";
    }
};

class TypeAttrListNode {
    std::vector<TypeAttrNode *> type_attrs;

  public:
    TypeAttrListNode() {
        type_attrs.clear();
    }

    void addTypeAttr(TypeAttrNode *type_attr) {
        type_attrs.push_back(type_attr);
    }

    std::vector<TypeAttrNode *> getAttrList() {
        return type_attrs;
    }
};

class BasicAttrNode {
  private:
    basic_type_kind type;
    friend class LiteralNode;

  public:
    BasicAttrNode(basic_type_kind t) : type(t) {}

    basic_type_kind get_type(void);

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode *type);
    bool is_type_equ(BasicAttrNode *type);
};

class OrdAttrNode {
  private:
    bool              is_subrange;
    SubrangeAttrNode *subrange_attr;
    EnumAttrNode     *enum_attr;
    friend class TypeAttrNode;
    friend class SubrangeAttrNode;
    friend class EnumAttrNode;
    friend class ArrayAttrNode;

  public:
    OrdAttrNode(SubrangeAttrNode *s_a)
            : is_subrange(true), subrange_attr(s_a), enum_attr(nullptr) {}
    OrdAttrNode(EnumAttrNode *e_a) : is_subrange(false), subrange_attr(nullptr), enum_attr(e_a) {}

    int get_length(void);

    int get_size(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode *type);
    bool is_type_equ(OrdAttrNode *type);
};

class SubrangeAttrNode {
  private:
    // bool      is_int_bound;
    ExprNode *low_bound, *up_bound;  // Integer or Char
    friend class OrdAttrNode;

  public:
    SubrangeAttrNode(ExprNode *lb, ExprNode *ub) : low_bound(lb), up_bound(ub) {}

    int get_length(void);

    int get_size(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode *type);
    bool is_type_equ(OrdAttrNode *type);
    bool is_type_equ(SubrangeAttrNode *type);
};

class EnumAttrNode {
  private:
    std::vector<ExprNode *> items;
    friend class OrdAttrNode;

  public:
    EnumAttrNode(std::vector<ExprNode *> i) : items(i) {}

    int get_length(void);

    int get_size(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode *type);
    bool is_type_equ(OrdAttrNode *type);
    bool is_type_equ(EnumAttrNode *type);
};

class StructAttrNode {
  private:
    struct_type_kind type;
    ArrayAttrNode   *array_attr;
    RecordAttrNode  *record_attr;
    friend class TypeAttrNode;
    friend class ArrayAttrNode;
    friend class RecordAttrNode;

  public:
    StructAttrNode(ArrayAttrNode *a_a) : type(array), array_attr(a_a) {}
    StructAttrNode(RecordAttrNode *r_a) : type(record), record_attr(r_a) {}

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode *type);
};

class SetAttrNode {  // TODO
  private:
    TypeAttrNode *basic_type;

  public:
};

class ArrayAttrNode {
  private:
    std::vector<TypeAttrNode *> index_type;
    TypeAttrNode               *element_type;
    friend class StructAttrNode;

  public:
    ArrayAttrNode(std::vector<TypeAttrNode *> it, TypeAttrNode *et)
            : index_type(it), element_type(et) {}

    int get_dim();

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode *type);
    bool is_type_equ(StructAttrNode *type);
    bool is_type_equ(ArrayAttrNode *type);
};

class RecordAttrNode {
  private:
    std::vector<VarDefNode *> defs;

  public:
    RecordAttrNode(std::vector<VarDefNode *> d) : defs(d) {}

    int get_dim();

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode *type);
    bool is_type_equ(StructAttrNode *type);
    bool is_type_equ(RecordAttrNode *type);
};

#endif