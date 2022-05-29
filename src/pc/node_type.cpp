#include "include/node_type.hpp"
#include "include/asmgen.hpp"
#include "include/symbol_table.hpp"

TypeDefNode::TypeDefNode(std::string id, TypeAttrNode *t)
        : uid(++global_uid),
          line_no(yylineno),
          name(id),
          type(t),
          is_type_id(t->getType() == type_identifier) {}

int TypeDefNode::getUid() {
    return uid;
}

bool TypeDefNode::genSymbolTable() {
    try {
        type = symbol_table.translateTypeId(type);
        type->translateId();
        type->genSymbolTable();  // insert (enum, int) pairs into const symbol table
    } catch (UndefineError &e) {
        error_handler.addMsg(e);
    } catch (RedefineError &e) {
        error_handler.addMsg(e);
    }
    if (symbol_table.existSymbol(name)) throw RedefineError(line_no, name);
    return symbol_table.addSymbol(name, type);
}

std::string TypeDefNode::genVizCode(int run) {
    std::string result = vizNode(uid, "TypeDefNode\n" + name);
    result += vizEdge(uid, type->getUid(), "typedef", "Type Definition");
    if (!run || !is_type_id) result += type->genVizCode(run);
    return result;
}

TypeDefListNode::TypeDefListNode() : uid(++global_uid), line_no(yylineno) {
    type_defs.clear();
}

int TypeDefListNode::getUid() {
    return uid;
}

void TypeDefListNode::addTypeDef(TypeDefNode *type_def) {
    type_defs.push_back(type_def);
}

std::string TypeDefListNode::genVizCode(int run) {
    std::string result = vizNode(uid, "TypeDefListNode");
    for (int i = 0; i < type_defs.size(); i++) {
        result += vizEdge(uid,
                          type_defs.at(i)->getUid(),
                          "typedef" + to_string(i + 1),
                          "Type Definition " + to_string(i + 1));
        result += type_defs.at(i)->genVizCode(run);
    }
    return result;
}

bool TypeDefListNode::genSymbolTable() {
    for (TypeDefNode *type : type_defs) try {
            type->genSymbolTable();
        } catch (RedefineError &e) {
            error_handler.addMsg(e);
        }
    return true;
}

TypeAttrNode::TypeAttrNode(TypeKind        type,
                           std::string     id,
                           BasicAttrNode  *b_attr,
                           OrdAttrNode    *o_attr,
                           StructAttrNode *s_attr,
                           PtrAttrNode    *p_attr)
        : uid(++global_uid),
          line_no(yylineno),
          root_type(type),
          name(id),
          basic_attr(b_attr),
          ord_attr(o_attr),
          struct_attr(s_attr),
          ptr_attr(p_attr) {}
TypeAttrNode::TypeAttrNode(std::string id)
        : TypeAttrNode(type_identifier, id, nullptr, nullptr, nullptr, nullptr) {}
TypeAttrNode::TypeAttrNode(BasicAttrNode *attr_node)
        : TypeAttrNode(basic, "", attr_node, nullptr, nullptr, nullptr) {}
TypeAttrNode::TypeAttrNode(OrdAttrNode *attr_node)
        : TypeAttrNode(ordinal, "", nullptr, attr_node, nullptr, nullptr) {}
TypeAttrNode::TypeAttrNode(StructAttrNode *attr_node)
        : TypeAttrNode(structured, "", nullptr, nullptr, attr_node, nullptr) {}
TypeAttrNode::TypeAttrNode(PtrAttrNode *attr_node)
        : TypeAttrNode(pointer, "", nullptr, nullptr, nullptr, attr_node) {}

int TypeAttrNode::getUid() {
    return uid;
}

int TypeAttrNode::getLineNumber() {
    return line_no;
}

std::string TypeAttrNode::getName() {
    return name;
}

TypeKind TypeAttrNode::getType() {
    return root_type;
}

