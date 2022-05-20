#include <cstdio>
#include <map>
#include <string>
#include <vector>

using namespace std;

#include "include/asmgen.hpp"

#define t0 (5)
#define t1 (6)
#define t2 (7)
#define s1 (9)

bool in_code_segment;
bool in_data_segment;
FILE *asm_file = NULL;
uint32_t internal_label_id = 0;

static map<string, string> internal_msg = {
    {"msg_div_zero", "illegal: divided by zero"},
    {"msg_signed_overflow", "illegal: signed arithmetic overflow"},
    {"msg_unsigned_overflow", "illegal: unsigned arithmetic overflow"},
    {"msg_out_of_bounds", "illegal: ordinal variable out of bounds"}};

void init_asm(uint32_t init_stack_top) {
    fprintf(asm_file, ".data\n");
    map<string, string>::iterator p = internal_msg.begin();
    while (p != internal_msg.end()) {
        fprintf(asm_file, "@%s:\n", p->first.data());
        fprintf(asm_file, "\t.asciiz \"%s\"\n", p->second.data());
        fprintf(asm_file, "\t.align 4\n");
        p++;
    }
    fprintf(asm_file, ".text\n");
    fprintf(asm_file, "init:\n");
    int32_t low_12_bits = init_stack_top & 0xFFF;
    if (low_12_bits & 0x800)
        low_12_bits |= ~0xFFF;
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
// Be careful to reuse t0-t6 when snippets referencing each other
// Use s1-s11 register or stack to backup your t3-t6 register

/********************* `Atom` Expression Calculation ********************/
// `Atom` Calculation is for Integer
// All calc will be checked strictly for overflow

string get_reg_xchg(uchar dst_reg, uchar src_reg) {
    if (dst_reg >= 32 || src_reg >= 32) {
        if (dst_reg >= 32)
            printf("illegal register x%d\n", dst_reg);
        else
            printf("illegal register x%d\n", src_reg);
        return "";
    }
    string res = "addi t3, x" + to_string(dst_reg) + ", 0\n";
    res += "add x" + to_string(dst_reg) + ", x" + to_string(src_reg) + ", x0\n";
    res += "add x" + to_string(src_reg) + ", t3, x0\n";
    return res;
}

string get_load_imm(int32_t imm) {
    string res = "";
    int32_t low_12_bits = imm & 0xFFF;
    if (low_12_bits & 0x800)
        low_12_bits |= ~0xFFF;
    int32_t high_20_bits = (imm - low_12_bits) >> 12;
    res += "lui t1, " + to_string(high_20_bits) + "\n";
    res += "addi t1, t1, " + to_string(low_12_bits) + "\n";
    return res;
}

string get_basic_calc(string operation, bool is_unsigned) {
    string res = "";
    if (operation == "not") {
        operation = "xor";
        res += get_reg_xchg(s1, t1);
        res += get_load_imm(0xFFFFFFFF);
        res += get_reg_xchg(t2, t1);
        res += get_reg_xchg(t1, s1);
    }
    if (operation == "add" || operation == "sub") {
        if (operation == "add")
            res += "add t0, t1, t2\n";
        else
            res += "sub t0, t1, t2\n";
        if (is_unsigned) {
            // add: t0 < t1 + t2, overflow
            // sub: t1 < t2, overflow
            if (operation == "add")
                res += "bgeu t0, t1, 28\n";
            else
                res += "bgeu t1 ,t2, 28\n";
            res += "addi a0, x0, 2\n";
            res += "la a1, @msg_unsigned_overflow\n";
            res += "ecall\n";
            res += "addi a0, x0, 0\n";
            res += "ecall\n";
        } else {
            // For add of signed int, if src2 < 0,
            // then res must less than src1, else overflow
            // t3 = t2 < 0
            // add: t4 = t0 < t1
            // sub: t4 = t1 < t0
            res += "slti t3, t2, 0\n";
            if (operation == "add")
                res += "slt t4, t0, t1\n";
            else
                res += "slt t4, t1, t0\n";
            res += "beq t3, t4, 28\n";
            res += "addi a0, x0, 2\n";
            res += "la a1, @msg_signed_overflow\n";
            res += "ecall\n";
            res += "addi a0, x0, 0\n";
            res += "ecall\n";
        }
    } else if (operation == "shr") {
        if (!is_unsigned)
            res += "sra t0, t1, t2\n";
        else
            res += "srl t0, t1, t2\n";
    } else if (operation == "shl") {
        res += "sll t0, t1, t2\n";
    } else if (operation == "and") {
        res += "and t0, t1, t2\n";
    } else if (operation == "or") {
        res += "or t0, t1, t2\n";
    } else if (operation == "xor") {
        res += "xor t0, t1, t2\n";
    } else if (operation == "cmp_lt") {
        if (!is_unsigned)
            res += "slt t0, t1, t2\n";
        else
            res += "sltu t0, t1, t2\n";
    } else if (operation == "mul" || operation == "div" || operation == "mod") {
        if (is_unsigned) {
            if (operation == "mul") {
                res += "addi t0, x0, 0\n";
                res += "beq t2, x0, 44\n";
                res += "addi t3, t0, 0\n";
                res += "add t0, t0, t1\n";
                res += "bgeu t0, t3, 24\n";
                res += "la a1, @msg_unsigned_overflow\n"; // two instructions
                res += "ecall\n";
                res += "addi a0, x0, 0\n";
                res += "ecall\n";
                res += "addi t2, t2, -1\n";
                res += "beq x0, x0, -40\n";
            } else if (operation == "div" || operation == "mod") {
                res += "bne t2, x0, 24\n";
                res += "la a1, @msg_div_zero\n";
                res += "ecall\n";
                res += "addi a0, x0, 0\n";
                res += "ecall\n";
                res += "addi t0, x0, 0\n";
                res += "bge x0, t1, 16\n";
                res += "sub t1, t1, t2\n";
                res += "addi t0, t0, 1\n";
                res += "beq x0, x0, -12\n";
                res += "beq t1, x0, 12\n";
                res += "add t1, t1, t2\n"; // Compensite for over-sub
                res += "addi t0, t0, -1\n";
                if (operation == "mod") // Return remainder rather than quotient
                    res += "addi t0, t1, 0";
            }
        } else {
            if (operation == "mod") {
                res += "slt t3, t1, 0\n";
                res += "slt t4, t2, 0\n";
                res += "xor s1, t3, t4\n"; // s1 = sign of result
            } else {
                res += "slt s1, t1, 0\n";
            }
            res += "beq t3, x0, 8\n";
            res += "sub t1, x0, t1\n"; // t1 = -t1, never overflow
            res += "beq t4, x0, 8\n";
            res += "sub t2, x0, t2\n";              // t2 = -t2
            res += get_basic_calc(operation, true); // do unsigned mul/div/mod
            res += "beq s1, x0, 28\n";
            res += "sub t0, x0, t0\n";
            res += "blt t0, x0, 24\n";
            res += "la a1, @msg_signed_overflow\n";
            res += "ecall\n";
            res += "addi a0, x0, 0\n";
            res += "ecall\n";
        }
    } else if (operation == "neg") { // No overflow check
        res += "sub t0, x0, t1\n";
    } else if (operation == "cmp_le") {
        res += get_reg_xchg(t1, t0);
        res += get_basic_calc("cmp_ge", is_unsigned);
    } else if (operation == "cmp_gt") {
        res += get_reg_xchg(t1, t2);
        res += get_basic_calc("cmp_lt", is_unsigned);
    } else if (operation == "cmp_ge") {
        if (!is_unsigned)
            res += "bge t1, t2, 12\n";
        else
            res += "bgeu t1, t2, 12\n";
        res += "addi t0, x0, 0\n";
        res += "beq x0, x0, 8\n";
        res += "addi t0, x0, 1\n";
    } else if (operation == "cmp_eq") {
        res += "beq t1, t2, 12\n";
        res += "addi t0, x0, 0\n";
        res += "beq x0, x0, 8\n";
        res += "addi t0, x0, 1\n";
    } else if (operation == "cmp_ne") {
        res += "bne t1, t2, 12\n";
        res += "addi t0, x0, 0\n";
        res += "beq x0, x0, 8\n";
        res += "addi t0, x0, 1\n";
    } else {
        printf("%s is not a basic expr\n", operation.data());
    }
    return res;
}

/************************** Memory Load-Store **************************/
// Always load memory cell to t1, and write t0 to memory cell

// size: 1, 2 or 4, and other value is illegal
// dir = 0: load, from mem to reg(t0)
// dir = 1: write, from reg(t1) to mem
// is_signed: only for read operation
string get_mem_access(int32_t offset, uint32_t size, bool dir, bool is_signed,
                      bool is_globl, string name) {
    if (size == 1) {

    } else if (size == 2) {

    } else if (size == 4) {

    } else {
        printf("size %d is illegal in single memory access\n", size);
    }
}

// Copy func/proc perameters before func/proc call
// mode = 0: global to local
// mode = 1: local to local
string get_mem_copy(uint32_t dst_offset, uint32_t src_offset, uint32_t size,
                    string src_name, bool mode) {}

/********************** Function or Procedure Call **********************/

// func/proc's perem/retval is always in stack
// func/proc's Activation Record:
// ------------------------------ (High Address)
// |          List of           | {Leftest in arg list is at lowest addr}
// |         Parameters         |
// ------------------------------
// |        Return Value        | {4 bytes, only for func, BasicRealType}
// ------------------------------
// |         OLD ra(x1)         | {4 bytes (one word)}
// ------------------------------
// |         OLD fp(x8)         | {4 bytes (one word), as control link}
// ------------------------------ <- fp(x8/s0) HERE!
// |       Local Variables      | {Vary with func. LEN is treated as const}
// ------------------------------ <- sp(x2) HERE!
// | (Saved REGs before a call) | {LEN is also calced and convey to nested func}
// ------------------------------
// |       Free Stack Space     |
// ------------------------------ (Low Address)

// 1 - DO NOT USE s0(fp) for other purpose during a call!
// 2 - If use callee saved register s1-s11 or other
// caller saved register, please save them before a call
// 3 - sp is never modified to another const after initialized
// 4 - To support access of non-local & non-global vars, use "OLD fp"
// or control link to get the start addr of caller's local var

// External Visable Function for Assembly Code Generation

string get_func_def() {}

string get_func_call() {}

string get_ctrl_link() {}

bool start_asm(std::string filename, uint32_t init_stack_top) {
    if (asm_file != NULL)
        return false;
    in_code_segment = in_data_segment = false;
    internal_label_id = 0;
    asm_file = fopen(filename.data(), "w+");
    if (asm_file == NULL)
        printf("unable to create file %s\n", filename.data());
    if (asm_file != NULL)
        init_asm(init_stack_top);
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
    for (int i = 0; i < snippet.length(); i++) {
        if (i == 0)
            fprintf(asm_file, "\t");
        fprintf(asm_file, "%c", snippet[i]);
        if (snippet[i] == '\n' && i != snippet.length() - 1)
            fprintf(asm_file, "\t");
    }
    return true;
}

void get_define_global(string name, vector<uchar> field_size) {
    // enter_segment(true);

    fprintf(asm_file, "%s:\n", name.data());

    fprintf(asm_file, "\t.align 4\n");
}

string get_ordinal_bound_check() {}

string get_stmt_assign() {}

string get_stmt_if() {}

string get_stmt_case() {}

string get_stmt_while() {}

string get_stmt_repeat() {}

string get_stmt_for() {}

string get_read() {}

string get_write() {}
