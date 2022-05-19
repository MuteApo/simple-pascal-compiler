#include "include/node_type.hpp"
#include "include/symbol_table.hpp"

int TypeDefNode::gen_sym_tab(void) {
    return symbol_table.addSymbol(this->identifier, this->type);
    // TODO: insert all enum identifier to const symtbl
}

int TypeAttrNode::get_length(void) {
    switch (this->root_type) {
        case basic: return this->basic_attr->get_length();
        case ordinal: return this->ord_attr->get_length();
        case structured: return this->struct_attr->get_length();
        case pointer: return BASIC_PTR_LEN;
        case type_identifier: {
            TypeAttrNode *lut_this = symbol_table.findTypeSymbol(this->type_id);
            if (lut_this == nullptr)
                return -1;  // Type identifer not found in all scope
            else
                return lut_this->get_length();
        }
    }
    return -1;
}

int TypeAttrNode::get_offset(void) {
    return -1;  // TODO
}
int TypeAttrNode::get_offset(std::vector<int> static_indexs) {
    return -1;  // TODO
}
int TypeAttrNode::get_offset(std::string member) {
    return -1;  // TODO
}

bool TypeAttrNode::is_type_equ(TypeAttrNode *type, bool use_struct) {
    if (!use_struct) return this == type;
    if (type->root_type != this->root_type) return false;
    switch (this->root_type) {
        case basic: return this->basic_attr->is_type_equ(type);
        case ordinal: return this->ord_attr->is_type_equ(type);
        case structured: return this->struct_attr->is_type_equ(type);
        case pointer: return type->root_type == pointer;
        case type_identifier: {
            TypeAttrNode *lut_this = symbol_table.findTypeSymbol(this->type_id);
            TypeAttrNode *lut_type = symbol_table.findTypeSymbol(type->type_id);
            return lut_this->is_type_equ(lut_type);
        }
    }
    return false;
}

basic_type_kind BasicAttrNode::get_type(void) {
    return this->type;
}

int BasicAttrNode::get_length(void) {
    switch (this->type) {
        case boolean: return BASIC_BOOL_LEN;
        case integer: return BASIC_INT_LEN;
        case real: return BASIC_REAL_LEN;
        case character: return BASIC_CHAR_LEN;
        case pointer: return BASIC_PTR_LEN;
    }
    return -1;
}

int BasicAttrNode::get_offset(void) {
    // TODO
    return -1;
}

bool BasicAttrNode::is_type_equ(TypeAttrNode *type) {
    if (type->root_type != basic) return false;
    return this->is_type_equ(type->basic_attr);
}
bool BasicAttrNode::is_type_equ(BasicAttrNode *type) {
    return type->type == this->type;
}

int OrdAttrNode::get_length(void) {
    return this->is_subrange ? this->subrange_attr->get_length() : this->enum_attr->get_length();
}

int OrdAttrNode::get_size(void) {
    return this->is_subrange ? this->subrange_attr->get_size() : this->enum_attr->get_size();
}

int OrdAttrNode::get_offset(void) {
    return -1;  // TODO
}

bool OrdAttrNode::is_type_equ(TypeAttrNode *type) {
    if (type->root_type != ordinal) return false;
    return this->is_type_equ(type->ord_attr);
}
bool OrdAttrNode::is_type_equ(OrdAttrNode *type) {
    if (type->is_subrange && this->is_subrange)
        return this->subrange_attr->is_type_equ(type->subrange_attr);
    if (!type->is_subrange && !this->is_subrange)
        return this->enum_attr->is_type_equ(type->enum_attr);
    return false;
}

int SubrangeAttrNode::get_length(void) {
    return -1;  // TODO
}

int SubrangeAttrNode::get_size(void) {
    return -1;  // TODO
}

int SubrangeAttrNode::get_offset(void) {
    return -1;  // TODO
}

bool SubrangeAttrNode::is_type_equ(TypeAttrNode *type) {
    if (type->root_type != ordinal) return false;
    return this->is_type_equ(type->ord_attr);
}
bool SubrangeAttrNode::is_type_equ(OrdAttrNode *type) {
    if (!type->is_subrange) return false;
    return this->is_type_equ(type->subrange_attr);
}
bool SubrangeAttrNode::is_type_equ(SubrangeAttrNode *type) {
    return false;  // TODO
}

int EnumAttrNode::get_length(void) {
    return BASIC_INT_LEN;
}

int EnumAttrNode::get_size(void) {
    return this->items.size();
}

int EnumAttrNode::get_offset(void) {
    return 0;
}

bool EnumAttrNode::is_type_equ(TypeAttrNode *type) {
    if (type->root_type != ordinal) return false;
    return this->is_type_equ(type->ord_attr);
}
bool EnumAttrNode::is_type_equ(OrdAttrNode *type) {
    if (type->is_subrange) return false;
    return this->is_type_equ(type->enum_attr);
}
bool EnumAttrNode::is_type_equ(EnumAttrNode *type) {
    return false;  // TODO
}

int StructAttrNode::get_length(void) {
    switch (this->type) {
        case array: return this->array_attr->get_length();
        case record: return this->record_attr->get_length();
    }
    return -1;
}

int StructAttrNode::get_offset(void) {
    return -1;  // TODO
}

bool StructAttrNode::is_type_equ(TypeAttrNode *type) {
    switch (this->type) {
        case array: return this->array_attr->is_type_equ(type);
        case record: return this->record_attr->is_type_equ(type);
    }
    return false;
}

int ArrayAttrNode::get_dim() {
    return this->index_type.size();
}

int ArrayAttrNode::get_length(void) {
    int result = this->element_type->get_length();
    for (TypeAttrNode *type : this->index_type) {
        if (type->root_type != ordinal) return -1;
        result *= type->ord_attr->get_size();
    }
    return result;
}

int ArrayAttrNode::get_offset(void) {
    return -1;  // TODO
}

bool ArrayAttrNode::is_type_equ(TypeAttrNode *type) {
    if (type->root_type != structured) return false;
    return this->is_type_equ(type->struct_attr);
}
bool ArrayAttrNode::is_type_equ(StructAttrNode *type) {
    if (type->type != array) return false;
    return this->is_type_equ(type->array_attr);
}
bool ArrayAttrNode::is_type_equ(ArrayAttrNode *type) {
    if (type->get_dim() != this->get_dim()) return false;
    if (!this->element_type->is_type_equ(type->element_type)) return false;
    for (int i = 0; i < this->index_type.size(); i++)
        if (!this->index_type.at(i)->is_type_equ(type->index_type.at(i))) return false;
    return true;
}

int RecordAttrNode::get_dim() {
    return -1;  // TODO
}

int RecordAttrNode::get_length(void) {
    return -1;  // TODO
}

int RecordAttrNode::get_offset(void) {
    return -1;  // TODO
}

bool RecordAttrNode::is_type_equ(TypeAttrNode *type) {
    if (type->root_type != structured) return false;
    return this->is_type_equ(type->struct_attr);
}
bool RecordAttrNode::is_type_equ(StructAttrNode *type) {
    if (type->type != record) return false;
    return this->is_type_equ(type->record_attr);
}
bool RecordAttrNode::is_type_equ(RecordAttrNode *type) {
    return false;  // TODO
}