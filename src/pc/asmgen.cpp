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

const uint8_t t_table[7]  = {5, 6, 7, 28, 29, 30, 31};
const uint8_t s_table[12] = {8, 9, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};

bool     in_code_segment;
bool     in_data_segment;
FILE    *asm_file          = NULL;
uint32_t internal_label_id = (uint32_t)((int32_t)-1);

const map<string, string> internal_msg = {
    {"msg_div_zero", "illegal: divided by zero"},
    {"msg_signed_overflow", "illegal: signed arithmetic overflow"},
    {"msg_unsigned_overflow", "illegal: unsigned arithmetic overflow"},
    {"msg_out_of_bounds", "illegal: ordinal variable out of bounds"}};

void init_asm(uint32_t init_stack_top) {
    fprintf(asm_file, ".data\n");
    map<string, string>::const_iterator p = internal_msg.begin();
    while (p != internal_msg.end()) {
        fprintf(asm_file, "_%s:\n", p->first.data());
        fprintf(asm_file, "\t.asciiz \"%s\"\n", p->second.data());
        fprintf(asm_file, "\t.align 4\n");
        p++;
    }
    fprintf(asm_file, ".text\n");
    fprintf(asm_file, "init:\n");
    int32_t low_12_bits = init_stack_top & 0xFFF;
    if (low_12_bits & 0x800) low_12_bits |= ~0xFFF;
    int32_t high_20_bits = (init_stack_top - low_12_bits) >> 12;
    fprintf(asm_file, "\tlui sp, %d\n", high_20_bits);
    fprintf(asm_file, "\taddi sp, sp, %d\n", low_12_bits);
}

// Use Static Register Allocation Policy:

// The Most Complex Expr Calc Step:
// RAM_VAR0 = RAM_VAR1 Basic_OP RAM_VAR2
// 1 - RAM_VAR1->t1
// 2 - RAM_VAR2->t2
// 3 - t0 = t1 BASIC_OP t2
// 4 - t0 -> RAM_VAR3

// RAM access is optional, but we always have:
// for binary operator: t0 = t1 atom_op t2
// for unary operator: t0 = atom_op t1

// For all assembly snippet,they use t3-t6 for intermediate result
// When call funcs here to get code snippet,
// use s1-s11 register or stack to backup your t3-t6 register

/********************* `Atom` Expression Calculation ********************/
// `Atom` Calculation is for Integer
// All calc will be checked strictly for overflow

// Only break content in t3
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

// Only break content in t0
string get_load_imm(int32_t imm) {
    string  res         = "";
    int32_t low_12_bits = imm & 0xFFF;
    if (low_12_bits & 0x800) low_12_bits |= ~0xFFF;
    int32_t high_20_bits = (imm - low_12_bits) >> 12;
    res += "\tlui t0, " + to_string(high_20_bits) + "\n";
    res += "\taddi t0, t0, " + to_string(low_12_bits) + "\n";
    return res;
}

