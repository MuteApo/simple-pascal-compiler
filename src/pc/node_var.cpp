#include "include/node_var.hpp"
#include "include/symbol_table.hpp"

VarDefNode::VarDefNode(std::string id, TypeAttrNode *t)
        : uid(++global_uid), name(id), type(t), is_type_id(t->getType() == type_identifier) {}

bool VarDefNode::is_legal() {
    return symbol_table.findTypeSymbol(type->getName()) != nullptr;
}

int VarDefNode::get_length(void) {
    TypeAttrNode *type_def = symbol_table.findTypeSymbol(type->getName());
    if (type_def == nullptr) return -1;
    return type_def->get_length();
}

std::string VarDefNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "VarDefNode\n" + name);
    result += vizChildEdge(uid, type->getUid(), "vardef", "Variable Definition");
    if (!run || !is_type_id) result += type->gen_viz_code(run);
    return result;
}

void VarDefNode::translateId() {
    type = symbol_table.translateTypeId(type);
    type->translateId();
}

bool VarDefNode::gen_sym_tab(int ord) {
    type = symbol_table.translateTypeId(type);
    type->translateId();
    return symbol_table.addSymbol(name, this, ord);
}

std::string VarDefNode::gen_asm_def() {
    TypeAttrNode *type_def = symbol_table.findTypeSymbol(type->getName());
    if (type_def == nullptr) return "";
    return type_def->gen_asm_def();
}

void VarDefListNode::addVarDef(IdListNode *ids, TypeAttrNode *type) {
    for (IdNode *id : ids->getIdList()) addVarDef(new VarDefNode(id->getName(), type));
}

std::string VarDefListNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "VarDefListNode");
    for (int i = 0; i < var_defs.size(); i++) {
        result += vizChildEdge(uid,
                               var_defs.at(i)->getUid(),
                               "field" + to_string(i + 1),
                               "Type of Field " + to_string(i + 1));
        result += var_defs.at(i)->gen_viz_code(run);
    }
    return result;
}

void VarDefListNode::translateId() {
    for (VarDefNode *var : var_defs) var->translateId();
}

bool VarDefListNode::gen_sym_tab(void) {
    bool result       = true;
    int  lvars_length = 0;
    int  ord          = 0;
    for (VarDefNode *var : var_defs) {
        result &= var->gen_sym_tab(ord++);
        lvars_length += var->get_length();
    }
    ar_lvars_length.push_back(lvars_length);
    return result;
}

std::string VarDefListNode::gen_asm_def() {
    std::string asm_def = "";
    for (VarDefNode *var : var_defs) asm_def += var->gen_asm_def();
    return asm_def;
}