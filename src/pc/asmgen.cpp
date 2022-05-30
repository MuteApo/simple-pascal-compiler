#include <cstdio>
#include <map>
#include <string>
#include <vector>

using namespace std;

#include "include/asmgen.hpp"

#define ECALL_EXIT 0
#define ECALL_PRT_CHAR 1
#define ECALL_PRT_STR 2
#define ECALL_PRT_INT 3
#define ECALL_READ_CHAR 4
#define ECALL_READ_STR 5
#define ECALL_READ_INT 6

#define GLOBAL_INIT_VAL (0)

bool     in_code_segment;
bool     in_data_segment;
string   asm_filename      = "";
FILE    *asm_file          = NULL;
uint32_t internal_label_id = (uint32_t)((int32_t)-1);

const map<string, string> internal_msg = {
    {"msg_div_zero", "[divide by zero]\\n"},
    {"msg_signed_overflow", "[overflow: on addition or subtraction]\\n"},
    {"msg_unsigned_overflow", "[overflow: on multiplication]\\n"},
    {"msg_out_of_bounds", "[ordinal variable out of bounds]\\n"},
    {"msg_boolean_true", "True"},
    {"msg_boolean_false", "False"}};

/** Basic Assembly Code Generation Function **/

void init_asm(uint32_t init_stack_top) {
    fprintf(asm_file, ".data\n");
    map<string, string>::const_iterator p = internal_msg.begin();
    while (p != internal_msg.end()) {
        fprintf(asm_file, "_%s:\n", p->first.data());
        fprintf(asm_file, "\t.align 4\n");
        fprintf(asm_file, "\t.asciiz \"%s\"\n", p->second.data());
        p++;
    }
    fprintf(asm_file, ".text\n");
    fprintf(asm_file, "init:\n");
    string sp_set_inst = "\tli sp, " + to_string(init_stack_top) + "\n";
    fprintf(asm_file, "%s", sp_set_inst.data());
    in_code_segment = true;
    in_data_segment = false;
}

bool start_asm(std::string filename, uint32_t init_stack_top) {
    if (asm_file != NULL) return false;
    in_code_segment = in_data_segment = false;
    internal_label_id                 = -1;
    asm_filename                      = filename;
    asm_file                          = fopen(filename.data(), "w+");
    if (asm_file == NULL) printf("unable to create file %s\n", filename.data());
    if (asm_file != NULL) init_asm(init_stack_top);
    return asm_file != NULL;
}

bool finish_asm(void) {
    if (asm_file != NULL) {
        fclose(asm_file);
        return true;
    } else
        return false;
}

bool remove_asm(void) {
    if (asm_file != NULL) {
        fclose(asm_file);
        remove(asm_filename.data());
        return true;
    } else
        return false;
}

bool write_segment(string snippet, bool data_seg) {
    if (asm_file == NULL) {
        printf("assembly file is not created yet\n");
        return false;
    } else if (snippet == "") {
        return true;
    }
    if (data_seg && !in_data_segment) {
        fprintf(asm_file, ".data\n");
        in_data_segment = true;
        in_code_segment = false;
    } else if (!data_seg && !in_code_segment) {
        fprintf(asm_file, ".text\n");
        in_data_segment = false;
        in_code_segment = true;
    }
    fprintf(asm_file, "%s", snippet.data());
    return true;
}

string get_define_global(string name, vector<uint8_t> field_size, vector<uint32_t> field_rep) {
    string res = "";
    if (field_size.size() != field_rep.size()) return "";
    res += name + ":\n";
    res += "\t.align 4\n";
    for (int i = 0; i < field_size.size(); i++) {
        if (field_size[i] == 1) {
            res += "\t.byte ";
        } else if (field_size[i] == 4) {
            res += "\t.word ";
        } else {
            return "";
        }
        for (int j = 0; j < field_rep[i]; j++) {
            if (j == 0)
                res += to_string(GLOBAL_INIT_VAL);
            else
                res += " " + to_string(GLOBAL_INIT_VAL);
        }
        res += "\n";
    }
    res += "\n";
    return res;
}

/** Register Operation **/

