#include "include/node_expr.hpp"
#include "include/symbol_table.hpp"

ExprNode::ExprNode(expr_node_type nt,
                   ExprEvalType   et,
                   ExprNode      *op_1,
                   ExprNode      *op_2,
                   LiteralNode   *l_a,
                   VarAccessNode *v_a_a,
                   IdNode        *i_a,
                   FuncNode      *f_a)
        : uid(++global_uid),
          line_no(yylineno),
          node_type(nt),
          eval_type(et),
          op1(op_1),
          op2(op_2),
          literal_attr(l_a),
          var_access_attr(v_a_a),
          id_attr(i_a),
          func_attr(f_a),
          res_type(nullptr) {}
ExprNode::ExprNode(ExprEvalType et, ExprNode *op1_, ExprNode *op2_)
        : ExprNode(el_nonleaf, et, op1_, op2_, nullptr, nullptr, nullptr, nullptr) {}
ExprNode::ExprNode(LiteralNode *l_a)
        : ExprNode(el_literal, EK_None, nullptr, nullptr, l_a, nullptr, nullptr, nullptr) {}
ExprNode::ExprNode(VarAccessNode *v_a_a)
        : ExprNode(el_var_access, EK_None, nullptr, nullptr, nullptr, v_a_a, nullptr, nullptr) {}
ExprNode::ExprNode(IdNode *i_a)
        : ExprNode(el_id, EK_None, nullptr, nullptr, nullptr, nullptr, i_a, nullptr) {}
ExprNode::ExprNode(FuncNode *f_a)
        : ExprNode(el_fun_call, EK_None, nullptr, nullptr, nullptr, nullptr, nullptr, f_a) {}

int ExprNode::getUid() {
    return uid;
}

expr_node_type ExprNode::getExprType() {
    return node_type;
}

IdNode *ExprNode::getIdNode() {
    return id_attr;
}

LiteralNode *ExprNode::getLiteralNode() {
    return literal_attr;
}

VarAccessNode *ExprNode::getVarAccessNode() {
    return var_access_attr;
}

std::string ExprNode::getNodeInfo() {
    std::string result = "ExprNode\n";
    if (node_type == el_nonleaf) result += enum2str(eval_type);
    return result;
}

std::string ExprNode::genVizCode(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    switch (node_type) {
        case el_nonleaf:
            result += vizEdge(uid, op1->getUid(), "op1", "Operand 1");
            result += op1->genVizCode(run);
            if (op2 != nullptr) {
                result += vizEdge(uid, op2->getUid(), "op2", "Operand 2");
                result += op2->genVizCode(run);
            }
            break;
        case el_literal:
            result += vizEdge(uid, literal_attr->getUid(), "literal", "Literal");
            result += literal_attr->genVizCode(run);
            break;
        case el_var_access:
            result += vizEdge(uid, var_access_attr->getUid(), "var access", "Variable Access");
            result += var_access_attr->genVizCode(run);
            break;
        case el_id:
            result += vizEdge(uid, id_attr->getUid(), "id", "Identifier");
            result += id_attr->genVizCode(run);
            break;
        case el_fun_call:
            result += vizEdge(uid, func_attr->getUid(), "fun call", "Function Call");
            result += func_attr->genVizCode(run);
            break;
    }
    return result;
}

TypeAttrNode *ExprNode::getResultType() {
    switch (node_type) {
        case el_nonleaf: {
            TypeAttrNode *t1 = op1->getResultType();
            if (op2 != nullptr) {
                TypeAttrNode *t2 = op2->getResultType();
                if (!t1->isTypeEqual(t2)) {
                    // TODO syntax error: type of operands is not equal
                    return nullptr;
                }
            }
            return res_type = t1;
        }
        case el_literal: return res_type = literal_attr->getResultType();
        case el_var_access: return res_type = var_access_attr->getResultType();
        case el_id: return res_type = id_attr->getResultType();
        case el_fun_call: return res_type = func_attr->getResultType();
    }
    return nullptr;
}

bool ExprNode::isValueEqual(ExprNode *expr) {
    if (node_type != expr->node_type) return false;
    switch (node_type) {
        case el_literal:
            return literal_attr->isValueEqual(expr->literal_attr);
            // TODO: how to compare const expressions?
    }
    return false;
}

