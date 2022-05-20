#include <cstdio>
#include <map>
#include <string>
#include <vector>

using namespace std;

#include "include/asmgen.hpp"

bool in_code_segment;
bool in_data_segment;
FILE *asm_file = NULL;

static map<string, string> internal_msg = {
    {"msg_div_zero", "illegal: divided by zero"},
    {"msg_signed_overflow", "illegal: signed arithmetic overflow"},
    {"msg_unsigned_overflow", "illegal: unsigned arithmetic overflow"},
    {"msg_out_of_bounds", "illegal: access out of bounds"}};

bool start_asm(std::string filename, uint32_t init_stack_top) {
    if (asm_file != NULL)
        return false;
    in_code_segment = in_data_segment = false;
    asm_file = fopen(filename.data(), "w+");
    if (asm_file == NULL)
        printf("unable to create file %s\n", filename.data());
    if (asm_file != NULL)
        add_internal(init_stack_top);
    return asm_file != NULL;
}

void add_internal(uint32_t init_stack_top) {
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
    uint32_t low_12_bits = init_stack_top & 0xFFFFF;
    uint32_t high_20_bits = (init_stack_top & ~(uint32_t)0xFFFFF) >> 12;
    fprintf(asm_file, "\tlui sp, %d\n", high_20_bits);
    fprintf(asm_file, "\taddi sp, sp, %d\n", low_12_bits);
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
            printf("\t");
        fprintf(asm_file, "%c", snippet[i]);
        if (snippet[i] == '\n' && i != snippet.length() - 1)
            fprintf(asm_file, "\t");
    }
    return true;
}

void insert_data(string name, vector<uchar> field_size) {
    // enter_segment(true);

    fprintf(asm_file, "%s:\n", name.data());

    fprintf(asm_file, "\t.align 4\n");
}

string get_expr() {}

string get_stmt_while(string);

string get_stmt_for();

string get_stmt_if() {}

string get_stmt_case() {}

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

// For complex atom calc, use t3-t6 for intermediate result
// t0-t6 are volatile. DO NOT use them for other purpose!

/********************* `Atom` Expression Calculation ********************/
// `Atom` Calculation is for Integer
// sub, mul, div, mod and neg is implement by software for strict overflow check

string get_complex_calc(string complex_op, bool is_unsigned, bool use_imm_src2,
                        int32_t imm) {
    string res = "";
    if (complex_op == "mul") {

    } else if (complex_op == "div") {

    } else if (complex_op == "mod") {

    } else if (complex_op == "neg") {

    } else {
        printf("%s is not a complex calculation\n", complex_op.data());
    }
    return res;
}

string get_basic_calc(string basic_op, bool is_unsigned, bool use_imm_src2,
                      int32_t imm) {
    string res = "";
    if (basic_op == "not") {
        basic_op = "xor";
        use_imm_src2 = true;
        imm = 0xFFFFFFFF;
    }
    if (use_imm_src2) {
        uint32_t low_12_bits = imm & 0xFFFFF;
        uint32_t high_20_bits = ((uint32_t)imm & ~(uint32_t)0xFFFFF) >> 12;
        res += "addi t2, t2, 0\n";
        res += "lui t2, " + to_string(high_20_bits) + "\n";
        res += "addi t2, t0, " + to_string(low_12_bits) + "\n";
    }
    if (basic_op == "add") {
        res += "add t0, t1, t2\n";
        if (is_unsigned) {
            res += "bgeu t0, t1, 24\n";
            res += "addi a0, x0, 2\n";
            res += "la a1, @msg_unsigned_overflow\n";
            res += "ecall\n";
            res += "addi a0, x0, 0\n";
            res += "ecall\n";
        } else {
            res += "slti t3, t2, 0\n";
            res += "slt t4, t0, t1\n";
            res += "beq t3, t4, 24\n";
            res += "addi a0, x0, 2\n";
            res += "la a1, @msg_signed_overflow\n";
            res += "ecall\n";
            res += "addi a0, x0, 0\n";
            res += "ecall\n";
        }
    } else if (basic_op == "shr") {
        if (!is_unsigned)
            res += "sra t0, t1, t2\n";
        else
            res += "srl t0, t1, t2\n";
    } else if (basic_op == "shl") {
        res += "sll t0, t1, t2\n";
    } else if (basic_op == "and") {
        res += "and t0, t1, t2\n";
    } else if (basic_op == "or") {
        res += "or t0, t1, t2\n";
    } else if (basic_op == "xor") {
        res += "xor t0, t1, t2\n";
    } else {
        printf("%s is not a basic calculation\n", basic_op.data());
    }
    return res;
}

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

/************************** Memory Load-Store **************************/
// Always load memory cell to t1, and write t0 to memory cell

// Copy func/proc perameters before func/proc call
string get_copy_local2local(uint32_t dst_offset, uint32_t src_offset,
                            uint32_t size) {}

string get_copy_globl2local(uint32_t dst_offset, string globl_name,
                            uint32_t size) {}

// size: 1, 2 or 4, and other value is illegal
// dir = 0: load, from mem to reg
// dir = 1: write, from reg to mem
// is_signed: only for read operation
string get_local_access(uint32_t offset, uint32_t size, bool dir,
                        bool is_signed) {}

string get_globl_access(string name, uint32_t offset, uint32_t size, bool dir,
                        bool is_signed) {}

/********************** Function or Procedure Call **********************/

// func/proc's perem/retval is always in stack
// func/proc's Activation Record:
// ------------------------------ (High Address)
// |          List of           | {Leftest in arg list is at lowest addr,}
// |         Parameters         | {and field of arg is stored reversly}
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

string get_func(bool is_proc) {}