string get_load_imm(int32_t imm) {
    string res = "";
    res += "\tli t0, " + to_string(imm) + "\n";
    return res;
}

// Will break content in t3
string get_reg_xchg(uint8_t dst_reg, uint8_t src_reg) {
    if (dst_reg >= 32 || src_reg >= 32) {
        if (dst_reg >= 32)
            printf("illegal register x%d\n", dst_reg);
        else
            printf("illegal register x%d\n", src_reg);
        return "";
    }
    string res = "\taddi t3, x" + to_string(dst_reg) + ", 0\n";
    res += "\tadd x" + to_string(dst_reg) + ", x" + to_string(src_reg) + ", x0\n";
    res += "\tadd x" + to_string(src_reg) + ", t3, x0\n";
    return res;
}

// Stack pointer is always and pre-decremented and post-incremented
string get_reg_save(uint8_t reg) {
    string res = "";
    res += "\taddi sp, sp, -4\n";
    res += "\tsw x" + to_string(reg) + ", 0(sp)\n";
    return res;
}

string get_reg_restore(uint8_t reg) {
    string res = "";
    res += "\tlw x" + to_string(reg) + ", 0(sp)\n";
    res += "\taddi sp, sp, 4\n";
    return res;
}

/** ALU Operation with Register **/

string get_integer_calc(string operation, bool is_unsigned) {
    string res = "";
    if (operation == "not") {
        operation = "xor";
        res += "\tli t2, 0xFFFFFFFF\n";
    }
    if (operation == "add" || operation == "sub") {
        if (operation == "add")
            res += "\tadd t0, t1, t2\n";
        else
            res += "\tsub t0, t1, t2\n";
        if (is_unsigned) {
            // add: t0 < t1 + t2, overflow
            // sub: t1 < t2, overflow
            if (operation == "add")
                res += "\tbgeu t0, t1, _lbl_" + to_string(++internal_label_id) + "\n";
            else
                res += "\tbgeu t1 ,t2, _lbl_" + to_string(++internal_label_id) + "\n";
            res += "\taddi a0, x0, " + to_string(ECALL_PRT_STR) + "\n";
            res += "\tla a1, _msg_unsigned_overflow\n";
            res += "\tecall\n";
            res += "\taddi a0, x0, " + to_string(ECALL_EXIT) + "\n";
            res += "\tecall\n";
            res += "_lbl_" + to_string(internal_label_id) + ":\n";
        } else {
            // For add of signed int, if src2 < 0,
            // then res must less than src1, else overflow
            // t3 = t2 < 0
            // add: t4 = t0 < t1
            // sub: t4 = t1 < t0
            res += "\tslti t3, t2, 0\n";
            if (operation == "add")
                res += "\tslt t4, t0, t1\n";
            else
                res += "\tslt t4, t1, t0\n";
            res += "\tbeq t3, t4, _lbl_" + to_string(++internal_label_id) + "\n";
            res += "\taddi a0, x0, " + to_string(ECALL_PRT_STR) + "\n";
            res += "\tla a1, _msg_signed_overflow\n";
            res += "\tecall\n";
            res += "\taddi a0, x0, " + to_string(ECALL_EXIT) + "\n";
            res += "\tecall\n";
            res += "_lbl_" + to_string(internal_label_id) + ":\n";
        }
    } else if (operation == "shr") {
        if (!is_unsigned)
            res += "\tsra t0, t1, t2\n";
        else
            res += "\tsrl t0, t1, t2\n";
    } else if (operation == "shl") {
        res += "\tsll t0, t1, t2\n";
    } else if (operation == "and") {
        res += "\tand t0, t1, t2\n";
    } else if (operation == "or") {
        res += "\tor t0, t1, t2\n";
    } else if (operation == "xor") {
        res += "\txor t0, t1, t2\n";
    } else if (operation == "cmp_lt") {
        if (!is_unsigned)
            res += "\tslt t0, t1, t2\n";
        else
            res += "\tsltu t0, t1, t2\n";
    } else if (operation == "mul" || operation == "div" || operation == "mod") {
        if (is_unsigned) {
            if (operation == "mul") {
                res += "\taddi t0, x0, 0\n";
                res += "_lbl_" + to_string(internal_label_id + 3) + ":\n";
                res += "\tbeq t2, x0, _lbl_" + to_string(internal_label_id + 1) + "\n";
                res += "\taddi t3, t0, 0\n";
                res += "\tadd t0, t0, t1\n";
                res += "\tbgeu t0, t3,  _lbl_" + to_string(internal_label_id + 2) + "\n";
                res += "\taddi a0, x0, " + to_string(ECALL_PRT_STR) + "\n";
                res += "\tla a1, _msg_unsigned_overflow\n";
                res += "\tecall\n";
                res += "\taddi a0, x0, " + to_string(ECALL_EXIT) + "\n";
                res += "\tecall\n";
                res += "_lbl_" + to_string(internal_label_id + 2) + ":\n";
                res += "\taddi t2, t2, -1\n";
                res += "\tbeq x0, x0, _lbl_" + to_string(internal_label_id + 3) + "\n";
                res += "_lbl_" + to_string(internal_label_id + 1) + ":\n";
                internal_label_id += 3;
            } else if (operation == "div" || operation == "mod") {
                res += "\tbne t2, x0, _lbl_" + to_string(++internal_label_id) + "\n";
                res += "\taddi a0, x0, " + to_string(ECALL_PRT_STR) + "\n";
                res += "\tla a1, _msg_div_zero\n";
                res += "\tecall\n";
                res += "\taddi a0, x0, " + to_string(ECALL_EXIT) + "\n";
                res += "\tecall\n";
                res += "_lbl_" + to_string(internal_label_id) + ":\n";
                res += "\taddi t0, x0, 0\n";
                res += "\tbge x0, t1, 16\n";
                res += "\tsub t1, t1, t2\n";
                res += "\taddi t0, t0, 1\n";
                res += "\tbeq x0, x0, -12\n";
                res += "\tbeq t1, x0, 12\n";
                res += "\tadd t1, t1, t2\n";  // Compensite for over-sub
                res += "\taddi t0, t0, -1\n";
                if (operation == "mod")  // Return remainder rather than quotient
                    res += "\taddi t0, t1, 0\n";
            }
        } else {
            if (operation != "mod") {
                res += "\tslti t3, t1, 0\n";
                res += "\tslti t4, t2, 0\n";
                res += "\txor t5, t3, t4\n";  // top of stack = sign of result
                res += "\taddi sp, sp, -4\n";
                res += "\tsw t5, 0(sp)\n";
            } else {
                res += "\tslti t3, t1, 0\n";
                res += "\tslti t4, t2, 0\n";
                res += "\tslti t5, t1, 0\n";
                res += "\taddi sp, sp, -4\n";
                res += "\tsw t5, 0(sp)\n";
            }
            res += "\tbeq t3, x0, 8\n";
            res += "\tsub t1, x0, t1\n";  // t1 = -t1, never overflow
            res += "\tbeq t4, x0, 8\n";
            res += "\tsub t2, x0, t2\n";               // t2 = -t2
            res += get_integer_calc(operation, true);  // do unsigned mul/div/mod
            res += "\tlw t5, 0(sp)\n";
            res += "\taddi sp, sp, 4\n";
            res += "\tbeq t5, x0, _lbl_" + to_string(++internal_label_id) + "\n";
            res += "\tsub t0, x0, t0\n";
            res += "\tbge x0, t0, _lbl_" + to_string(internal_label_id) + "\n";
            res += "\taddi a0, x0, " + to_string(ECALL_PRT_STR) + "\n";
            res += "\tla a1, _msg_signed_overflow\n";
            res += "\tecall\n";
            res += "\taddi a0, x0, " + to_string(ECALL_EXIT) + "\n";
            res += "\tecall\n";
            res += "_lbl_" + to_string(internal_label_id) + ":\n";
        }
    } else if (operation == "neg") {  // No overflow check
        res += "\tsub t0, x0, t1\n";
    } else if (operation == "cmp_le") {
        res += get_reg_xchg(t_table[1], t_table[2]);
        res += get_integer_calc("cmp_ge", is_unsigned);
    } else if (operation == "cmp_gt") {
        res += get_reg_xchg(t_table[1], t_table[2]);
        res += get_integer_calc("cmp_lt", is_unsigned);
    } else if (operation == "cmp_ge") {
        if (!is_unsigned)
            res += "\tbge t1, t2, 12\n";
        else
            res += "\tbgeu t1, t2, 12\n";
        res += "\taddi t0, x0, 0\n";
        res += "\tbeq x0, x0, 8\n";
        res += "\taddi t0, x0, 1\n";
    } else if (operation == "cmp_eq") {
        res += "\tbeq t1, t2, 12\n";
        res += "\taddi t0, x0, 0\n";
        res += "\tbeq x0, x0, 8\n";
        res += "\taddi t0, x0, 1\n";
    } else if (operation == "cmp_ne") {
        res += "\tbne t1, t2, 12\n";
        res += "\taddi t0, x0, 0\n";
        res += "\tbeq x0, x0, 8\n";
        res += "\taddi t0, x0, 1\n";
    } else {
        printf("%s is not a basic expr\n", operation.data());
    }
    return res;
}

