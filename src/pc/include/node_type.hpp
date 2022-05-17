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
    friend class EnumAttrNode;
    friend class StructAttrNode;
    friend class ArrayAttrNode;

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
        if (!use_struct)
            return (this == type);  // TODO dangerous! act pointer comparison
        else
            switch (this->root_type) {
                case basic: return basic_attr->is_type_equ(type);
                case ordinal: return ord_attr->is_type_equ(type);
                case structured: return struct_attr->is_type_equ(type);
                default: return false;
            }
    }
};

class BasicAttrNode {
  private:
    basic_type_kind type;

  public:
    BasicAttrNode(basic_type_kind t) : type(t) {}

    basic_type_kind get_type(void) {
        return this->type;
    }

    int get_length(void) {
        switch (this->type) {
            case boolean: return BASIC_BOOL_LEN;
            case integer: return BASIC_INT_LEN2;
            case real: return BASIC_REAL_LEN;
            case character: return BASIC_CHAR_LEN;
            case pointer: return BASIC_PTR_LEN;
        }
    }

    int get_offset(void) {
        return 0;
    }

    bool is_type_equ(TypeAttrNode* type) {
        if (type->root_type != basic)
            return false;
        return is_type_equ(type->basic_attr);
    }
    bool is_type_equ(BasicAttrNode* type) {
        return type->type == this->type;
    }
};

class OrdAttrNode {
  private:
    TypeAttrNode*     basic_type;  // TODO when to use this variable?
    bool              is_subrange;
    SubrangeAttrNode* subrange_attr;
    EnumAttrNode*     enum_attr;
    friend class TypeAttrNode;
    friend class SubrangeAttrNode;
    friend class EnumAttrNode;

  public:
    OrdAttrNode(int lower_bound, int upper_bound)
            : is_subrange(true),
              subrange_attr(new SubrangeAttrNode(lower_bound, upper_bound)),
              enum_attr(nullptr) {}
    OrdAttrNode(char lower_bound, char upper_bound)
            : is_subrange(true),
              subrange_attr(new SubrangeAttrNode(lower_bound, upper_bound)),
              enum_attr(nullptr) {}
    OrdAttrNode(std::vector<std::string> ids)
            : is_subrange(false), subrange_attr(nullptr), enum_attr(new EnumAttrNode(ids)) {}
    ~OrdAttrNode() {
        if (subrange_attr != nullptr)
            delete subrange_attr;
        if (enum_attr != nullptr)
            delete enum_attr;
    }

    int get_length() {
        return is_subrange ? subrange_attr->get_length() : enum_attr->get_length();
    }

    int get_offset() {
        // TODO
    }

    bool is_type_equ(TypeAttrNode* type) {
        if (type->root_type != ordinal)
            return false;
        return is_type_equ(type->ord_attr);
    }
    bool is_type_equ(OrdAttrNode* type) {
        if (type->is_subrange && is_subrange)
            return subrange_attr->is_type_equ(type->subrange_attr);
        if (!type->is_subrange && !is_subrange)
            return enum_attr->is_type_equ(type->enum_attr);
        return false;
    }
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

    int get_length(void) {
        return 2 * BASIC_INT_LEN2 + (is_int_bound ? BASIC_INT_LEN2 : BASIC_CHAR_LEN);
    }

    bool is_type_equ(TypeAttrNode* type) {
        if (type->root_type != ordinal)
            return false;
        return is_type_equ(type->ord_attr);
    }
    bool is_type_equ(OrdAttrNode* type) {
        if (!type->is_subrange)
            return false;
        return is_type_equ(type->subrange_attr);
    }
    bool is_type_equ(SubrangeAttrNode* type) {
        if (type->is_int_bound != is_int_bound)
            return false;
        return type->bounds[0] == bounds[0] && type->bounds[1] == bounds[1];
    }
};

class EnumAttrNode {
  private:
    std::vector<std::string> identifiers;  // enum translates identifier to integer
    friend class OrdAttrNode;

  public:
    EnumAttrNode(std::vector<std::string> ids) : identifiers(ids) {}

    void addItem(std::string id) {
        identifiers.push_back(id);
    }

    int get_length(void) {
        return BASIC_INT_LEN2 * identifiers.size();
    }

    int get_offset() {
        // TODO
    }

    bool is_type_equ(TypeAttrNode* type) {
        if (type->root_type != ordinal)
            return false;
        return is_type_equ(type->ord_attr);
    }
    bool is_type_equ(OrdAttrNode* type) {
        if (type->is_subrange)
            return false;
        return is_type_equ(type->enum_attr);
    }
    bool is_type_equ(EnumAttrNode* type) {
        if (type->identifiers.size() != identifiers.size())
            return false;
        for (int i = 0; i < identifiers.size(); i++)
            if (type->identifiers.at(i) != identifiers.at(i))
                return false;
        return true;
    }
};

class StructAttrNode {
  private:
    struct_type_kind type;
    ArrayAttrNode*   array_attr;
    friend class TypeAttrNode;
    friend class ArrayAttrNode;

  public:
    int  get_length() {}
    bool is_type_equ(TypeAttrNode* type) {}
};

class SetAttrNode {  // TODO
  private:
  public:
};

class ArrayAttrNode {
  private:
    std::vector<TypeAttrNode*> index_type;
    TypeAttrNode*              element_type;

  public:
    ArrayAttrNode(std::vector<TypeAttrNode*> it, TypeAttrNode* et)
            : index_type(it), element_type(et) {}

    int getDim() {
        return index_type.size();
    }

    int get_length(void) {
        // TODO
    }

    int get_offset() {
        // TODO
    }

    bool is_type_equ(TypeAttrNode* type) {
        if (type->root_type != structured)
            return false;
        return is_type_equ(type->struct_attr);
    }
    bool is_type_equ(StructAttrNode* type) {
        if (type->type != array)
            return false;
        return is_type_equ(type->array_attr);
    }
    bool is_type_equ(ArrayAttrNode* type) {
        if (getDim() != type->getDim())
            return false;
        if (!element_type->is_type_equ(type->element_type, true))
            return false;
        for (int i = 0; i < index_type.size(); i++)
            if (!index_type.at(i)->is_type_equ(type->index_type.at(i), true))
                return false;
        return true;
    }
};

class RecordAttrNode {
  private:
  public:
};

#endif