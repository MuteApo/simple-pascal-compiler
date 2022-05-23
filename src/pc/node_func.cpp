#include "include/node_func.hpp"

ParamDefNode::ParamDefNode(bool is_r, std::string id, TypeAttrNode *t)
        : uid(++global_uid), line_no(yylineno), is_ref(is_r), var_def(new VarDefNode(id, t)) {}
ParamDefNode::~ParamDefNode() {
    if (var_def != nullptr) delete var_def;
}

int ParamDefNode::getUid() {
    return uid;
}

std::string ParamDefNode::getNodeInfo() {
    std::string result = "ParamDefNode";
    if (is_ref) result += "(ref)";
    return result;
}

std::string ParamDefNode::genVizCode(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    result += vizEdge(uid, var_def->getUid(), "paramdef", "Parameter Definition");
    result += var_def->genVizCode(run);
    return result;
}

bool ParamDefNode::genSymbolTable(int order) {
    var_def->translateId();
    return symbol_table.addSymbol(var_def->getName(), var_def, order);
}

bool ParamDefNode::test_arg_type(TypeAttrNode *type) {
    return var_def->getType()->isTypeEqual(type);
}

ParamDefListNode::ParamDefListNode() : uid(++global_uid), line_no(yylineno) {
    param_defs.clear();
}

int ParamDefListNode::getUid() {
    return uid;
}

std::vector<ParamDefNode *> &ParamDefListNode::getParamList() {
    return param_defs;
}

void ParamDefListNode::addParamDef(ParamDefNode *param) {
    param_defs.push_back(param);
}
void ParamDefListNode::addParamDef(bool is_ref, IdListNode *ids, TypeAttrNode *type) {
    for (IdNode *id : ids->getIdList()) addParamDef(new ParamDefNode(is_ref, id->getName(), type));
}

void ParamDefListNode::mergeParamDefList(ParamDefListNode *defs) {
    for (ParamDefNode *def : defs->getParamList()) addParamDef(def);
}

std::string ParamDefListNode::genVizCode(int run) {
    std::string result = vizNode(uid, "ParamDefListNode");
    for (int i = 0; i < param_defs.size(); i++) {
        result += vizEdge(uid,
                          param_defs.at(i)->getUid(),
                          "param" + to_string(i + 1),
                          "Type of Param " + to_string(i + 1));
        result += param_defs.at(i)->genVizCode(run);
    }
    return result;
}

bool ParamDefListNode::genSymbolTable() {
    bool result = true;
    int  ord    = 0;
    for (ParamDefNode *def : param_defs) result &= def->genSymbolTable(ord++);
    return result;
}

bool ParamDefListNode::test_arg_type(std::vector<ExprNode *> args) {
    if (param_defs.size() != args.size()) return false;
    for (int i = 0; i < param_defs.size(); i++)
        if (!param_defs.at(i)->test_arg_type(args.at(i)->getResultType())) return false;
    return true;
}

FuncDefNode::FuncDefNode(std::string id, ParamDefListNode *p_d, BlockNode *b)
        : uid(++global_uid),
          line_no(yylineno),
          is_func(false),
          name(id),
          param_defs(p_d),
          retval_type(nullptr),
          block(b) {}
FuncDefNode::FuncDefNode(std::string id, ParamDefListNode *p_d, TypeAttrNode *r_v, BlockNode *b)
        : uid(++global_uid), is_func(true), name(id), param_defs(p_d), retval_type(r_v), block(b) {}

int FuncDefNode::getUid() {
    return uid;
}

TypeAttrNode *FuncDefNode::getRetValType() {
    return retval_type;
}

bool FuncDefNode::hasDecl() {
    if (param_defs != nullptr) return true;
    if (block->hasDecl()) return true;
    return false;
}

std::string FuncDefNode::genVizCode(int run) {
    std::string result = vizNode(uid, "FuncDefNode\n" + name);
    if (is_func) {
        result += vizEdge(uid, retval_type->getUid(), "retval", "Return Value Type");
        result += retval_type->genVizCode(run);
    }
    if (param_defs != nullptr) {
        result += vizEdge(uid, param_defs->getUid(), "params", "Type of Parameters");
        result += param_defs->genVizCode(run);
    }
    if (block != nullptr) {
        result += vizEdge(uid,
                          block->getUid(),
                          "block",
                          std::string(is_func ? "Function" : "Procedure") + " Block");
        result += block->genVizCode(run);
    }
    return result;
}

bool FuncDefNode::genSymbolTable() {
    symbol_table.addSymbol(name, this);
    if (hasDecl()) symbol_table.enterScope();
    if (param_defs != nullptr) param_defs->genSymbolTable();
    block->visit();
    if (hasDecl()) symbol_table.leaveScope();
    return true;
}

bool FuncDefNode::test_arg_type(ExprListNode *args) {
    return param_defs->test_arg_type(args->getExprList());
}

FuncDefListNode::FuncDefListNode() : uid(++global_uid), line_no(yylineno) {
    func_defs.clear();
}

int FuncDefListNode::getUid() {
    return uid;
}

void FuncDefListNode::addFunc(FuncDefNode *func_def) {
    func_defs.push_back(func_def);
}

std::string FuncDefListNode::genVizCode(int run) {
    std::string result = vizNode(uid, "FuncDefListNode");
    for (int i = 0; i < func_defs.size(); i++) {
        result += vizEdge(uid,
                          func_defs.at(i)->getUid(),
                          "func" + to_string(i + 1),
                          "Function " + to_string(i + 1));
        result += func_defs.at(i)->genVizCode(run);
    }
    return result;
}

bool FuncDefListNode::genSymbolTable() {
    bool result = true;
    for (FuncDefNode *def : func_defs) result &= def->genSymbolTable();
    return result;
}