#include "include/node_func.hpp"
#include "include/asmgen.hpp"

ParamDefNode::ParamDefNode(bool is_r, std::string id, TypeAttrNode *t)
        : uid(++global_uid), line_no(yylineno), is_ref(is_r), var_def(new VarDefNode(id, t)) {}
ParamDefNode::~ParamDefNode() {
    if (var_def != nullptr) delete var_def;
}

int ParamDefNode::getUid() {
    return uid;
}

VarDefNode *ParamDefNode::getVarDef() {
    return var_def;
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

int ParamDefNode::genSymbolTable(int offset) {
    try {
        var_def->translateId();
    } catch (UndefineError &e) {
        throw e;
    }
    if (symbol_table.existSymbol(var_def->getName()))
        throw RedefineError(line_no, var_def->getName());
    symbol_table.addSymbol(var_def->getName(), var_def, offset);
    return var_def->getType()->getLength();
}

bool ParamDefNode::testArgType(TypeAttrNode *type) {
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
    int offset = 0;
    for (int i = 0; i < param_defs.size(); i++) try {
            offset += param_defs.at(i)->genSymbolTable(offset);
        } catch (RedefineError &e) {
            throw e;
        }
    return true;
}

bool ParamDefListNode::testArgType(ExprListNode *args) {
    std::vector<ExprNode *> &arg_list = args->getExprList();
    if (param_defs.size() != arg_list.size())
        throw ArgumentNumberError(args->getLineNumber(), param_defs.size(), arg_list.size());
    for (int i = 0; i < param_defs.size(); i++)
        if (!param_defs.at(i)->testArgType(arg_list.at(i)->getResultType()))
            throw ArgumentTypeError(args->getLineNumber(),
                                    i + 1,
                                    param_defs.at(i)->getVarDef()->getType()->getTypeString(),
                                    arg_list.at(i)->getResultType()->getTypeString());
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
        : uid(++global_uid),
          line_no(yylineno),
          is_func(true),
          name(id),
          param_defs(p_d),
          retval_type(r_v),
          block(b) {}

int FuncDefNode::getUid() {
    return uid;
}

int FuncDefNode::getLineNumber() {
    return line_no;
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
    return symbol_table.addSymbol(name, this);
}

bool FuncDefNode::testArgType(ExprListNode *args) {
    if (param_defs == nullptr && args->getDim())
        throw ArgumentNumberError(args->getLineNumber(), 0, args->getDim());
    try {
        param_defs->testArgType(args);
    } catch (ArgumentNumberError &e) {
        throw e;
    } catch (ArgumentTypeError &e) {
        throw e;
    }

    return true;
}

void FuncDefNode::visit() {
    symbol_table.enterScope();
    try {
        if (param_defs != nullptr) {
            param_defs->genSymbolTable();
        }
        block->visit(name);
    } catch (RedefineError &e) {
        error_handler.addMsg(e);
    }
    symbol_table.leaveScope();
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
    for (FuncDefNode *def : func_defs) try {
            def->genSymbolTable();
        } catch (RedefineError &e) {
            error_handler.addMsg(e);
        }
    return true;
}

void FuncDefListNode::visit() {
    for (FuncDefNode *def : func_defs) def->visit();
}