/** Memory Operation **/

string get_local_addr(int32_t local_offset) {
    string res = "";
    res += "\tli t0, " + to_string(local_offset) + "\n";
    // local variable field in AR is used reversely
    res += "\tsub t0, fp, t0\n";
    return res;
}

string get_global_addr(string name) {
    string res = "";
    res += "\tla t0, " + name + "\n";
    return res;
}

// Will broke content in t1
string get_param_addr(uint32_t param_offset, bool has_retval) {
    string res = "";
    res += "\taddi t0, fp, 56\n";
    if (has_retval) res += "\taddi t0, t0, 4\n";
    res += "\tli t1, " + to_string(param_offset) + "\n";
    res += "\tadd t0, t1, t0\n";
    return res;
}

string get_mem_access(uint8_t size, bool dir, bool is_signed) {
    string res = "";
    if (size == 1) {
        if (!dir) {
            if (is_signed)
                res += "\tlb t0, 0(t1)\n";
            else
                res += "\tlbu t0, 0(t1)\n";
        } else {
            res += "\tsb t2, 0(t1)\n";
        }
    } else if (size == 2) {
        if (!dir) {
            if (is_signed)
                res += "\tlh t0, 0(t1)\n";
            else
                res += "\tlhu t0, 0(t1)\n";
        } else {
            res += "\tsh t2, 0(t1)\n";
        }
    } else if (size == 4) {
        if (!dir)
            res += "\tlw t0, 0(t1)\n";
        else
            res += "\tsw t2, 0(t1)\n";
    } else {
        printf("size %d is illegal in single memory access\n", size);
    }
    return res;
}