string get_integer_calc(string operation, bool is_unsigned) {
    string res = "";
    if (operation == "not") {
        operation = "xor";
        res += get_load_imm(0xFFFFFFFF);
        res += get_reg_xchg(t_table[2], t_table[0]);
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
            res += "\tblt t0, x0, _lbl_" + to_string(internal_label_id) + "\n";
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
        res += get_reg_xchg(t_table[1], t_table[0]);
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

/************************** Memory Load-Store **************************/
// Always load memory cell to t0, and write t2 to memory cell
// Memory address is in t1

// size: 1, 2 or 4, and other value is illegal
// dir = false: load, from mem to reg(t0)
// dir = true: write, from reg(t1) to mem
// is_signed: only for read operation

// Will only break t0 register
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

string get_var_access(int32_t var_pos, int32_t offset, uint8_t size, bool dir, bool is_signed) {
    string res = "";
    res += get_load_imm(var_pos + offset);
    res += "\tsub t1, fp, t0\n";  // local variable field in AR is used reversely
    res += get_mem_access(size, dir, is_signed);
    return res;
}

string get_var_access(string name, int32_t offset, uint8_t size, bool dir, bool is_signed) {
    string res = "";
    res += get_load_imm(offset);
    res += "\tla t1, " + name + "\n";
    res += "\tadd t1, t0, t1\n";
    res += get_mem_access(size, dir, is_signed);
    return res;
}

// Stack pointer is always and pre-decremented and post-incremented
string get_reg_save(void) {
    string res = "";
    res += "\taddi sp, sp, -4\n";
    res += "\tsw t1, 0(sp)\n";
    return res;
}

string get_reg_restore(void) {
    string res = "";
    res += "\tlw t0, 0(sp)\n";
    res += "\taddi sp, sp, 4\n";
    return res;
}

// Copy func/proc perameters before func/proc call
// t0: temp regs for single load and store
// t1: src base addr
// t2: dst base addr
// t3: byte counter

string get_mem_copy(void) {
    string res = "";
    res += "_lbl_" + to_string(internal_label_id + 2) + ":\n";
    res += "\tbeq t3, x0, _lbl_" + to_string(internal_label_id + 1) + "\n";
    res += "\tlw t0, 0(t1)\n";
    res += "\tsw t0, 0(t2)\n";
    res += "\taddi t1, t1, 1\n";
    res += "\taddi t2, t2, 1\n";
    res += "\taddi t3, t3, -1\n";
    res += "\tbeq x0, x0, _lbl_" + to_string(internal_label_id + 2) + "\n";
    res += "_lbl_" + to_string(internal_label_id + 1) + ":\n";
    internal_label_id += 2;
    return res;
}

string get_params_copy(uint32_t dst_stk_pos, uint32_t src_stk_pos, uint32_t length) {
    string res = "";
    res += get_load_imm(length);
    res += "\tadd t3, t0, x0\n";
    res += get_load_imm(dst_stk_pos);
    res += "\tadd t2, t0, sp\n";
    res += get_load_imm(src_stk_pos);
    res += "\tadd t1, t0, sp\n";
    res += get_mem_copy();
    res += "\tsub sp, sp, t3\n";
    return res;
}

string get_params_copy(uint32_t dst_stk_pos, string src_name, uint32_t length) {
    string res = "";
    res += "\tla t1, " + src_name + "\n";
    res += get_load_imm(length);
    res += "\tadd t3, t0, x0\n";
    res += get_load_imm(dst_stk_pos);
    res += "\tadd t2, t0, sp\n";
    res += get_mem_copy();
    res += "\tsub sp, sp, t3\n";
    return res;
}

/********************** Function or Procedure Call **********************/

// func/proc's perem/retval is always in stack
// func/proc's Activation Record:
// ------------------------------ (High Address)
// |          List of           | {Leftest in arg list is at lowest addr}
// |         Parameters         |
// |----------------------------|
// |        Return Value        | {4 bytes, only for func, BasicRealType}
// |----------------------------|
// |         OLD ra(x1)         | {4 bytes (one word)}
// |----------------------------|
// |         OLD s1-s11         | {4 * 11 bytes (11 words)}
// |----------------------------|
// |        OLD fp(x8/s0)       | {4 bytes (one word), as indirect access link}
// |----------------------------| <- fp(x8/s0) HERE!
// |       Local Variables      | {LEN varies with func as a const}
// |----------------------------| <- sp(x2) HERE!
// | (Saved REGs before a call) | {Saved temporary registers}
// |----------------------------|
// |       Free Stack Space     | {For inner func/proc's AR}
// ------------------------------ (Low Address)

// 1 - DO NOT USE s0(fp) for other purpose during a call!
// 2 - sp is never modified to another const after initialized
// 3 - To support access of non-local & non-global vars, use "OLD fp"
// or access link to get the start addr of caller's local var

// External Visable Function for Assembly Code Generation

string get_func_def(string name, uint32_t args_len, uint32_t local_vars_len, string func_body) {
    string res = "";
    res += name + ":\n";
    res += "\taddi sp, sp, -4\n";
    res += "\tsw ra, 0(sp)\n";
    res += "\taddi sp, sp, -44\n";
    for (int i = 1; i <= 11; i++) {
        res += "\tsw x" + to_string(s_table[i]) + ", " + to_string((i - 1) * 4) + "(sp)\n";
    }
    res += "\taddi sp, sp, -4\n";
    res += "\tsw fp, 0(sp)\n";
    res += "\tadd fp, sp, x0\n";
    res += get_load_imm(local_vars_len);
    res += "\tsub sp, sp, t0\n";
    res += func_body;
    res += get_load_imm(local_vars_len);
    res += "\tadd sp, sp, t0\n";
    res += "\tlw fp, 0(sp)\n";
    res += "\taddi sp, sp, 4\n";
    for (int i = 1; i <= 11; i++) {
        res += "\tlw x" + to_string(s_table[i]) + ", " + to_string((i - 1) * 4) + "(sp)\n";
    }
    res += "\taddi sp, sp, 44\n";
    res += "\tlw ra, 0(sp)\n";
    res += "\taddi sp, sp, 4\n";
    res += "\tjalr x0, 0(ra)\n";
    return res;
}

string get_func_call(string name, string parem_copy, bool has_retval) {
    string res = "";
    res += parem_copy;
    if (has_retval) res += "\taddi sp, sp, -4\n";
    res += "\tcall " + name + "\n";
    return res;
}

// Clean params and retval after a call
string get_func_cleanup(uint32_t args_len, bool has_retval) {
    string res = "";
    if (!has_retval)
        res += get_load_imm(args_len);
    else
        res += get_load_imm(args_len + 4);
    res += "\tadd sp, sp, t0\n";
    return res;
}

std::string get_retval_read(uint8_t size, bool is_unsigned) {  // For caller to get return value
    string res = "";
    res += "\taddi t1, fp, 52\n";
    res += get_mem_access(size, false, is_unsigned);
    return res;
}

std::string get_retval_write(uint8_t size, bool is_unsigned) {  // For callee to put return value
    string res = "";
    // 52 = 4 * (1 (old ra) + 11 (old s1-s11) + 1 (old fp))
    res += "\taddi t1, fp, 52\n";
    res += get_mem_access(size, true, is_unsigned);
    return res;
}

string get_param_access(
    uint32_t param_pos, uint32_t offset, bool has_retval, uint8_t size, bool dir, bool is_signed) {
    string res = "";
    res += "\taddi t1, fp, 52\n";
    if (has_retval) res += "\taddi t1, t1, 4\n";
    res += get_load_imm(param_pos + offset);
    res += "\tadd t1, t1, t0\n";
    res += get_mem_access(size, dir, is_signed);
    return res;
}

// fp of target level of AR is finally in t0
string get_access_link(uint32_t jmp_level) {
    string res = "";
    res += get_load_imm(jmp_level);
    res += "\tadd t1, t0, x0\n";  // t1 = level that needs to jump
    res += "\tadd t0, x0, fp\n";  // t0 = current level's fp
    res += "_lbl_" + to_string(++internal_label_id) + ":\n";
    res += "\tlw t0, 0(t0)\n";  // x0 = old fp
    res += "\taddi t1, t1, -1\n";
    res += "bne t1, x0, " + to_string(internal_label_id) + "\n";
    return res;
}

bool start_asm(std::string filename, uint32_t init_stack_top) {
    if (asm_file != NULL) return false;
    in_code_segment = in_data_segment = false;
    internal_label_id                 = -1;
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

// seg_sel: false-code segment, true-data segment
bool write_segment(string snippet, bool data_seg) {
    if (asm_file == NULL) {
        printf("assembly file is not created yet\n");
        return false;
    }
    if (data_seg && !in_data_segment) {
        fprintf(asm_file, ".data\n");
        in_data_segment = true;
        in_code_segment = false;
    } else if (!data_seg && !in_code_segment) {
        fprintf(asm_file, ".text\n");
        in_data_segment = false;
        in_code_segment = true;
    } else
        return false;
    fprintf(asm_file, "%s", snippet.data());
    return true;
}

string get_define_global(string name, vector<uint8_t> field_size, vector<uint8_t> field_rep) {
    string res = "";
    if (field_size.size() != field_rep.size()) return "";
    res += "\t.align 4\n";
    res += name + ":\n";
    for (int i = 0; i < field_size.size(); i++) {
        if (field_size[i] == 1) {
            res += "\t.byte ";
        } else if (field_size[i] == 4) {
            res += "\t.word ";
        } else
            return "";
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

// Ordinal data for checking is in t1 register
string get_ordinal_bound_check(uint32_t lower_bound, uint32_t upper_bound) {
    string res = "";
    res += get_load_imm(lower_bound);
    res += "\tadd t3, t0, x0\n";
    res += get_load_imm(upper_bound);
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
string get_stmt_cond(string calc_expr, string then_stmts, string else_stmts) {
    string res = "";
    res += calc_expr;
    res += "\tbne t1, x0, _lbl_" + to_string(internal_label_id + 1) + "\n";
    res += else_stmts;
    res += "\tbeq x0, x0, _lbl_" + to_string(internal_label_id + 2) + "\n";
    res += "_lbl_" + to_string(internal_label_id + 1) + ":\n";
    res += then_stmts;
    res += "_lbl_" + to_string(internal_label_id + 2) + ":\n";
    internal_label_id += 2;
    return res;
}

string get_stmt_while(string calc_expr, string stmts) {
    string res = "";
    res += "_lbl_" + to_string(internal_label_id + 1) + ":\n";
    res += calc_expr;
    res += "\tbeq t1, x0, _lbl_" + to_string(internal_label_id + 2) + "\n";
    res += stmts;
    res += "\tbeq x0, x0, _lbl_" + to_string(internal_label_id + 1) + "\n";
    res += "_lbl_" + to_string(internal_label_id + 2) + ":\n";
    internal_label_id += 2;
    return res;
}

string get_stmt_until(string calc_expr, string stmts) {
    string res = "";
    res += "_lbl_" + to_string(++internal_label_id) + ":\n";
    res += stmts;
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
