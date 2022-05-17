#ifndef _NODE_TYPE_H_
#define _NODE_TYPE_H_

#include <set>
#include <string>
#include <vector>

#define BASIC_BOOL_LEN 1
#define BASIC_INT_LEN2 2
#define BASIC_REAL_LEN 4
#define BASIC_CHAR_LEN 1
#define BASIC_PTR_LEN 4

typedef enum { basic = 101, ordinal, structured, pointer, type_identifier } type_kind;
typedef enum { boolean = 201, integer, real, character } basic_type_kind;
typedef enum { subrange = 301, enumerate } ord_type_kind;
typedef enum { array = 401, record } struct_type_kind;

class TypeAttrNode;
class BasicAttrNode;
class OrdAttrNode;
class SubrangeAttrNode;
class EnumAttrNode;
class StructAttrNode;
class SetAttrNode;
class ArrayAttrNode;
class RecordAttrNode;

class TypeDefNode {
  private:
    std::string   identifier;
    TypeAttrNode* type;

  public:
    TypeDefNode(std::string id, TypeAttrNode* t);
};

class TypeAttrNode {
  private:
    type_kind       root_type;
    std::string     type_id;
    BasicAttrNode*  basic_attr;
    OrdAttrNode*    ord_attr;
    StructAttrNode* struct_attr;
    friend class BasicAttrNode;
    friend class OrdAttrNode;
    friend class SubrangeAttrNode;
    friend class EnumAttrNode;
    friend class StructAttrNode;
    friend class ArrayAttrNode;
    friend class RecordAttrNode;

  public:
    // TODO ctor & dtor

    int get_length(void);

    // -1: Fail to get the offset
    int get_offset(void);                     // For basic type (RealType) or pointer
    int get_offset(std::vector<int> indexs);  // For array type
    int get_offset(std::string member = "");

    bool is_type_equ(TypeAttrNode* type, bool use_struct = true);
};

class BasicAttrNode {
  private:
    basic_type_kind type;

  public:
    BasicAttrNode(basic_type_kind t) : type(t) {}

    basic_type_kind get_type(void);

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode* type);
    bool is_type_equ(BasicAttrNode* type);
};

class OrdAttrNode {
  private:
    bool              is_subrange;
    SubrangeAttrNode* subrange_attr;
    EnumAttrNode*     enum_attr;
    friend class TypeAttrNode;
    friend class SubrangeAttrNode;
    friend class EnumAttrNode;
    friend class ArrayAttrNode;

  public:
    OrdAttrNode(int lower_bound, int upper_bound);
    OrdAttrNode(char lower_bound, char upper_bound);
    OrdAttrNode(std::vector<std::string> ids);
    ~OrdAttrNode();

    int get_length(void);

    int get_size(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode* type);
    bool is_type_equ(OrdAttrNode* type);
};

class SubrangeAttrNode {
  private:
    bool is_int_bound;
    int  bounds[2] = {0};  // Integer or Char
    friend class OrdAttrNode;

  private:
    SubrangeAttrNode(int lower_bound, int upper_bound) : is_int_bound(true) {
        bounds[0] = lower_bound, bounds[1] = upper_bound;
    }
    SubrangeAttrNode(char lower_bound, char upper_bound) : is_int_bound(false) {
        bounds[0] = lower_bound, bounds[1] = upper_bound;
    }

    int get_length(void);

    int get_size(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode* type);
    bool is_type_equ(OrdAttrNode* type);
    bool is_type_equ(SubrangeAttrNode* type);
};

class EnumAttrNode {
  private:
    std::vector<std::string> identifiers;  // enum translates identifier to integer
    friend class OrdAttrNode;

  public:
    EnumAttrNode(std::vector<std::string> ids) : identifiers(ids) {}

    void addItem(std::string id);

    int get_length(void);

    int get_size(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode* type);
    bool is_type_equ(OrdAttrNode* type);
    bool is_type_equ(EnumAttrNode* type);
};

class StructAttrNode {
  private:
    struct_type_kind type;
    ArrayAttrNode*   array_attr;
    RecordAttrNode*  record_attr;
    friend class TypeAttrNode;
    friend class ArrayAttrNode;
    friend class RecordAttrNode;

  public:
    StructAttrNode(std::vector<TypeAttrNode*> it, TypeAttrNode* et);
    ~StructAttrNode();

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode* type);
};

class SetAttrNode {  // TODO
  private:
    TypeAttrNode* basic_type;

  public:
};

class ArrayAttrNode {
  private:
    std::vector<TypeAttrNode*> index_type;
    TypeAttrNode*              element_type;
    friend class StructAttrNode;

  public:
    ArrayAttrNode(std::vector<TypeAttrNode*> it, TypeAttrNode* et)
            : index_type(it), element_type(et) {}

    int get_dim();

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode* type);
    bool is_type_equ(StructAttrNode* type);
    bool is_type_equ(ArrayAttrNode* type);
};

class RecordAttrNode {  // TODO how to design it
  private:
    std::vector<std::string>   identifiers;
    std::vector<TypeAttrNode*> types;

  public:
    RecordAttrNode(std::vector<std::string> ids, std::vector<TypeAttrNode*> ts)
            : identifiers(ids), types(ts) {}

    int get_dim();

    int get_length(void);

    int get_offset(void);

    bool is_type_equ(TypeAttrNode* type);
    bool is_type_equ(StructAttrNode* type);
    bool is_type_equ(RecordAttrNode* type);
};

#endif