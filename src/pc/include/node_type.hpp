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
#define ALIGN_LEN 4

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

    int getUid();

    bool gen_sym_tab();

    std::string gen_viz_code(int run);
};

class TypeDefListNode {
  private:
    int                        uid;
    std::vector<TypeDefNode *> type_defs;

  public:
    TypeDefListNode();

    int getUid();

    void addTypeDef(TypeDefNode *type_def);

    std::string gen_viz_code(int run);

    bool gen_sym_tab();
};

class TypeAttrNode {
  private:
    int             uid;
    type_kind       root_type;
    std::string     name;
    BasicAttrNode  *basic_attr;
    OrdAttrNode    *ord_attr;
    StructAttrNode *struct_attr;
    PtrAttrNode    *ptr_attr;
    friend class BasicAttrNode;
    friend class OrdAttrNode;
    friend class SubrangeAttrNode;
    friend class EnumAttrNode;
    friend class StructAttrNode;
    friend class ArrayAttrNode;
    friend class RecordAttrNode;
    friend class PtrAttrNode;

  public:
    TypeAttrNode(type_kind       type,
                 std::string     id,
                 BasicAttrNode  *b_attr = nullptr,
                 OrdAttrNode    *o_attr = nullptr,
                 StructAttrNode *s_attr = nullptr,
                 PtrAttrNode    *p_attr = nullptr);
    TypeAttrNode(std::string id);
    TypeAttrNode(BasicAttrNode *attr_node);
    TypeAttrNode(OrdAttrNode *attr_node);
    TypeAttrNode(StructAttrNode *attr_node);
    TypeAttrNode(PtrAttrNode *attr_node);

    int getUid();

    std::string getName();

    type_kind getType();

    int getLength();

    int getOffset(std::string member = "");

    int getSize();

    std::string getNodeInfo();

    std::string gen_viz_code(int run);

    void translateId();

    bool gen_sym_tab();

    bool is_type_equ(TypeAttrNode *type, bool use_struct = true);

    std::string gen_asm_def() {
        // TODO
        return "";
    }
};

class TypeAttrListNode {
  private:
    int                         uid;
    std::vector<bool>           is_type_id;
    std::vector<TypeAttrNode *> type_attrs;

  public:
    TypeAttrListNode();

    int getUid();

    int getDim();

    std::vector<TypeAttrNode *> getAttrList();

    int getSize();

    void addTypeAttr(TypeAttrNode *type_attr);

    std::string gen_viz_code(int run);

    void translateId();

    bool is_type_equ(TypeAttrListNode *type);
};

class BasicAttrNode {
  private:
    int             uid;
    basic_type_kind type;
    friend class LiteralNode;

  public:
    BasicAttrNode(basic_type_kind t);

    int getUid();

    basic_type_kind getType();

    int getLength();

    int getOffset();

    std::string getNodeInfo();

    std::string gen_viz_code(int run);

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
    OrdAttrNode(SubrangeAttrNode *s_a);
    OrdAttrNode(EnumAttrNode *e_a);

    int getUid();

    int getLength();

    int getOffset();

    int getSize();

    std::string gen_viz_code(int run);

    void translateId();

    bool gen_sym_tab();

    bool is_type_equ(OrdAttrNode *type);
};

class SubrangeAttrNode {
  private:
    int       uid;
    ExprNode *low_bound, *up_bound;  // Integer or Char
    bool      is_low_id, is_up_id;
    friend class OrdAttrNode;

  public:
    SubrangeAttrNode(ExprNode *lb, ExprNode *ub);

    int getUid();

    int getLength();

    int getOffset();

    int getSize();

    std::string gen_viz_code(int run);

    void translateId();

    bool is_type_equ(SubrangeAttrNode *type);
};

class EnumAttrNode {
  private:
    int                     uid;
    std::vector<ExprNode *> items;
    friend class OrdAttrNode;

  public:
    EnumAttrNode(std::vector<ExprNode *> exprs);

    int getUid();

    int getLength();

    int getOffset();

    int getSize();

    std::string gen_viz_code(int run);

    bool gen_sym_tab();

    bool is_type_equ(EnumAttrNode *type);
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
    StructAttrNode(ArrayAttrNode *a_a);
    StructAttrNode(RecordAttrNode *r_a);

    int getUid();

    int getLength();

    int getOffset(std::string member);

    std::string gen_viz_code(int run);

    void translateId();

    bool is_type_equ(StructAttrNode *type);
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

    int getUid();

    int getDim();

    int getLength();

    int getOffset();

    std::string gen_viz_code(int run);

    void translateId();

    bool is_type_equ(ArrayAttrNode *type);
};

class RecordAttrNode {
  private:
    int             uid;
    VarDefListNode *defs;

  public:
    RecordAttrNode(VarDefListNode *d);

    int getUid();

    int getDim();

    int getLength();

    int getOffset(std::string member);

    std::string gen_viz_code(int run);

    void translateId();

    bool is_type_equ(RecordAttrNode *type);
};

class PtrAttrNode {
  private:
    int           uid;
    bool          is_base_id;
    TypeAttrNode *base_attr;

  public:
    PtrAttrNode(TypeAttrNode *e);

    int getUid();

    int getLength();

    int getOffset();

    std::string gen_viz_code(int run);

    void translateId();
};

#endif