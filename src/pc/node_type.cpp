#include "include/node_type.hpp"
#include "include/symbol_table.hpp"

int TypeDefNode::gen_sym_tab(void) {
    return symbol_table.addSymbol(name, type);
    // TODO: insert all enum identifier to const symtbl
}

std::string TypeDefNode::gen_viz_code() {
    std::string result = vizNode(uid, "TypeDefNode\n" + name);
    result += vizChildEdge(uid, type->getUid());
    result += type->gen_viz_code();
    return result;
}

int TypeAttrNode::get_length(void) {
    switch (root_type) {
        case basic: return basic_attr->get_length();
        case ordinal: return ord_attr->get_length();
        case structured: return struct_attr->get_length();
        case pointer: return BASIC_PTR_LEN;
        case type_identifier: {
            TypeAttrNode *lut_this = symbol_table.findTypeSymbol(type_id);
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
    if (type->root_type != root_type) return false;
    switch (root_type) {
        case basic: return basic_attr->is_type_equ(type);
        case ordinal: return ord_attr->is_type_equ(type);
        case structured: return struct_attr->is_type_equ(type);
        case pointer: return type->root_type == pointer;
        case type_identifier: {
            TypeAttrNode *lut_this = symbol_table.findTypeSymbol(type_id);
            TypeAttrNode *lut_type = symbol_table.findTypeSymbol(type->type_id);
            return lut_this->is_type_equ(lut_type);
        }
    }
    return false;
}

std::string TypeAttrNode::getNodeInfo() {
    std::string result = "TypeAttrNode\n";
    if (root_type == type_identifier) return result + type_id;
    return result;
}

std::string TypeAttrNode::gen_viz_code() {
    std::string result = vizNode(uid, getNodeInfo());
    switch (root_type) {
        case basic:
            result += vizChildEdge(uid, basic_attr->getUid());
            result += basic_attr->gen_viz_code();
            break;
        case ordinal:
            result += vizChildEdge(uid, ord_attr->getUid());
            result += ord_attr->gen_viz_code();
            break;
        case structured:
            result += vizChildEdge(uid, struct_attr->getUid());
            result += struct_attr->gen_viz_code();
            break;
    }
    return result;
}

basic_type_kind BasicAttrNode::getType() {
    return type;
}

int BasicAttrNode::get_length(void) {
    switch (type) {
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
    return is_type_equ(type->basic_attr);
}
bool BasicAttrNode::is_type_equ(BasicAttrNode *type) {
    return type->type == this->type;
}

int OrdAttrNode::get_length(void) {
    return is_subrange ? subrange_attr->get_length() : enum_attr->get_length();
}

int OrdAttrNode::get_size(void) {
    return is_subrange ? subrange_attr->get_size() : enum_attr->get_size();
}

int OrdAttrNode::get_offset(void) {
    return -1;  // TODO
}

bool OrdAttrNode::is_type_equ(TypeAttrNode *type) {
    if (type->root_type != ordinal) return false;
    return is_type_equ(type->ord_attr);
}
bool OrdAttrNode::is_type_equ(OrdAttrNode *type) {
    if (type->is_subrange && is_subrange) return subrange_attr->is_type_equ(type->subrange_attr);
    if (!type->is_subrange && !is_subrange) return enum_attr->is_type_equ(type->enum_attr);
    return false;
}

std::string OrdAttrNode::gen_viz_code() {
    std::string result = vizNode(uid, "OrdAttrNode");
    if (is_subrange) {
        result += vizChildEdge(uid, subrange_attr->getUid());
        result += subrange_attr->gen_viz_code();
    } else {
        result += vizChildEdge(uid, enum_attr->getUid());
        result += enum_attr->gen_viz_code();
    }
    return result;
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
    return is_type_equ(type->ord_attr);
}
bool SubrangeAttrNode::is_type_equ(OrdAttrNode *type) {
    if (!type->is_subrange) return false;
    return is_type_equ(type->subrange_attr);
}
bool SubrangeAttrNode::is_type_equ(SubrangeAttrNode *type) {
    return false;  // TODO
}

std::string SubrangeAttrNode::gen_viz_code() {
    std::string result = vizNode(uid, "SubrangeAttrNode");
    result += vizChildEdge(uid, low_bound->getUid());
    result += low_bound->gen_viz_code();
    result += vizChildEdge(uid, up_bound->getUid());
    result += up_bound->gen_viz_code();
    return result;
}

int EnumAttrNode::get_length(void) {
    return BASIC_INT_LEN;
}

int EnumAttrNode::get_size(void) {
    return items.size();
}

int EnumAttrNode::get_offset(void) {
    return 0;
}

bool EnumAttrNode::is_type_equ(TypeAttrNode *type) {
    if (type->root_type != ordinal) return false;
    return is_type_equ(type->ord_attr);
}
bool EnumAttrNode::is_type_equ(OrdAttrNode *type) {
    if (type->is_subrange) return false;
    return is_type_equ(type->enum_attr);
}
bool EnumAttrNode::is_type_equ(EnumAttrNode *type) {
    return false;  // TODO
}

std::string EnumAttrNode::gen_viz_code() {
    std::string result = vizNode(uid, "EnumAttrNode");
    for (ExprNode *expr : items) {
        result += vizChildEdge(uid, expr->getUid());
        result += expr->gen_viz_code();
    }
    return result;
}

int StructAttrNode::get_length(void) {
    return is_array ? array_attr->get_length() : record_attr->get_length();
}

int StructAttrNode::get_offset(void) {
    return -1;  // TODO
}

bool StructAttrNode::is_type_equ(TypeAttrNode *type) {
    return is_array ? array_attr->is_type_equ(type) : record_attr->is_type_equ(type);
}

std::string StructAttrNode::gen_viz_code() {
    std::string result = vizNode(uid, "StructAttrNode");
    if (is_array) {
        result += vizChildEdge(uid, array_attr->getUid());
        result += array_attr->gen_viz_code();
    } else {
        result += vizChildEdge(uid, record_attr->getUid());
        result += record_attr->gen_viz_code();
    }
    return result;
}

int ArrayAttrNode::get_dim() {
    return index_type->getDim();
}

int ArrayAttrNode::get_length(void) {
    return -1;  // TODO
}

int ArrayAttrNode::get_offset(void) {
    return -1;  // TODO
}

bool ArrayAttrNode::is_type_equ(TypeAttrNode *type) {
    if (type->root_type != structured) return false;
    return is_type_equ(type->struct_attr);
}
bool ArrayAttrNode::is_type_equ(StructAttrNode *type) {
    if (!type->is_array) return false;
    return is_type_equ(type->array_attr);
}
bool ArrayAttrNode::is_type_equ(ArrayAttrNode *type) {
    return false;  // TODO
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
    return is_type_equ(type->struct_attr);
}
bool RecordAttrNode::is_type_equ(StructAttrNode *type) {
    if (type->is_array) return false;
    return is_type_equ(type->record_attr);
}
bool RecordAttrNode::is_type_equ(RecordAttrNode *type) {
    return false;  // TODO
}

std::string RecordAttrNode::gen_viz_code() {
    std::string result = vizNode(uid, "RecordAttrNode");
    result += vizChildEdge(uid, defs->getUid());
    result += defs->gen_viz_code();
    return result;
}