string get_mem_copy(uint32_t length) {
    string res = "";
    res += "\tli t3, " + to_string(length) + "\n";
    res += "_lbl_" + to_string(internal_label_id + 2) + ":\n";
    res += "\tbeq t3, x0, _lbl_" + to_string(internal_label_id + 1) + "\n";
    res += "\tlb t0, 0(t2)\n";
    res += "\tsb t0, 0(t1)\n";
    res += "\taddi t1, t1, 1\n";
    res += "\taddi t2, t2, 1\n";
    res += "\taddi t3, t3, -1\n";
    res += "\tbeq x0, x0, _lbl_" + to_string(internal_label_id + 2) + "\n";
    res += "_lbl_" + to_string(internal_label_id + 1) + ":\n";
    internal_label_id += 2;
    return res;
}

/** Function Defination and Call **/

string get_func_def(string name, uint32_t local_var_len, string func_body) {
    string res = "";
    res += name + ":\n";
    res += "\tsub a0, a1, a0\n";
    res += "\taddi a0, a0, 1\n";
    res += "\taddi sp, sp, -4\n";
    res += "\tsw a0, 0(sp)\n";  // Save indirect access link onto stack
    res += "\taddi sp, sp, -4\n";
    res += "\tsw ra, 0(sp)\n";  // Save return address
    res += "\taddi sp, sp, -44\n";
    for (int i = 1; i <= 11; i++) {
        res += "\tsw x" + to_string(s_table[i]) + ", " + to_string((i - 1) * 4) + "(sp)\n";
    }
    res += "\taddi sp, sp, -4\n";
    res += "\tsw fp, 0(sp)\n";    // Save control link
    res += "\tadd fp, sp, x0\n";  // fp = sp (new fp)
    res += "\tli t0, " + to_string(local_var_len) + "\n";
    res += "\tsub sp, sp, t0\n";
    res += func_body;
    res += "\tadd sp, fp, x0\n";  // Cleanup local variables
    res += "\tlw fp, 0(sp)\n";
    res += "\taddi sp, sp, 4\n";
    for (int i = 1; i <= 11; i++) {
        res += "\tlw x" + to_string(s_table[i]) + ", " + to_string((i - 1) * 4) + "(sp)\n";
    }
    res += "\taddi sp, sp, 44\n";
    res += "\tlw ra, 0(sp)\n";
    res += "\taddi sp, sp, 4\n";  // Cleanup return address
    res += "\taddi sp, sp, 4\n";  // Cleanup access link
    res += "\tjalr x0, 0(ra)\n";
    return res;
}

