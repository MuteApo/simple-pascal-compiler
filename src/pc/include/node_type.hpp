#ifndef _NODE_TYPE_H_
#define _NODE_TYPE_H_

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

class TypeDefNode {
  private:
  public:
};

class TypeAttrNode {
  private:
    type_kind       root_type;
    std::string     type_identifier;
    BasicAttrNode*  basic_attr;
    OrdAttrNode*    ord_attr;
    StructAttrNode* struct_attr;
    friend class BasicAttrNode;
    friend class OrdAttrNode;
    friend class SubrangeAttrNode;

  public:
    TypeAttrNode();
    TypeAttrNode();
    TypeAttrNode();
    TypeAttrNode();
    TypeAttrNode();
    int get_length(void) {
        if (this->root_type == basic)
            return this->basic_attr->get_length();
        else if (this->root_type == ordinal)
            return this->ord_attr->get_length();
        else if (this->root_type == structured)
            return this->struct_attr->get_length();
        else
            return -1;
    }
    // -1: Fail to get the offset
    int get_offset(void) {  // For basic type (RealType)
        if (this->root_type != basic)
            return -1;
        else
            return 0;
    }
    int get_offset(std::vector<int> indexs) {  // For array type
        if (this->root_type != structured)
            return -1;
        else if (this->struct_attr->type != array)
            return -1;
    }
    int get_offset(std::string member = "") {
        if (this->root_type == basic)
            return 0;
        else if (this->root_type == ordinal)
            return ord_attr->get_offset();
    }
    bool is_type_equ(TypeAttrNode* type, bool use_struct = false) {
        if (!use_struct) {
            return (this == type);
        } else {
            if (this->root_type == basic)
                return basic_attr->is_type_equ(type);
            else if (this->root_type == ordinal)
                return ord_attr->is_type_equ(type);
            else if (this->root_type == structured)
                return struct_attr->is_type_equ(type);
            else
                return false;
        }
    }
};

class BasicAttrNode {
  private:
    basic_type_kind type;

  public:
    BasicAttrNode(basic_type_kind t) : type(t) {}

    basic_type_kind get_type(void) { return this->type; }

    int get_length(void) {
        switch (this->type) {
            case boolean: return BASIC_BOOL_LEN;
            case integer: return BASIC_INT_LEN2;
            case real: return BASIC_REAL_LEN;
            case character: return BASIC_CHAR_LEN;
            case pointer: return BASIC_PTR_LEN;
        }
    }
    int get_offset(void) { return 0; }

    bool is_type_equ(TypeAttrNode* type) {
        if (type->root_type != basic) return false;
        return type->basic_attr->type == this->type;
    }
};

class OrdAttrNode {
  private:
    TypeAttrNode*     basic_type;
    bool              is_subrange;
    SubrangeAttrNode* subrange_attr;
    friend class SubrangeAttrNode;

  public:
    OrdAttrNode(std::string name, int lower_bound, int upper_bound);
    OrdAttrNode(std::string name, char lower_bound, char upper_bound);
    OrdAttrNode();
    int  get_length() {}
    int  get_offset() {}
    bool is_type_equ(TypeAttrNode* type) {}
};

class SubrangeAttrNode {
  private:
    bool is_int_bound;
    int  bounds[2] = {0};  // Integer or Char

  private:
    SubrangeAttrNode(int lower_bound, int upper_bound) : is_int_bound(1) {
        bounds[0] = lower_bound, bounds[1] = upper_bound;
    }
    SubrangeAttrNode(char lower_bound, char upper_bound) : is_int_bound(0) {
        bounds[0] = lower_bound, bounds[1] = upper_bound;
    }

    int get_length(void) { return is_int_bound ? 2 * 2 + 2 : 2 * 2 + 1; }

    bool is_type_equ(TypeAttrNode* type) {
        if (type->root_type != ordinal) return false;
        return is_type_equ(type->ord_attr);
    }
    bool is_type_equ(OrdAttrNode* type) {
        if (!type->is_subrange) return false;
        return is_type_equ(type->subrange_attr);
    }
    bool is_type_equ(SubrangeAttrNode* type) {
        if (type->is_int_bound != this->is_int_bound) return false;
        return type->bounds[0] == this->bounds[0] && type->bounds[1] == this->bounds[1];
    }
};

class EnumAttrNode {
  private:
    friend class Type;

  public:
};

class StructAttrNode {
  private:
    struct_type_kind type;
    friend class TypeAttrNode;

  public:
    int  get_length() {}
    bool is_type_equ(TypeAttrNode* type) {}
};

class SetAttrNode {
  private:
  public:
};

class ArrayAttrNode {
  private:
    int                        dim_num;
    std::vector<TypeAttrNode*> index_type;
    TypeAttrNode*              element_type;

  public:
};

class RecordAttrNode {
  private:
  public:
};

#endif