std::string TypeAttrNode::getTypeString() {
    switch (root_type) {
        case basic: return basic_attr->getTypeString();
        case ordinal: return ord_attr->getTypeString();
        case structured: return struct_attr->getTypeString();
        case pointer: return ptr_attr->getTypeString();
        case type_identifier: return getName();
    }
    return "";
}

BasicAttrNode *TypeAttrNode::getBasicAttrNode() {
    return basic_attr;
}

OrdAttrNode *TypeAttrNode::getOrdAttrNode() {
    return ord_attr;
}

StructAttrNode *TypeAttrNode::getStructAttr() {
    return struct_attr;
}

int TypeAttrNode::getLength() {
    switch (root_type) {
        case basic: return basic_attr->getLength();
        case ordinal: return ord_attr->getLength();
        case structured: return struct_attr->getLength();
        case pointer: return ptr_attr->getLength();
        case type_identifier: break;  // identifier replacement should already be done
    }
    return -1;
}

int TypeAttrNode::getOffset(std::string member) {
    switch (root_type) {
        case basic: return basic_attr->getOffset();
        case ordinal: return ord_attr->getOffset();
        case structured: return struct_attr->getOffset(member);
        case pointer: return ptr_attr->getOffset();
        case type_identifier: break;  // identifier replacement should already be done
    }
    return -1;
}

int TypeAttrNode::getSize(int ord) {
    if (root_type != ordinal) throw IndexTypeError(line_no, ord, "ordinal", getTypeString());
    return ord_attr->getSize();
}

std::string TypeAttrNode::getNodeInfo() {
    std::string result = "TypeAttrNode\n";
    if (root_type == type_identifier) result += name;
    return result;
}

std::string TypeAttrNode::genVizCode(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    switch (root_type) {
        case basic:
            result += vizEdge(uid, basic_attr->getUid(), "basic", "Basic Type");
            result += basic_attr->genVizCode(run);
            break;
        case ordinal:
            result += vizEdge(uid, ord_attr->getUid(), "ordinal", "Ordinal Type");
            result += ord_attr->genVizCode(run);
            break;
        case structured:
            result += vizEdge(uid, struct_attr->getUid(), "struct", "Structured Type");
            result += struct_attr->genVizCode(run);
            break;
        case pointer:
            result += vizEdge(uid, ptr_attr->getUid(), "ptr", "Pointer Type");
            result += ptr_attr->genVizCode(run);
            break;
        case type_identifier: break;
    }
    return result;
}

void TypeAttrNode::translateId() {
    try {
        switch (root_type) {
            case basic: break;  // nothing to do
            case ordinal: ord_attr->translateId(); break;
            case structured: struct_attr->translateId(); break;
            case pointer: ptr_attr->translateId(); break;
            case type_identifier: break;  // done in upper level
        }
    } catch (UndefineError &e) {
        throw e;
    }
}

bool TypeAttrNode::genSymbolTable() {
    try {
        if (root_type == ordinal) return ord_attr->genSymbolTable();
    } catch (RedefineError &e) {
        throw e;
    }
    return true;
}

bool TypeAttrNode::isTypeEqual(TypeAttrNode *type, bool use_struct) {
    if (!use_struct) return name == type->name;
    if (root_type != type->root_type) return false;
    switch (root_type) {
        case basic: return basic_attr->isTypeEqual(type->basic_attr);
        case ordinal: return ord_attr->isTypeEqual(type->ord_attr);
        case structured: return struct_attr->isTypeEqual(type->struct_attr);
        case pointer: return ptr_attr->isTypeEqual(type->ptr_attr);
        case type_identifier: {
            TypeAttrNode *lut_this = symbol_table.findTypeSymbol(name);
            TypeAttrNode *lut_type = symbol_table.findTypeSymbol(name);
            if (lut_this == nullptr || lut_type == nullptr) throw UndefineError(line_no, name);
            return lut_this->isTypeEqual(lut_type);
        }
    }
    return false;
}

