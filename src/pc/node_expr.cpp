#include "include/node_expr.hpp"
#include "include/asmgen.hpp"
#include "include/exception.hpp"
#include "include/symbol_table.hpp"

ExprNode::ExprNode(ExprNodeType   nt,
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

int ExprNode::getLineNumber() {
    return line_no;
}

ExprNodeType ExprNode::getNodeType() {
    return node_type;
}

std::string ExprNode::getExprTypeString(ExprEvalType e) {
    switch (e) {
        case EK_Add: return "Add";
        case EK_Sub: return "Sub";
        case EK_Mul: return "Mul";
        case EK_Div: return "Div";
        case EK_Mod: return "Mod";
        case EK_Fdiv: return "Float Div";
        case EK_Eq: return "=";
        case EK_Ne: return "<>";
        case EK_Lt: return "<";
        case EK_Gt: return ">";
        case EK_Le: return "<=";
        case EK_Ge: return ">=";
        case EK_Not: return "!";
        case EK_And: return "&";
        case EK_Or: return "|";
        case EK_Xor: return "^";
        case EK_Shl: return "<<";
        case EK_Shr: return ">>";
        case EK_In: return "In";
    }
    return "";
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
    if (node_type == el_nonleaf) result += getExprTypeString(eval_type);
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
    if (res_type != nullptr) return res_type;
    try {
        switch (node_type) {
            case el_nonleaf: {
                TypeAttrNode *t1 = op1->getResultType();
                if (op2 == nullptr) return res_type = t1;
                TypeAttrNode *t2 = op2->getResultType();
                if (!t1->isTypeEqual(t2))
                    throw ExpressionTypeError(
                        op1->getLineNumber(), t1->getTypeString(), t2->getTypeString());
                if (eval_type == EK_Eq || eval_type == EK_Ne || eval_type == EK_Lt ||
                    eval_type == EK_Gt || eval_type == EK_Le || eval_type == EK_Ge)
                    return res_type = new TypeAttrNode(new BasicAttrNode(boolean));
                return res_type = t2;
            }
            case el_literal: return res_type = literal_attr->getResultType();
            case el_var_access: return res_type = var_access_attr->getResultType();
            case el_id: return res_type = id_attr->getResultType();
            case el_fun_call: return res_type = func_attr->getResultType();
        }
    } catch (Exception &e) {
        throw e;
    }
    return nullptr;
}

bool ExprNode::isValueEqual(ExprNode *expr) {
    if (node_type != expr->node_type) return false;
    if (node_type != el_literal) return false;
    return literal_attr->isValueEqual(expr->literal_attr);
}

// Result is always in t0 register
std::string ExprNode::genAsmCodeRHS() {  // Only for right value code generation
    std::string asm_code = "";
    if (error_handler.getErrorCount() != 0) return asm_code;
    ExprNodeType node_type = getNodeType();
    if (node_type == el_nonleaf) {
        asm_code += op1->genAsmCodeRHS();
        asm_code += get_reg_save(t_table[0]);
        if (op2 != nullptr) {
            asm_code += op2->genAsmCodeRHS();
            asm_code += get_reg_xchg(t_table[2], t_table[0]);
        }
        asm_code += get_reg_restore(t_table[1]);
        bool          is_ordinal_type = getResultType()->getType() == ordinal;
        BasicTypeKind basic_type      = getResultType()->getBasicAttrNode()->getType();
        BasicTypeKind op1_type        = op1->getResultType()->getBasicAttrNode()->getType();
        // TODO: Basic Type for Ordinal
        switch (eval_type) {
            case EK_Add: {
                if (basic_type == integer) {
                    asm_code += get_integer_calc("add", false);
                }
                break;
            }
            case EK_Sub: {
                if (basic_type == integer) {
                    asm_code += get_integer_calc("sub", false);
                }
                break;
            }
            case EK_Mul: {
                if (basic_type == integer) {
                    asm_code += get_integer_calc("mul", false);
                }
                break;
            }
            case EK_Div: {
                if (basic_type == integer) {
                    asm_code += get_integer_calc("div", false);
                }
                break;
            }
            case EK_Mod: {
                if (basic_type == integer) {
                    asm_code += get_integer_calc("mod", false);
                }
                break;
            }
            case EK_Eq: {
                if (basic_type == boolean && op1_type == integer) {
                    asm_code += get_integer_calc("cmp_eq", false);
                }
                break;
            }
            case EK_Ne: {
                if (basic_type == boolean && op1_type == integer) {
                    asm_code += get_integer_calc("cmp_ne", false);
                }
                break;
            }
            case EK_Lt: {
                if (basic_type == boolean && op1_type == integer) {
                    asm_code += get_integer_calc("cmp_lt", false);
                }
                break;
            }
            case EK_Gt: {
                if (basic_type == boolean && op1_type == integer) {
                    asm_code += get_integer_calc("cmp_gt", false);
                }
                break;
            }
            case EK_Le: {
                if (basic_type == boolean && op1_type == integer) {
                    asm_code += get_integer_calc("cmp_le", false);
                }
                break;
            }
            case EK_Ge: {
                if (basic_type == boolean && op1_type == integer) {
                    asm_code += get_integer_calc("cmp_ge", false);
                }
                break;
            }
            case EK_Not: {
                if (basic_type == integer) {
                    asm_code += get_integer_calc("not", true);
                }
                break;
            }
            case EK_And: {
                if (basic_type == integer) {
                    asm_code += get_integer_calc("and", true);
                }
                break;
            }
            case EK_Or: {
                if (basic_type == integer) {
                    asm_code += get_integer_calc("or", true);
                }
                break;
            }
            case EK_Xor: {
                if (basic_type == integer) {
                    asm_code += get_integer_calc("xor", true);
                }
                break;
            }
            case EK_Shl: {
                if (basic_type == integer) {
                    asm_code += get_integer_calc("shl", true);
                }
                break;
            }
            case EK_Shr: {
                if (basic_type == integer) {
                    asm_code += get_integer_calc("shr", true);
                }
                break;
            }
        }
        // TODO: For ordinal type, check boundary
    } else if (node_type == el_literal) {
        asm_code += literal_attr->genAsmCode();
    } else if (node_type == el_var_access) {
        asm_code += var_access_attr->genAsmCode(false);
    } else if (node_type == el_id) {
        asm_code += id_attr->genAsmCode(false);
    } else if (node_type == el_fun_call) {
        // TODO
    }
    return asm_code;
}

// Data to write is in t2
std::string ExprNode::genAsmCodeLHS() {
    if (error_handler.getErrorCount() != 0) return "";
    if (node_type == el_id) {
        return id_attr->genAsmCode(true);
    } else if (node_type == el_var_access) {
        return var_access_attr->genAsmCode(true);
    }
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

int ExprListNode::getLineNumber() {
    return line_no;
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
    if (type->getType() != rhs.type->getType()) return type->getType() < rhs.type->getType();
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
    switch (type->getType()) {
        case boolean: return result + to_string(bval);
        case integer: return result + to_string(ival);
        case real: return result + to_string(dval);
        case character: return result + to_string(cval);
    }
    return result;
}

std::string LiteralNode::genVizCode(int run) {
    return vizNode(uid, getNodeInfo());
}

TypeAttrNode *LiteralNode::getResultType() {
    return new TypeAttrNode(type);
}

std::string LiteralNode::genAsmCode() {
    uint32_t    val;
    std::string asm_code = "";
    switch (type->getType()) {
        case boolean: {
            val = (bval == true) ? 1 : 0;
            break;
        }
        case integer: {
            val = ival;
            break;
        }
        case real: {
            // TODO: Single Floating-Point to IEEE754 format
            *(float *)(&val) = dval;
            break;
        }
        case character: {
            val = cval;
            break;
        }
    }
    asm_code += get_load_imm(val);
    return asm_code;
}

int LiteralNode::diff(LiteralNode *rhs) {
    if (is_nil || rhs->is_nil) throw ExpressionTypeError(line_no, "real basic type", "pointer");
    if (type->getType() != rhs->type->getType())
        throw ExpressionTypeError(line_no, type->getTypeString(), rhs->type->getTypeString());
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

VarAccessNode::VarAccessNode(VarAccessType t, ExprNode *h, ExprListNode *i_l, ExprNode *m)
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

std::string VarAccessNode::genAsmCode(bool access_write) {  // TODO
    std::string asm_code = "";
    switch (type) {
        case va_pointer: {
            break;
        }
        case va_array: {
            break;
        }
        case va_record: {
            break;
        }
    }
    return asm_code;
}

TypeAttrNode *VarAccessNode::getResultType() {
    TypeAttrNode *type_attr = nullptr;
    if (host->getNodeType() == el_literal) {
        VarDefNode *def = symbol_table.findVarSymbol(host->getIdNode()->getName());
        if (def == nullptr) throw UndefineError(line_no, host->getIdNode()->getName());
        type_attr = def->getType();
    } else
        type_attr = host->getResultType();
    switch (type) {
        case va_pointer: return res_type = type_attr;
        case va_array: {
            ArrayAttrNode *array_attr = type_attr->getStructAttr()->getArrayAttr();
            try {
                array_attr->testIndexType(index_list);
            } catch (IndexDimensionError &e) {
                throw e;
            } catch (IndexTypeError &e) {
                throw e;
            }
            return res_type = array_attr->getElementType();
        }
        case va_record: {
            VarDefNode *var_def = type_attr->getStructAttr()->getRecordAttr()->getVarDef(
                member->getIdNode()->getName());
            if (var_def == nullptr)
                throw MemberNotFoundError(line_no,
                                          host->getResultType()->getTypeString(),
                                          member->getIdNode()->getName());
            return res_type = var_def->getType();
        }
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
    ConstDefNode *result = symbol_table.findConstSymbol(name);
    if (result == nullptr) throw UndefineError(line_no, name);
    return result;
}

TypeAttrNode *IdNode::getType() {
    TypeAttrNode *result = symbol_table.findTypeSymbol(name);
    if (result == nullptr) throw UndefineError(line_no, name);
    return result;
}

std::string IdNode::getNodeInfo() {
    return "IdNode\n" + name;
}

std::string IdNode::genVizCode(int run) {
    return vizNode(uid, getNodeInfo());
}

TypeAttrNode *IdNode::getResultType() {
    VarDefNode *var_def = symbol_table.findVarSymbol(name);
    if (var_def != nullptr) return res_type = var_def->getType();
    ConstDefNode *const_def = symbol_table.findConstSymbol(name);
    if (const_def != nullptr) return res_type = const_def->getExpr()->getResultType();
    throw UndefineError(line_no, name);
}

std::string IdNode::genAsmCode(bool access_write) {
    std::string   asm_code = "";
    int           var_level;
    VarDefNode   *var_node   = symbol_table.findVarSymbol(name, &var_level);
    ConstDefNode *const_node = symbol_table.findConstSymbol(name);
    if (var_node != nullptr) {
        TypeAttrNode *var_type_node = var_node->getType();
        TypeKind      var_type      = var_type_node->getType();
        if (var_type == basic || var_type == ordinal) {
            if (var_level == 0) {  // Global Variable
                asm_code += get_var_access(name,
                                           var_type_node->getOffset(),
                                           var_type_node->getLength(),
                                           access_write,
                                           true);
            } else if (var_level == symbol_table.getLevel()) {
                // TODO: Local Variable
            } else {
                // TODO: non-local & non-global variable
            }
        }
    } else if (const_node != nullptr) {
        if (!access_write) {
            asm_code += const_node->getExpr()->genAsmCodeRHS();
        }
    }
    return asm_code;
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

bool FuncNode::testArgType() {
    FuncDefNode *func = symbol_table.findFuncSymbol(func_name);
    if (func == nullptr) throw UndefineError(line_no, func_name);
    try {
        func->testArgType(arg_list);
    } catch (ArgumentNumberError &e) {
        throw e;
    } catch (ArgumentTypeError &e) {
        throw e;
    }
    return true;
}

TypeAttrNode *FuncNode::getResultType() {
    FuncDefNode *func = symbol_table.findFuncSymbol(func_name);
    if (func == nullptr) throw UndefineError(line_no, func_name);
    return res_type = func->getRetValType();
}