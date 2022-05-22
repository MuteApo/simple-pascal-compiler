#include "include/node_func.hpp"

ParamDefNode::ParamDefNode(bool is_r, std::string id, TypeAttrNode *t)
        : uid(++global_uid), is_ref(is_r), var_def(new VarDefNode(id, t)) {}
ParamDefNode::~ParamDefNode() {
    if (var_def != nullptr) delete var_def;
}

std::string ParamDefNode::getNodeInfo() {
    std::string result = "ParamDefNode";
    if (is_ref) result += "(ref)";
    return result;
}

std::string ParamDefNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    result += vizChildEdge(uid, var_def->getUid(), "paramdef", "Parameter Definition");
    result += var_def->gen_viz_code(run);
    return result;
}

bool ParamDefNode::gen_sym_tab(int order) {
    var_def->translateId();
    return symbol_table.addSymbol(var_def->getName(), var_def, order);
}

bool ParamDefNode::test_arg_type(TypeAttrNode *type) {
    return var_def->getType()->is_type_equ(type);
}

void ParamDefListNode::addParamDef(bool is_ref, IdListNode *ids, TypeAttrNode *type) {
    for (IdNode *id : ids->getIdList()) addParamDef(new ParamDefNode(is_ref, id->getName(), type));
}

std::string ParamDefListNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "ParamDefListNode");
    for (int i = 0; i < param_defs.size(); i++) {
        result += vizChildEdge(uid,
                               param_defs.at(i)->getUid(),
                               "param" + to_string(i + 1),
                               "Type of Param " + to_string(i + 1));
        result += param_defs.at(i)->gen_viz_code(run);
    }
    return result;
}

bool ParamDefListNode::gen_sym_tab() {
    bool result = true;
    int  ord    = 0;
    for (ParamDefNode *def : param_defs) result &= def->gen_sym_tab(ord++);
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
          is_func(false),
          name(id),
          param_defs(p_d),
          retval_type(nullptr),
          block(b) {}
FuncDefNode::FuncDefNode(std::string id, ParamDefListNode *p_d, TypeAttrNode *r_v, BlockNode *b)
        : uid(++global_uid), is_func(true), name(id), param_defs(p_d), retval_type(r_v), block(b) {}

bool FuncDefNode::hasDecl() {
    if (param_defs != nullptr) return true;
    if (block->hasDecl()) return true;
    return false;
}

std::string FuncDefNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "FuncDefNode\n" + name);
    if (is_func) {
        result += vizChildEdge(uid, retval_type->getUid(), "retval", "Return Value Type");
        result += retval_type->gen_viz_code(run);
    }
    if (param_defs != nullptr) {
        result += vizChildEdge(uid, param_defs->getUid(), "params", "Type of Parameters");
        result += param_defs->gen_viz_code(run);
    }
    if (block != nullptr) {
        result += vizChildEdge(uid,
                               block->getUid(),
                               "block",
                               std::string(is_func ? "Function" : "Procedure") + " Block");
        result += block->gen_viz_code(run);
    }
    return result;
}

bool FuncDefNode::gen_sym_tab() {
    symbol_table.addSymbol(name, this);
    if (hasDecl()) symbol_table.enterScope();
    if (param_defs != nullptr) param_defs->gen_sym_tab();
    block->visit();
    if (hasDecl()) symbol_table.leaveScope();
    return true;
}

bool FuncDefNode::test_arg_type(ExprListNode *args) {
    return param_defs->test_arg_type(args->getExprList());
}

std::string FuncDefListNode::gen_viz_code(int run) {
    std::string result = vizNode(uid, "FuncDefListNode");
    for (int i = 0; i < func_defs.size(); i++) {
        result += vizChildEdge(uid,
                               func_defs.at(i)->getUid(),
                               "func" + to_string(i + 1),
                               "Function " + to_string(i + 1));
        result += func_defs.at(i)->gen_viz_code(run);
    }
    return result;
}

bool FuncDefListNode::gen_sym_tab() {
    bool result = true;
    for (FuncDefNode *def : func_defs) result &= def->gen_sym_tab();
    return result;
}