#ifndef __NODE_TYPE_H__
#define __NODE_TYPE_H__

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
class StringAttrNode;
class RecordAttrNode;
class PtrAttrNode;

#include "defs.hpp"
#include "node_expr.hpp"
#include "node_var.hpp"
#include <string>
#include <vector>

extern int global_uid;
extern int yylineno;

#define BASIC_BOOL_LEN 1
#define BASIC_INT_LEN 4
#define BASIC_REAL_LEN 4
#define BASIC_CHAR_LEN 1
#define BASIC_PTR_LEN 4
#define ALIGN_LEN 4

class TypeDefNode {
  private:
    int           uid;
    int           line_no;
    bool          is_type_id;
    std::string   name;
    TypeAttrNode *type;

  public:
    TypeDefNode(std::string id, TypeAttrNode *t);

    int getUid();

    bool genSymbolTable();

    std::string genVizCode(int run);
};

class TypeDefListNode {
  private:
    int                        uid;
    int                        line_no;
    std::vector<TypeDefNode *> type_defs;

  public:
    TypeDefListNode();

    int getUid();

    void addTypeDef(TypeDefNode *type_def);

    std::string genVizCode(int run);

    bool genSymbolTable();
};

class TypeAttrNode {
  private:
    int             uid;
    int             line_no;
    TypeKind        root_type;
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
    TypeAttrNode(TypeKind        type,
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

    int getLineNumber();

    std::string getName();

    TypeKind getType();

    std::string getTypeString();

    BasicAttrNode *getBasicAttrNode();

    OrdAttrNode *getOrdAttrNode();

    StructAttrNode *getStructAttr();

    int getLength();

    int getOffset(std::string member = "");

    int getSize(int ord);

    std::string getNodeInfo();

    std::string genVizCode(int run);

    void translateId();

    bool genSymbolTable();

    bool isTypeEqual(TypeAttrNode *type, bool use_struct = true);

    bool isTypeCompatible(TypeAttrNode *type);

    std::string genAsmDef(std::string var_name);
};

class TypeAttrListNode {
  private:
    int                         uid;
    int                         line_no;
    std::vector<bool>           is_type_id;
    std::vector<TypeAttrNode *> type_attrs;

  public:
    TypeAttrListNode();

    int getUid();

    int getDim();

    std::string getTypeString();

    std::vector<TypeAttrNode *> &getAttrList();

    int getSize(int i);

    int getTotalSize();

    void addTypeAttr(TypeAttrNode *type_attr);

    std::string genVizCode(int run);

    void translateId();

    bool isTypeEqual(TypeAttrListNode *type);

    bool testIndexType(ExprListNode *indices);
};

class BasicAttrNode {
  private:
    int           uid;
    int           line_no;
    BasicTypeKind type;
    friend class LiteralNode;

  public:
    BasicAttrNode(BasicTypeKind t);

    int getUid();

    BasicTypeKind getType();

    std::string getTypeString();

    int getLength();

    int getOffset();

    std::string getNodeInfo();

    std::string genVizCode(int run);

    bool isTypeEqual(BasicAttrNode *type);
};

class OrdAttrNode {
  private:
    int               uid;
    int               line_no;
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

    std::string getTypeString();

    int getLength();

    int getOffset();

    int getSize();

    SubrangeAttrNode *getSubrange();

    std::string genVizCode(int run);

    void translateId();

    bool genSymbolTable();

    bool isTypeEqual(OrdAttrNode *type);

    bool isTypeCompatible(TypeAttrNode *type);
};

class SubrangeAttrNode {
  private:
    int       uid;
    int       line_no;
    ExprNode *low_bound, *up_bound;  // Integer or Char
    bool      is_low_id, is_up_id;
    friend class OrdAttrNode;

  public:
    SubrangeAttrNode(ExprNode *lb, ExprNode *ub);

    int getUid();

    std::string getTypeString();

    int getLength();