std::string TypeAttrNode::genAsmDef(std::string var_name) {
    std::string           asm_def = "";
    std::vector<uint8_t>  field_size;
    std::vector<uint32_t> field_rep;
    switch (root_type) {
        case basic: {
            field_size.push_back(basic_attr->getLength());
            field_rep.push_back(1);
            asm_def = get_define_global(var_name, field_size, field_rep);
            break;
        }
        case ordinal: {
            field_size.push_back(ord_attr->getLength());
            field_rep.push_back(1);
            asm_def = get_define_global(var_name, field_size, field_rep);
            break;
        }
        case structured: {
            int length = struct_attr->getLength();
            if (length / (20 * 4) > 0) {
                for (int i = 0; i < length / (20 * 4); i++) {
                    field_size.push_back(4);
                    field_rep.push_back(20);
                }
                length = length % (20 * 4);
            }
            field_size.push_back(4);
            field_rep.push_back(length / 4);
            length = length % 4;
            if (length) {
                field_size.push_back(1);
                field_rep.push_back(length);
            }
            asm_def = get_define_global(var_name, field_size, field_rep);
            break;
        }
        case pointer: {
            field_size.push_back(ptr_attr->getLength());
            field_rep.push_back(1);
            asm_def = get_define_global(var_name, field_size, field_rep);
            break;
        }
    }
    return asm_def;
}

TypeAttrListNode::TypeAttrListNode() : uid(++global_uid), line_no(yylineno) {
    is_type_id.clear();
    type_attrs.clear();
}

int TypeAttrListNode::getUid() {
    return uid;
}

int TypeAttrListNode::getDim() {
    return type_attrs.size();
}

std::string TypeAttrListNode::getTypeString() {
    std::string result = "";
    if (!type_attrs.empty()) result += type_attrs.at(0)->getTypeString();
    for (int i = 1; i <= type_attrs.size(); i++) result += "," + type_attrs.at(i)->getTypeString();
    return result;
}

std::vector<TypeAttrNode *> TypeAttrListNode::getAttrList() {
    return type_attrs;
}

int TypeAttrListNode::getSize(int i) {
    return type_attrs.at(i)->getSize(i + 1);
}

int TypeAttrListNode::getTotalSize() {
    int result = 1;
    for (int i = 0; i < type_attrs.size(); i++) try {
            result *= type_attrs.at(i)->getSize(i);
        } catch (IndexTypeError &e) {
            error_handler.addMsg(e);
        }
    return result;
}

void TypeAttrListNode::addTypeAttr(TypeAttrNode *type_attr) {
    is_type_id.push_back(type_attr->getType() == type_identifier);
    type_attrs.push_back(type_attr);
}

std::string TypeAttrListNode::genVizCode(int run) {
    std::string result = vizNode(uid, "TypeAttrListNode");
    for (int i = 0; i < type_attrs.size(); i++) {
        result += vizEdge(uid,
                          type_attrs.at(i)->getUid(),
                          "index" + to_string(i + 1),
                          "Type of Index " + to_string(i + 1));
        if (!run || !is_type_id.at(i)) result += type_attrs.at(i)->genVizCode(run);
    }
    return result;
}

void TypeAttrListNode::translateId() {
    for (int i = 0; i < type_attrs.size(); i++) try {
            type_attrs.at(i) = symbol_table.translateTypeId(type_attrs.at(i));
            type_attrs.at(i)->translateId();
        } catch (UndefineError &e) {
            throw e;
        }
}

bool TypeAttrListNode::isTypeEqual(TypeAttrListNode *type) {
    if (type_attrs.size() != type->type_attrs.size()) return false;
    for (int i = 0; i < type_attrs.size(); i++)
        if (!type_attrs.at(i)->isTypeEqual(type->type_attrs.at(i))) return false;
    return true;
}

