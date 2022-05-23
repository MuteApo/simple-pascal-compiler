#include "include/node_var.hpp"
#include "include/symbol_table.hpp"

VarDefNode::VarDefNode(std::string id, TypeAttrNode *t)
        : uid(++global_uid),
          line_no(yylineno),
          name(id),
          type(t),
          is_type_id(t->getType() == type_identifier) {}

int VarDefNode::getUid() {
    return uid;
}

std::string VarDefNode::getName() {
    return name;
}

TypeAttrNode *VarDefNode::getType() {
    return type;
}

bool VarDefNode::isLegal() {
    return symbol_table.findTypeSymbol(type->getName()) != nullptr;
}

std::string VarDefNode::genVizCode(int run) {
    std::string result = vizNode(uid, "VarDefNode\n" + name);
    result += vizEdge(uid, type->getUid(), "vardef", "Variable Definition");
    if (!run || !is_type_id) result += type->genVizCode(run);
    return result;
}

void VarDefNode::translateId() {
    type = symbol_table.translateTypeId(type);
    type->translateId();
}

bool VarDefNode::genSymbolTable(int ord) {
    type = symbol_table.translateTypeId(type);
    type->translateId();
    return symbol_table.addSymbol(name, this, ord);
}

std::string VarDefNode::genAsmDef() {
    TypeAttrNode *type_def = symbol_table.findTypeSymbol(type->getName());
    if (type_def == nullptr) return "";
    return type_def->genAsmDef();
}

VarDefListNode::VarDefListNode() : uid(++global_uid), line_no(yylineno) {
    var_defs.clear();
}

int VarDefListNode::getUid() {
    return uid;
}

int VarDefListNode::getDim() {
    return var_defs.size();
}

std::vector<VarDefNode *> &VarDefListNode::getVarList() {
    return var_defs;
}

VarDefNode *VarDefListNode::getVarDef(std::string id) {
    for (VarDefNode *def : var_defs)
        if (def->getName() == id) return def;
    return nullptr;
}

void VarDefListNode::addVarDef(VarDefNode *var_def) {
    var_defs.push_back(var_def);
}
void VarDefListNode::addVarDef(IdListNode *ids, TypeAttrNode *type) {
    for (IdNode *id : ids->getIdList()) addVarDef(new VarDefNode(id->getName(), type));
}

void VarDefListNode::mergeVarDefList(VarDefListNode *defs) {
    for (VarDefNode *def : defs->getVarList()) addVarDef(def);
}

int VarDefListNode::getLength() {
    int result = 0;
    for (VarDefNode *def : var_defs) result += def->getType()->getLength();
    return result;
}

int VarDefListNode::getOffset(std::string member) {
    int result = 0;
    for (VarDefNode *def : var_defs) {
        if (def->getName() == member) break;
        result += def->getType()->getLength();
    }
    return result;
}

std::string VarDefListNode::genVizCode(int run) {
    std::string result = vizNode(uid, "VarDefListNode");
    for (int i = 0; i < var_defs.size(); i++) {
        result += vizEdge(uid,
                          var_defs.at(i)->getUid(),
                          "field" + to_string(i + 1),
                          "Type of Field " + to_string(i + 1));
        result += var_defs.at(i)->genVizCode(run);
    }
    return result;
}

void VarDefListNode::translateId() {
    for (int i = 0; i < var_defs.size(); i++) var_defs.at(i)->translateId();
}

bool VarDefListNode::genSymbolTable() {
    bool result       = true;
    int  lvars_length = 0;
    int  ord          = 0;
    for (VarDefNode *var : var_defs) {
        result &= var->genSymbolTable(ord++);
        lvars_length += var->getType()->getLength();
    }
    ar_lvars_length.push_back(lvars_length);
    return result;
}

bool VarDefListNode::isTypeEqual(VarDefListNode *rhs) {
    if (getDim() != rhs->getDim()) return false;
    for (int i = 0; i < var_defs.size(); i++)
        if (!var_defs.at(i)->getType()->isTypeEqual(rhs->var_defs.at(i)->getType())) return false;
    return true;
}

std::string VarDefListNode::genAsmDef() {
    std::string asm_def = "";
    for (VarDefNode *var : var_defs) asm_def += var->genAsmDef();
    return asm_def;
}