    int getOffset();

    int getSize();

    ExprNode *getLowerBound();

    std::string genVizCode(int run);

    void translateId();

    bool isTypeEqual(SubrangeAttrNode *type);

    bool isTypeCompatible(TypeAttrNode *type);
};

class EnumAttrNode {
  private:
    int                     uid;
    int                     line_no;
    bool                    is_sym_gen;
    std::vector<ExprNode *> items;
    friend class OrdAttrNode;

  public:
    EnumAttrNode(std::vector<ExprNode *> exprs);

    int getUid();

    std::string getTypeString();

    int getLength();

    int getOffset();

    int getSize();

    std::string genVizCode(int run);

    bool genSymbolTable();

    bool isTypeEqual(EnumAttrNode *type);

    bool isTypeCompatible(TypeAttrNode *type);
};

class StructAttrNode {
  private:
    int             uid;
    int             line_no;
    StructTypeKind  struct_type;
    ArrayAttrNode  *array_attr;
    StringAttrNode *string_attr;
    RecordAttrNode *record_attr;
    friend class TypeAttrNode;
    friend class ArrayAttrNode;
    friend class RecordAttrNode;

  public:
    StructAttrNode(StructTypeKind  st,
                   ArrayAttrNode  *a_a = nullptr,
                   StringAttrNode *s_a = nullptr,
                   RecordAttrNode *r_a = nullptr);
    StructAttrNode(ArrayAttrNode *a_a);
    StructAttrNode(StringAttrNode *s_a);
    StructAttrNode(RecordAttrNode *r_a);

    int getUid();

    StructTypeKind getType();

    std::string getTypeString();

    ArrayAttrNode *getArrayAttr();

    StringAttrNode *getStringAttr();

    RecordAttrNode *getRecordAttr();

    int getLength();

    int getOffset(std::string member);

    std::string genVizCode(int run);

    void translateId();

    bool isTypeEqual(StructAttrNode *type);
};

class SetAttrNode {  // TODO
  private:
    int           uid;
    int           line_no;
    TypeAttrNode *basic_type;

  public:
};

class ArrayAttrNode {
  private:
    int               uid;
    int               line_no;
    bool              is_ele_type_id;
    TypeAttrListNode *index_type;
    TypeAttrNode     *element_type;
    friend class StructAttrNode;

  public:
    ArrayAttrNode(TypeAttrListNode *it, TypeAttrNode *et);

    int getUid();

    int getDim();

    TypeAttrNode *getElementType();

    TypeAttrListNode *getIndexType();

    std::string getTypeString();

    int getLength();

    std::string genVizCode(int run);

    void translateId();

    bool isTypeEqual(ArrayAttrNode *type);

    bool testIndexType(ExprListNode *indices);
};

class StringAttrNode {
  private:
    int       uid;
    int       line_no;
    bool      is_len_id;
    ExprNode *len;

  public:
    StringAttrNode(ExprNode *l);

    int getUid();

    int getLength();

    void setLength(int l);

    std::string genVizCode(int run);

    void translateId();
};

class RecordAttrNode {
  private:
    int             uid;
    int             line_no;
    VarDefListNode *defs;

  public:
    RecordAttrNode(VarDefListNode *d);

    int getUid();

    int getDim();

    std::string getTypeString();

    VarDefNode *getVarDef(std::string id);

    int getLength();

    int getOffset(std::string member);

    std::string genVizCode(int run);

    void translateId();

    bool isTypeEqual(RecordAttrNode *type);
};

class PtrAttrNode {
  private:
    int           uid;
    int           line_no;
    bool          is_base_id;
    TypeAttrNode *base_attr;

  public:
    PtrAttrNode(TypeAttrNode *e);

    int getUid();

    std::string getTypeString();

    int getLength();

    int getOffset();

    std::string genVizCode(int run);

    void translateId();

    bool isTypeEqual(PtrAttrNode *type);
};

#endif