bool TypeAttrListNode::testIndexType(ExprListNode *indices) {
    std::vector<ExprNode *> &index_list = indices->getExprList();
    if (type_attrs.size() != index_list.size())
        throw IndexDimensionError(indices->getLineNumber(), type_attrs.size(), index_list.size());
    for (int i = 0; i < type_attrs.size(); i++)
        if (!type_attrs.at(i)->isTypeEqual(index_list.at(i)->getResultType()))
            throw IndexTypeError(indices->getLineNumber(),
                                 i + 1,
                                 type_attrs.at(i)->getTypeString(),
                                 index_list.at(i)->getResultType()->getTypeString());
    return true;
}

BasicAttrNode::BasicAttrNode(BasicTypeKind t) : uid(++global_uid), line_no(yylineno), type(t) {}

int BasicAttrNode::getUid() {
    return uid;
}

BasicTypeKind BasicAttrNode::getType() {
    return type;
}

std::string BasicAttrNode::getTypeString() {
    switch (type) {
        case boolean: return "boolean";
        case integer: return "integer";
        case real: return "real";
        case character: return "character";
    }
    return "";
}

int BasicAttrNode::getLength() {
    return ALIGN_LEN;
}

int BasicAttrNode::getOffset() {
    return 0;
}

std::string BasicAttrNode::getNodeInfo() {
    return getTypeString();
}

std::string BasicAttrNode::genVizCode(int run) {
    return vizNode(uid, "BasicAttrNode\n" + getNodeInfo());
}

bool BasicAttrNode::isTypeEqual(BasicAttrNode *type) {
    return type->type == this->type;
}

OrdAttrNode::OrdAttrNode(SubrangeAttrNode *s_a)
        : uid(++global_uid),
          line_no(yylineno),
          is_subrange(true),
          subrange_attr(s_a),
          enum_attr(nullptr) {}
OrdAttrNode::OrdAttrNode(EnumAttrNode *e_a)
        : uid(++global_uid),
          line_no(yylineno),
          is_subrange(false),
          subrange_attr(nullptr),
          enum_attr(e_a) {}

int OrdAttrNode::getUid() {
    return uid;
}

std::string OrdAttrNode::getTypeString() {
    return is_subrange ? subrange_attr->getTypeString() : enum_attr->getTypeString();
}

int OrdAttrNode::getLength() {
    return is_subrange ? subrange_attr->getLength() : enum_attr->getLength();
}

int OrdAttrNode::getOffset() {
    return is_subrange ? subrange_attr->getOffset() : enum_attr->getOffset();
}

int OrdAttrNode::getSize() {
    return is_subrange ? subrange_attr->getSize() : enum_attr->getSize();
}

std::string OrdAttrNode::genVizCode(int run) {
    std::string result = vizNode(uid, "OrdAttrNode");
    if (is_subrange) {
        result += vizEdge(uid, subrange_attr->getUid(), "subrange", "Subrange Type");
        result += subrange_attr->genVizCode(run);
    } else {
        result += vizEdge(uid, enum_attr->getUid(), "enum", "Enumerated Type");
        result += enum_attr->genVizCode(run);
    }
    return result;
}

void OrdAttrNode::translateId() {
    try {
        if (is_subrange) subrange_attr->translateId();
    } catch (UndefineError &e) {
        throw e;
    }
}

bool OrdAttrNode::genSymbolTable() {
    try {
        if (!is_subrange) return enum_attr->genSymbolTable();
    } catch (RedefineError &e) {
        throw e;
    }
    return true;
}

bool OrdAttrNode::isTypeEqual(OrdAttrNode *type) {
    if (is_subrange != type->is_subrange) return false;
    return is_subrange ? subrange_attr->isTypeEqual(type->subrange_attr) :
                         enum_attr->isTypeEqual(type->enum_attr);
}

SubrangeAttrNode::SubrangeAttrNode(ExprNode *lb, ExprNode *ub)
        : uid(++global_uid),
          line_no(yylineno),
          low_bound(lb),
          up_bound(ub),
          is_low_id(lb->getNodeType() == el_id),
          is_up_id(ub->getNodeType() == el_id) {}

