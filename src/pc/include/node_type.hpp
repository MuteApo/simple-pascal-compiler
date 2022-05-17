#ifndef _NODE_TYPE_H_
#define _NODE_TYPE_H_

#include <string>
#include <vector>

#define BASIC_BOOL_LEN 1
#define BASIC_INT_LEN2 2
#define BASIC_REAL_LEN 4
#define BASIC_CHAR_LEN 1
#define BASIC_PTR_LEN 4

typedef enum {basic, ordinal, structured, pointer, type_identifier} type_kind; 
typedef enum {boolean, integer, real, character} basic_type_kind;
typedef enum {subrange, enumerate} ord_type_kind;
typedef enum {array, record} struct_type_kind;

class TypeDefNode{
private:

public:

};

class TypeAttrNode{
private:
    type_kind root_type;
    std::string type_identifier;
    BasicAttrNode *basic_attr;
    OrdAttrNode *ord_attr;
    StructAttrNode *struct_attr;
    friend class BasicAttrNode;
    friend class OrdAttrNode;
    friend class StructAttrNode;
public:
    TypeAttrNode();
    TypeAttrNode();
    TypeAttrNode();
    TypeAttrNode();
    TypeAttrNode();
    int get_length(void){
        if(this->root_type == basic)
            return this->basic_attr->get_length();
        else if(this->root_type == ordinal)
            return this->ord_attr->get_length();
        else if(this->root_type == structured)
            return this->struct_attr->get_length();
        else return -1;
    }
    // -1: Fail to get the offset
    int get_offset(void){ // For basic type (RealType)
        if(this->root_type != basic) return -1;
        else return 0;
    }
    int get_offset(std::vector<int> indexs){ // For array type
        if(this->root_type != structured) return -1;
        else if(this->struct_attr->type != array) return -1;

    }
    int get_offset(std::string member = ""){
        if(this->root_type == basic) return 0;
        else if(this->root_type == ordinal)
            return ord_attr->get_offset();
    }
    bool is_type_equ(TypeAttrNode *type ,bool use_struct = false){
        if(!use_struct){
            return (this == type);
        }else{
            if(this->root_type == basic)
                return basic_attr->is_type_equ(type);
            else if(this->root_type == ordinal)
                return ord_attr->is_type_equ(type);
            else if(this->root_type == structured)
                return struct_attr->is_type_equ(type);
            else return false;
        }
    }
};

class StructAttrNode{
private:
    struct_type_kind type;
    friend class TypeAttrNode;
public:
    int get_length(){
        
    }
    bool is_type_equ(TypeAttrNode *type){
        
    }
};

class SetAttrNode{
private:
    
public:

};

class ArrayAttrNode{
private:
    int dim_num;
    std::vector<TypeAttrNode*> index_type;
    TypeAttrNode* element_type;
public:

};

class RecordAttrNode{
private:

public:

};

class OrdAttrNode{
private:
    TypeAttrNode *basic_type;
    bool is_subrange;
    friend class TypeAttrNode;
    
    
public:
    OrdAttrNode(std::string name, int lower_bound, int upper_bound);
    OrdAttrNode(std::string name, char lower_bound, char upper_bound);
    OrdAttrNode();
    int get_length(){

    }
    int get_offset(){

    }
    bool is_type_equ(TypeAttrNode *type){

    }
};

class SubrangeAttrNode{
private:
    bool is_int_bound;
    int bounds[2] = {0, 0}; // Integer or Char
    friend class Type;
private:
    SubrangeAttrNode(int lower_bound, int upper_bound){
        bounds[0] = lower_bound;
        bounds[1] = upper_bound;
        is_int_bound = 1;
    }
    SubrangeAttrNode(char lower_bound, char upper_bound){
        bounds[0] = (int)lower_bound;
        bounds[1] = (int)upper_bound;
        is_int_bound = 0;
    }
    int get_length(void){
        if(is_int_bound) return 2 * 2 + 2;
        else return 2 * 2 + 1;
    }
    bool is_type_equ(TypeAttrNode type){
        
    }
};


class EnumAttrNode{
private:
    friend class Type;
public:

};


class BasicAttrNode{
private:
    basic_type_kind type;
    friend class TypeAttrNode;
public:
    BasicAttrNode(basic_type_kind type):type(type){}
    basic_type_kind get_type(void){return this->type;}
    int get_length(void){
        if(this->type == boolean) return BASIC_BOOL_LEN;
        else if(this->type == integer) return BASIC_INT_LEN2;
        else if(this->type == real) return BASIC_REAL_LEN;
        else if(this->type == character) return BASIC_CHAR_LEN;
        else if(this->type == pointer) return BASIC_PTR_LEN;
    }
    int get_offset(void){return 0;}
    bool is_type_equ(TypeAttrNode *type){
        if(type->root_type != basic) return false;
        else if(type->basic_attr->type != this->type)
            return false;
        return true;
    }
};

#endif