string get_param_copy(uint32_t length) {
    string res = "";
    res += "\tli t1, " + to_string(length) + "\n";
    res += "\tsub sp, sp, t1\n";
    res += "\tadd t1, sp, x0\n";
    res += get_mem_copy(length);
    return res;
}

string
get_func_call(string name, string param_copys, bool has_retval, uint32_t call_lv, uint32_t def_lv) {
    string res = "";
    res += param_copys;
    if (has_retval) res += "\taddi sp, sp, -4\n";
    res += "\tli a0, " + to_string(call_lv) + "\n";
    res += "\tli a1, " + to_string(def_lv) + "\n";
    res += "\tcall " + name + "\n";
    return res;
}

// For caller to get return value
std::string get_retval_read(bool is_unsigned) {
    string res = "";
    res += "\taddi t1, fp, 56\n";
    res += get_mem_access(4, false, is_unsigned);
    return res;
}

// For callee to put return value
std::string get_retval_write(bool is_unsigned) {
    string res = "";
    // 56 = 4 * (1 (indirect access link) + 1 (old ra) + 11 (old s1-s11) + 1 (old fp))
    res += "\taddi t1, fp, 56\n";
    res += get_mem_access(4, true, is_unsigned);
    return res;
}

// Clean params and retval after a call
string get_func_cleanup(uint32_t arg_len, bool has_retval) {
    string res = "";
    if (!has_retval)
        res += "\tli t0, " + to_string(arg_len) + "\n";
    else
        res += "\tli t0, " + to_string(arg_len + 4) + "\n";
    res += "\tadd sp, sp, t0\n";
    return res;
}

string get_fp_before(uint32_t level_to_jump) {
    string res = "";
    res += "\tli t1, " + to_string(level_to_jump) + "\n";
    res += "_lbl_" + to_string(++internal_label_id) + ":\n";
    res += "\tlw t0, 0(t0)\n";  // x0 = old fp
    res += "\taddi t1, t1, -1\n";
    res += "\tbne t1, x0, " + to_string(internal_label_id) + "\n";
    return res;
}

// fp of target level of AR is finally in t0
string get_access_link(void) {
    string res = "";
    res += "\tlw t1, 52(fp)\n";   // t1 = level that needs to jump
    res += "\tadd t0, x0, fp\n";  // t0 = current level's fp
    res += "_lbl_" + to_string(++internal_label_id) + ":\n";
    res += "\tlw t0, 0(t0)\n";  // x0 = old fp
    res += "\taddi t1, t1, -1\n";
    res += "bne t1, x0, " + to_string(internal_label_id) + "\n";
    return res;
}

/** Basic Statement and Dynamic Check **/