int SubrangeAttrNode::getUid() {
    return uid;
}

std::string SubrangeAttrNode::getTypeString() {
    return low_bound->getResultType()->getTypeString() + ".." +
           up_bound->getResultType()->getTypeString();
}

int SubrangeAttrNode::getLength() {
    return ALIGN_LEN;
}

int SubrangeAttrNode::getOffset() {
    return 0;
}

int SubrangeAttrNode::getSize() {
    return up_bound->getLiteralNode()->diff(low_bound->getLiteralNode()) + 1;
}

std::string SubrangeAttrNode::genVizCode(int run) {
    std::string result = vizNode(uid, "SubrangeAttrNode");
    result += vizEdge(uid, low_bound->getUid(), "low", "Lower Bound");
    if (!run || !is_low_id) result += low_bound->genVizCode(run);
    result += vizEdge(uid, up_bound->getUid(), "up", "Upper Bound");
    if (!run || !is_up_id) result += up_bound->genVizCode(run);
    return result;
}

void SubrangeAttrNode::translateId() {
    try {
        low_bound = symbol_table.translateConstId(low_bound);
    } catch (UndefineError &e) {
        throw e;
    }
    try {
        up_bound = symbol_table.translateConstId(up_bound);
    } catch (UndefineError &e) {
        throw e;
    }
}

bool SubrangeAttrNode::isTypeEqual(SubrangeAttrNode *type) {
    if (!low_bound->isValueEqual(type->low_bound)) return false;
    if (!up_bound->isValueEqual(type->up_bound)) return false;
    return true;
}

EnumAttrNode::EnumAttrNode(std::vector<ExprNode *> exprs)
        : uid(++global_uid), line_no(yylineno), is_sym_gen(false) {
    items.clear();
    for (ExprNode *expr : exprs) items.push_back(expr);
}

int EnumAttrNode::getUid() {
    return uid;
}

std::string EnumAttrNode::getTypeString() {
    return "enum";
}

int EnumAttrNode::getLength() {
    return ALIGN_LEN;
}

int EnumAttrNode::getOffset() {
    return 0;
}

int EnumAttrNode::getSize() {
    return items.size();
}

std::string EnumAttrNode::genVizCode(int run) {
    std::string result = vizNode(uid, "EnumAttrNode");
    for (int i = 0; i < items.size(); i++) {
        result += vizEdge(uid,
                          items.at(i)->getUid(),
                          "enum" + to_string(i + 1),
                          "Enum at Ordinal " + to_string(i + 1));
        result += items.at(i)->genVizCode(run);
    }
    return result;
}

bool EnumAttrNode::genSymbolTable() {
    if (is_sym_gen) return true;
    is_sym_gen = true;
    for (int i = 0; i < items.size(); i++) {
        std::string id = items.at(i)->getIdNode()->getName();
        if (symbol_table.existSymbol(id)) throw RedefineError(items.at(i)->getLineNumber(), id);
        symbol_table.addSymbol(id, new ConstDefNode(id, new ExprNode(new LiteralNode(i))));
    }
    return true;
}

bool EnumAttrNode::isTypeEqual(EnumAttrNode *type) {
    if (items.size() != type->items.size()) return false;
    for (int i = 0; i < items.size(); i++)
        if (items.at(i) != type->items.at(i)) return false;
    return true;
}

StructAttrNode::StructAttrNode(StructTypeKind  st,
                               ArrayAttrNode  *a_a,
                               StringAttrNode *s_a,
                               RecordAttrNode *r_a)
        : uid(++global_uid),
          line_no(yylineno),
          struct_type(st),
          array_attr(a_a),
          string_attr(s_a),
          record_attr(r_a) {}
StructAttrNode::StructAttrNode(ArrayAttrNode *a_a)
        : StructAttrNode(struct_array, a_a, nullptr, nullptr) {}