ExprListNode::ExprListNode() : uid(++global_uid), line_no(yylineno) {
    exprs.clear();
}

int ExprListNode::getUid() {
    return uid;
}

int ExprListNode::getDim() {
    return exprs.size();
}

std::vector<ExprNode *> &ExprListNode::getExprList() {
    return exprs;
}

void ExprListNode::addExpr(ExprNode *expr) {
    exprs.push_back(expr);
}

std::string ExprListNode::genVizCode(int run) {
    std::string result = vizNode(uid, "ExprListNode");
    for (int i = 0; i < exprs.size(); i++) {
        result += vizEdge(uid,
                          exprs.at(i)->getUid(),
                          "expr" + to_string(i + 1),
                          "Expression " + to_string(i + 1));
        result += exprs.at(i)->genVizCode(run);
    }
    return result;
}

LiteralNode::LiteralNode(bool is_n, BasicAttrNode *t, bool bv, int iv, double dv, char cv)
        : uid(++global_uid),
          line_no(yylineno),
          is_nil(is_n),
          type(t),
          bval(bv),
          ival(iv),
          dval(dv),
          cval(cv) {}
LiteralNode::LiteralNode(void) : LiteralNode(true, nullptr, 0, 0, 0, 0) {}
LiteralNode::LiteralNode(bool b) : LiteralNode(false, new BasicAttrNode(boolean), b, 0, 0, 0) {}
LiteralNode::LiteralNode(int i) : LiteralNode(false, new BasicAttrNode(integer), 0, i, 0, 0) {}
LiteralNode::LiteralNode(double d) : LiteralNode(false, new BasicAttrNode(real), 0, 0, d, 0) {}
LiteralNode::LiteralNode(char c) : LiteralNode(false, new BasicAttrNode(character), 0, 0, 0, c) {}
LiteralNode::~LiteralNode() {
    if (type != nullptr) delete type;
}

bool LiteralNode::operator<(const LiteralNode &rhs) const {
    if (is_nil) return !rhs.is_nil;
    if (type->getType() != rhs.type->getType()) return type->line_no < rhs.type->line_no;
    switch (type->getType()) {
        case boolean: return bval < rhs.bval;
        case integer: return ival < rhs.ival;
        case real: return dval < rhs.dval;
        case character: return cval < rhs.cval;
    }
    return false;
}

int LiteralNode::getUid() {
    return uid;
}

BasicAttrNode *LiteralNode::getType() {
    return type;
}

std::string LiteralNode::getNodeInfo() {
    std::string result = "LiteralNode\n";
    if (is_nil) return result + "NIL";
    result += type->getNodeInfo() + "\n";
    if (type->getType() == boolean) return result + to_string(bval);
    if (type->getType() == integer) return result + to_string(ival);
    if (type->getType() == real) return result + to_string(dval);
    if (type->getType() == character) return result + to_string(cval);
    return result + "illegal literal";
}

std::string LiteralNode::genVizCode(int run) {
    return vizNode(uid, getNodeInfo());
}

TypeAttrNode *LiteralNode::getResultType() {
    return new TypeAttrNode(type);
}

int LiteralNode::diff(LiteralNode *rhs) {
    if (is_nil || rhs->is_nil) {
        // TODO syntax error: cannot differ between pointers
        return 0;
    }
    if (type != rhs->type) {
        // TODO syntax error: cannot cannot differ between two types
        return 0;
    }
    switch (type->getType()) {
        case boolean: return bval - rhs->bval;
        case integer: return ival - rhs->ival;
        case real: return dval - rhs->dval;
        case character: return cval - rhs->cval;
    }
    return 0;
}

bool LiteralNode::isValueEqual(LiteralNode *expr) {
    if (is_nil && expr->is_nil) return true;
    if (is_nil != expr->is_nil) return false;
    if (type->getType() != expr->type->getType()) return false;
    switch (type->getType()) {
        case boolean: return bval == expr->bval;
        case integer: return ival == expr->ival;
        case real: return dval == expr->dval;
        case character: return cval == expr->cval;
    }
    return false;
}

std::string LiteralNode::toString() {
    switch (type->type) {
        case boolean: return to_string(bval);
        case integer: return to_string(ival);
        case real: return to_string(dval);
        case character: return to_string(cval);
    }
    return "";
}