string get_bound_check(uint32_t lower_bound, uint32_t upper_bound) {
    string res = "";
    res += "\tli t3, " + to_string(lower_bound) + "\n";
    res += "\tli t4, " + to_string(upper_bound) + "\n";
    res += "\tadd t4, t0, x0\n";
    res += "\tslt t5, t1, t3\n";
    res += "\tsltu t6, t4, t1\n";
    res += "\tor t5, t5, t6\n";
    res += "\tbeq t5, x0, _lbl_" + to_string(++internal_label_id) + "\n";
    res += "\taddi a0, x0, " + to_string(ECALL_PRT_STR) + "\n";
    res += "\tla a1, _msg_out_of_bounds\n";
    res += "\tecall\n";
    res += "\taddi a0, x0, " + to_string(ECALL_EXIT) + "\n";
    res += "\tecall\n";
    res += "_lbl_" + to_string(internal_label_id) + ":\n";
    return res;
}

// BUG: stmts can not be too long
// calc_expr must store its result in t1, and saved temperary registers itself
string get_stmt_cond(string calc_expr, string then_stmt, string else_stmt) {
    string res = "";
    res += calc_expr;
    res += "\tbne t1, x0, _lbl_" + to_string(internal_label_id + 1) + "\n";
    res += else_stmt;
    res += "\tbeq x0, x0, _lbl_" + to_string(internal_label_id + 2) + "\n";
    res += "_lbl_" + to_string(internal_label_id + 1) + ":\n";
    res += then_stmt;
    res += "_lbl_" + to_string(internal_label_id + 2) + ":\n";
    internal_label_id += 2;
    return res;
}

string get_stmt_while(string calc_expr, string stmt) {
    string res = "";
    res += "_lbl_" + to_string(internal_label_id + 1) + ":\n";
    res += calc_expr;
    res += "\tbeq t1, x0, _lbl_" + to_string(internal_label_id + 2) + "\n";
    res += stmt;
    res += "\tbeq x0, x0, _lbl_" + to_string(internal_label_id + 1) + "\n";
    res += "_lbl_" + to_string(internal_label_id + 2) + ":\n";
    internal_label_id += 2;
    return res;
}

string get_stmt_until(string calc_expr, string stmt) {
    string res = "";
    res += "_lbl_" + to_string(++internal_label_id) + ":\n";
    res += stmt;
    res += calc_expr;
    res += "\tbeq t1, x0, _lbl_" + to_string(internal_label_id) + "\n";
    return res;
}

string get_read(string type) {
    string res = "";
    if (type == "char") {
        res += "\taddi a0, x0, " + to_string(ECALL_READ_CHAR) + "\n";
    } else if (type == "str_ptr") {
        res += "\taddi a0, x0, " + to_string(ECALL_READ_STR) + "\n";
        res += "\tadd a1, t1, x0\n";
    } else if (type == "int") {
        res += "\taddi a0, x0, " + to_string(ECALL_READ_INT) + "\n";
    } else
        return "";
    res += "\tecall\n";
    if (type == "char" || type == "int") res += "\taddi t0, a1, 0\n";
    return res;
}

string get_write(string type) {
    string res = "";
    res += "\tadd a1, t1, x0\n";
    if (type == "char") {
        res += "\taddi a0, x0, " + to_string(ECALL_PRT_CHAR) + "\n";
    } else if (type == "str_ptr") {
        res += "\taddi a0, x0, " + to_string(ECALL_PRT_STR) + "\n";
    } else if (type == "int") {
        res += "\taddi a0, x0, " + to_string(ECALL_PRT_INT) + "\n";
    } else if (type == "bool") {
        res += "\taddi a0, x0, " + to_string(ECALL_PRT_STR) + "\n";
        res += "\tla a1, _msg_boolean_true\n";
        res += "\tbne t1, x0, _lbl_" + to_string(++internal_label_id) + "\n";
        res += "\tla a1, _msg_boolean_false\n";
        res += "_lbl_" + to_string(internal_label_id) + ":\n";
    } else
        return "";
    res += "\tecall\n";
    return res;
}

string get_exit(void) {
    string res = "";
    res += "\taddi a0, x0, 0\n";
    res += "\tecall\n";
    return res;
}