StructAttrNode::StructAttrNode(StringAttrNode *s_a)
        : StructAttrNode(struct_string, nullptr, s_a, nullptr) {}
StructAttrNode::StructAttrNode(RecordAttrNode *r_a)
        : StructAttrNode(struct_record, nullptr, nullptr, r_a) {}

int StructAttrNode::getUid() {
    return uid;
}

std::string StructAttrNode::getTypeString() {
    switch (struct_type) {
        case struct_array: return array_attr->getTypeString();
        case struct_string: return "string";
        case struct_record: return record_attr->getTypeString();
    }
    return "";
}

ArrayAttrNode *StructAttrNode::getArrayAttr() {
    return array_attr;
}

RecordAttrNode *StructAttrNode::getRecordAttr() {
    return record_attr;
}

int StructAttrNode::getLength() {
    switch (struct_type) {
        case struct_array: return array_attr->getLength();
        case struct_string: return string_attr->getLength();
        case struct_record: return record_attr->getLength();
    }
    return 0;
}

int StructAttrNode::getOffset(std::string member) {
    switch (struct_type) {
        case struct_array: return 0;
        case struct_string: return 0;
        case struct_record: return record_attr->getOffset(member);
    }
    return 0;
}

std::string StructAttrNode::genVizCode(int run) {
    std::string result = vizNode(uid, "StructAttrNode");
    switch (struct_type) {
        case struct_array:
            result += vizEdge(uid, array_attr->getUid(), "array", "Array Type");
            result += array_attr->genVizCode(run);
            break;
        case struct_string:
            result += vizEdge(uid, string_attr->getUid(), "string", "String Type");
            result += string_attr->genVizCode(run);
            break;
        case struct_record:
            result += vizEdge(uid, record_attr->getUid(), "record", "Record Type");
            result += record_attr->genVizCode(run);
            break;
    }
    return result;
}

void StructAttrNode::translateId() {
    try {
        switch (struct_type) {
            case struct_array: array_attr->translateId(); break;
            case struct_string: string_attr->translateId(); break;
            case struct_record: record_attr->translateId(); break;
        }
    } catch (UndefineError &e) {
        throw e;
    }
}

bool StructAttrNode::isTypeEqual(StructAttrNode *type) {
    if (struct_type != type->struct_type) return false;
    switch (struct_type) {
        case struct_array: return array_attr->isTypeEqual(type->array_attr);
        case struct_string: return true; ;
        case struct_record: return record_attr->isTypeEqual(type->record_attr);
    }
    return false;
}

ArrayAttrNode::ArrayAttrNode(TypeAttrListNode *it, TypeAttrNode *et)
        : uid(++global_uid),
          line_no(yylineno),
          index_type(it),
          element_type(et),
          is_ele_type_id(et->getType() == type_identifier) {}

int ArrayAttrNode::getUid() {
    return uid;
}

int ArrayAttrNode::getDim() {
    return index_type->getDim();
}

TypeAttrNode *ArrayAttrNode::getElementType() {
    return element_type;
}

TypeAttrListNode *ArrayAttrNode::getIndexType() {
    return index_type;
}

std::string ArrayAttrNode::getTypeString() {
    return "[" + index_type->getTypeString() + "] of " + element_type->getTypeString();
}

int ArrayAttrNode::getLength() {
    int result = element_type->getLength();
    try {
        result *= index_type->getTotalSize();
    } catch (IndexTypeError &e) {
        throw e;
    }
    return result;
}

std::string ArrayAttrNode::genVizCode(int run) {
    std::string result = vizNode(uid, "ArrayAttrNode");
    result += vizEdge(uid, index_type->getUid(), "indices", "Type of Indices");
    result += index_type->genVizCode(run);
    result += vizEdge(uid, element_type->getUid(), "element", "Type of Element");
    if (!run || !is_ele_type_id) result += element_type->genVizCode(run);
    return result;
}