VarAccessNode::VarAccessNode(var_access_type t, ExprNode *h, ExprListNode *i_l, ExprNode *m)
        : uid(++global_uid),
          line_no(yylineno),
          type(t),
          host(h),
          index_list(i_l),
          member(m),
          res_type(nullptr) {}
VarAccessNode::VarAccessNode(ExprNode *h) : VarAccessNode(va_pointer, h, nullptr, nullptr) {}
VarAccessNode::VarAccessNode(ExprNode *h, ExprListNode *indices)
        : VarAccessNode(va_array, h, indices, nullptr) {}
VarAccessNode::VarAccessNode(ExprNode *h, ExprNode *m) : VarAccessNode(va_record, h, nullptr, m) {}

int VarAccessNode::getUid() {
    return uid;
}

std::string VarAccessNode::getNodeInfo() {
    std::string result = "VarAccessNode\n";
    if (type == va_pointer) result += "^";
    return result;
}

std::string VarAccessNode::genVizCode(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    result += vizEdge(uid, host->getUid(), "host", "Host Name");
    result += host->genVizCode(run);
    if (type == va_array) {
        result += vizEdge(uid, index_list->getUid(), "array index", "Array Index");
        result += index_list->genVizCode(run);
    }
    if (type == va_record) {
        result += vizEdge(uid, member->getUid(), "record field", "Record Field");
        result += member->genVizCode(run);
    }
    return result;
}

TypeAttrNode *VarAccessNode::getResultType() {
    TypeAttrNode *type_attr =
        host->getExprType() == el_literal ?
            symbol_table.findVarSymbol(host->getIdNode()->getName())->getType() :
            host->getResultType();
    switch (type) {
        case va_pointer: return res_type = type_attr;
        case va_array:
            return res_type = type_attr->getStructAttr()->getArrayAttr()->getElementType();
        case va_record:
            return res_type = type_attr->getStructAttr()
                                  ->getRecordAttr()
                                  ->getVarDef(member->getIdNode()->getName())
                                  ->getType();
    }
    return nullptr;
}

IdNode::IdNode(std::string id)
        : uid(++global_uid), line_no(yylineno), name(id), res_type(nullptr) {}

int IdNode::getUid() {
    return uid;
}

std::string IdNode::getName() {
    return name;
}

ConstDefNode *IdNode::getConst() {
    return symbol_table.findConstSymbol(name);
}

TypeAttrNode *IdNode::getType() {
    return symbol_table.findTypeSymbol(name);
}

std::string IdNode::getNodeInfo() {
    return "IdNode\n" + name;
}

std::string IdNode::genVizCode(int run) {
    return vizNode(uid, getNodeInfo());
}

TypeAttrNode *IdNode::getResultType() {
    return res_type = symbol_table.findVarSymbol(name)->getType();
}

IdListNode::IdListNode() : uid(++global_uid), line_no(yylineno) {
    ids.clear();
}

int IdListNode::getUid() {
    return uid;
}

std::vector<IdNode *> &IdListNode::getIdList() {
    return ids;
}

std::string IdListNode::getIdString(std::string sep) {
    if (ids.empty()) return "";
    std::string result = "(" + ids.at(0)->getName();
    for (int i = 1; i < ids.size(); i++) result += sep + ids.at(i)->getName();
    return result + ")";
}

void IdListNode::addId(IdNode *id) {
    ids.push_back(id);
}

FuncNode::FuncNode(std::string id, ExprListNode *a_l)
        : uid(++global_uid), line_no(yylineno), func_name(id), arg_list(a_l), res_type(nullptr) {}

int FuncNode::getUid() {
    return uid;
}

std::string FuncNode::getNodeInfo() {
    return "FuncNode\n" + func_name;
}

std::string FuncNode::genVizCode(int run) {
    std::string result = vizNode(uid, getNodeInfo());
    if (arg_list != nullptr) {
        result += vizEdge(uid, arg_list->getUid(), "args", "Arguments of Function Call");
        result += arg_list->genVizCode(run);
    }
    return result;
}

bool FuncNode::test_arg_type() {
    FuncDefNode *func = symbol_table.findFuncSymbol(func_name);
    return func->test_arg_type(arg_list);
}

TypeAttrNode *FuncNode::getResultType() {
    return res_type = symbol_table.findFuncSymbol(func_name)->getRetValType();
}