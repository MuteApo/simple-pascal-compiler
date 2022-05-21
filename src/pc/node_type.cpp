#include "include/node_type.hpp"
#include "include/symbol_table.hpp"

TypeDefNode::TypeDefNode(std::string id, TypeAttrNode *t)
        : uid(++global_uid), name(id), type(t), is_type_id(t->getType() == type_identifier) {}

bool TypeDefNode::gen_sym_tab() {
    type = symbol_table.translateTypeId(type);
    type->translateId();
    return symbol_table.addSymbol(name, type);
}

std::string TypeDefNode::gen_viz_code() {
    std::string result = vizNode(uid, "TypeDefNode\n" + name);
    result += vizChildEdge(uid, type->getUid(), "typedef", "Type Definition");
    if (!is_type_id) result += type->gen_viz_code();
    return result;
}

void TypeAttrNode::translateId() {
    switch (root_type) {
        case basic: break;    // nothing to do
        case pointer: break;  // nothing to do
        case ordinal: ord_attr->translateId(); break;
        case structured: struct_attr->translateId(); break;
        case type_identifier: break;  // done in upper level
    }
}

int TypeAttrNode::get_length(void) {
    switch (root_type) {
        case basic: return basic_attr->get_length();
        case ordinal: return ord_attr->get_length();
        case structured: return struct_attr->get_length();
        case pointer: return BASIC_PTR_LEN;
        case type_identifier: {
            TypeAttrNode *lut_this = symbol_table.findTypeSymbol(name);
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
    if (!use_struct) return name == type->name;
    if (root_type != type->root_type) return false;
    switch (root_type) {
        case basic: return basic_attr->is_type_equ(type->basic_attr);
        case ordinal: return ord_attr->is_type_equ(type->ord_attr);
        case structured: return struct_attr->is_type_equ(type->struct_attr);
        case pointer: return type->root_type == pointer;
        case type_identifier: {
            TypeAttrNode *lut_this = symbol_table.findTypeSymbol(name);
            TypeAttrNode *lut_type = symbol_table.findTypeSymbol(name);
            return lut_this->is_type_equ(lut_type);
        }
    }
    return false;
}

std::string TypeAttrNode::getNodeInfo() {
    std::string result = "TypeAttrNode\n";
    if (root_type == type_identifier) return result + name;
    return result;
}

std::string TypeAttrNode::gen_viz_code() {
    std::string result = vizNode(uid, getNodeInfo());
    switch (root_type) {
        case basic:
            result += vizChildEdge(uid, basic_attr->getUid(), "basic", "Basic Type");
            result += basic_attr->gen_viz_code();
            break;
        case ordinal:
            result += vizChildEdge(uid, ord_attr->getUid(), "ordinal", "Ordinal Type");
            result += ord_attr->gen_viz_code();
            break;
        case structured:
            result += vizChildEdge(uid, struct_attr->getUid(), "struct", "Structured Type");
            result += struct_attr->gen_viz_code();
            break;
        case pointer:
            // TODO: any actions?
            break;
        case type_identifier:
            // TODO: any actions?
            break;
    }
    return result;
}

void TypeAttrListNode::translateId() {
    for (int i = 0; i < type_attrs.size(); i++) {
        type_attrs.at(i) = symbol_table.translateTypeId(type_attrs.at(i));
        type_attrs.at(i)->translateId();
    }
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

bool BasicAttrNode::is_type_equ(BasicAttrNode *type) {
    return type->type == this->type;
}

void OrdAttrNode::translateId() {
    if (is_subrange) subrange_attr->translateId();
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

bool OrdAttrNode::is_type_equ(OrdAttrNode *type) {
    if (is_subrange != type->is_subrange) return false;
    return is_subrange ? subrange_attr->is_type_equ(type->subrange_attr) :
                         enum_attr->is_type_equ(type->enum_attr);
}

std::string OrdAttrNode::gen_viz_code() {
    std::string result = vizNode(uid, "OrdAttrNode");
    if (is_subrange) {
        result += vizChildEdge(uid, subrange_attr->getUid(), "subrange", "Subrange Type");
        result += subrange_attr->gen_viz_code();
    } else {
        result += vizChildEdge(uid, enum_attr->getUid(), "enum", "Enumerated Type");
        result += enum_attr->gen_viz_code();
    }
    return result;
}

SubrangeAttrNode::SubrangeAttrNode(ExprNode *lb, ExprNode *ub)
        : uid(++global_uid),
          low_bound(lb),
          up_bound(ub),
          is_low_id(lb->getExprType() == el_id),
          is_up_id(ub->getExprType() == el_id) {}

void SubrangeAttrNode::translateId() {
    low_bound = symbol_table.translateConstId(low_bound);
    up_bound  = symbol_table.translateConstId(up_bound);
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

bool SubrangeAttrNode::is_type_equ(SubrangeAttrNode *type) {
    if (!low_bound->is_value_equ(type->low_bound)) return false;
    if (!up_bound->is_value_equ(type->up_bound)) return false;
    return true;
}

std::string SubrangeAttrNode::gen_viz_code() {
    std::string result = vizNode(uid, "SubrangeAttrNode");
    result += vizChildEdge(uid, low_bound->getUid(), "low", "Lower Bound");
    if (!is_low_id) result += low_bound->gen_viz_code();
    result += vizChildEdge(uid, up_bound->getUid(), "up", "Upper Bound");
    if (!is_up_id) result += up_bound->gen_viz_code();
    return result;
}

EnumAttrNode::EnumAttrNode(std::vector<ExprNode *> exprs) : uid(++global_uid) {
    items.clear();
    for (ExprNode *expr : exprs) items.push_back(expr->getIdNode()->getName());
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

bool EnumAttrNode::is_type_equ(EnumAttrNode *type) {
    if (items.size() != type->items.size()) return false;
    for (int i = 0; i < items.size(); i++)
        if (items.at(i) != type->items.at(i)) return false;
    return true;
}

std::string EnumAttrNode::getNodeInfo() {
    std::string result = "EnumAttrNode\n(";
    if (items.size()) result += items.at(0);
    for (int i = 1; i < items.size(); i++) result += "," + items.at(i);
    return result + ")";
}

void StructAttrNode::translateId() {
    if (is_array)
        array_attr->translateId();
    else
        record_attr->translateId();
}

int StructAttrNode::get_length(void) {
    return is_array ? array_attr->get_length() : record_attr->get_length();
}

int StructAttrNode::get_offset(void) {
    return -1;  // TODO
}

bool StructAttrNode::is_type_equ(StructAttrNode *type) {
    if (is_array != type->is_array) return false;
    return is_array ? array_attr->is_type_equ(type->array_attr) :
                      record_attr->is_type_equ(type->record_attr);
}

std::string StructAttrNode::gen_viz_code() {
    std::string result = vizNode(uid, "StructAttrNode");
    if (is_array) {
        result += vizChildEdge(uid, array_attr->getUid(), "array", "Array Type");
        result += array_attr->gen_viz_code();
    } else {
        result += vizChildEdge(uid, record_attr->getUid(), "record", "Record Type");
        result += record_attr->gen_viz_code();
    }
    return result;
}

ArrayAttrNode::ArrayAttrNode(TypeAttrListNode *it, TypeAttrNode *et)
        : uid(++global_uid),
          index_type(it),
          element_type(et),
          is_ele_type_id(et->getType() == type_identifier) {}

void ArrayAttrNode::translateId() {
    element_type = symbol_table.translateTypeId(element_type);
    element_type->translateId();
    index_type->translateId();
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

bool ArrayAttrNode::is_type_equ(ArrayAttrNode *type) {
    if (!element_type->is_type_equ(type->element_type)) return false;
    return index_type->is_type_equ(type->index_type);
}

void RecordAttrNode::translateId() {
    defs->translateId();
}

int RecordAttrNode::get_dim() {
    return defs->getDim();
}

int RecordAttrNode::get_length(void) {
    return -1;  // TODO
}

int RecordAttrNode::get_offset(void) {
    return -1;  // TODO
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