void ArrayAttrNode::translateId() {
    try {
        element_type = symbol_table.translateTypeId(element_type);
        element_type->translateId();
    } catch (UndefineError &e) {
        throw e;
    }
    try {
        index_type->translateId();
    } catch (UndefineError &e) {
        throw e;
    }
}

bool ArrayAttrNode::isTypeEqual(ArrayAttrNode *type) {
    if (!element_type->isTypeEqual(type->element_type)) return false;
    return index_type->isTypeEqual(type->index_type);
}

bool ArrayAttrNode::testIndexType(ExprListNode *indices) {
    try {
        // TODO temporary removed
        // index_type->testIndexType(indices);
    } catch (IndexDimensionError &e) {
        throw e;
    } catch (IndexTypeError &e) {
        throw e;
    }
    return true;
}

StringAttrNode::StringAttrNode(ExprNode *l)
        : uid(++global_uid), line_no(yylineno), len(l), is_len_id(l->getNodeType() == el_id) {}

int StringAttrNode::getUid() {
    return uid;
}

int StringAttrNode::getLength() {
    return stoi(len->getLiteralNode()->toString()) * ALIGN_LEN;
}

std::string StringAttrNode::genVizCode(int run) {
    std::string result = vizNode(uid, "StringAttrNode");
    result += vizEdge(uid, len->getUid(), "len", "String Length");
    if (!run || !is_len_id) result += len->genVizCode(run);
    return result;
}

void StringAttrNode::translateId() {
    try {
        len = symbol_table.translateConstId(len);
    } catch (UndefineError &e) {
        throw e;
    }
}

RecordAttrNode::RecordAttrNode(VarDefListNode *d) : uid(++global_uid), line_no(yylineno), defs(d) {}

int RecordAttrNode::getUid() {
    return uid;
}

int RecordAttrNode::getDim() {
    return defs->getDim();
}

std::string RecordAttrNode::getTypeString() {
    return defs->getTypeString();
}

VarDefNode *RecordAttrNode::getVarDef(std::string id) {
    return defs->getVarDef(id);
}

int RecordAttrNode::getLength() {
    return defs->getLength();
}

int RecordAttrNode::getOffset(std::string member) {
    return defs->getOffset(member);
}

std::string RecordAttrNode::genVizCode(int run) {
    std::string result = vizNode(uid, "RecordAttrNode");
    result += vizEdge(uid, defs->getUid(), "fields", "Record Fields");
    result += defs->genVizCode(run);
    return result;
}

void RecordAttrNode::translateId() {
    try {
        defs->translateId();
    } catch (UndefineError &e) {
        throw e;
    }
}

bool RecordAttrNode::isTypeEqual(RecordAttrNode *type) {
    return defs->isTypeEqual(type->defs);
}

PtrAttrNode::PtrAttrNode(TypeAttrNode *e)
        : uid(++global_uid),
          line_no(yylineno),
          base_attr(e),
          is_base_id(e->getType() == type_identifier) {}

int PtrAttrNode::getUid() {
    return uid;
}

std::string PtrAttrNode::getTypeString() {
    return "pointer to " + base_attr->getTypeString();
}

int PtrAttrNode::getLength() {
    return ALIGN_LEN;
}

int PtrAttrNode::getOffset() {
    return 0;
}

std::string PtrAttrNode::genVizCode(int run) {
    std::string result = vizNode(uid, "PtrAttrNode");
    result += vizEdge(uid, base_attr->getUid(), "base", "Basic Type");
    if (!run || !is_base_id) result += base_attr->genVizCode(run);
    return result;
}

void PtrAttrNode::translateId() {
    try {
        base_attr = symbol_table.translateTypeId(base_attr);
    } catch (UndefineError &e) {
        throw e;
    }
}

bool PtrAttrNode::isTypeEqual(PtrAttrNode *type) {
    return base_attr->isTypeEqual(type->base_attr);
}