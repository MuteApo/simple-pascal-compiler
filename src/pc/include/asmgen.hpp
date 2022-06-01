#ifndef _ASM_GEN_
#define _ASM_GEN_

#include <string>
#include <vector>

#define ECALL_EXIT 0
#define ECALL_INT_MUL 1
#define ECALL_INT_DIV 2
#define ECALL_INT_MOD 3
#define ECALL_REAL_ADD 4
#define ECALL_REAL_SUB 5
#define ECALL_REAL_MUL 6
#define ECALL_REAL_DIV 7
#define ECALL_PRT_CHAR 8
#define ECALL_PRT_STR 9
#define ECALL_PRT_INT 10
#define ECALL_PRT_REAL 11
#define ECALL_READ_CHAR 12
#define ECALL_READ_STR 13
#define ECALL_READ_INT 14
#define ECALL_READ_REAL 15

// All ASM Snippets use Static Register Allocation Policy below:
// 1 - Atom Operation: t0 = t1 BASIC_OP(binary operator) t2, or
//                     t0 = BASIC_OP(unary operator) t1
// 2 - Register Load: t0 = src(immediate or stack)
// 3 - Register Store: dst(stack) = t1
// 4 - Memory Read: t0 = MEM[t2]
// 5 - Memory Write: MEM[t1] = t2

// For all assembly snippet, they use t3-t6 for intermediate result
// During code generation, use s1-11 or stack to backup t0-t6 register

const uint8_t t_table[7]  = {5, 6, 7, 28, 29, 30, 31};
const uint8_t s_table[12] = {8, 9, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};
const uint8_t a_table[8]  = {10, 11, 12, 13, 14, 15, 16, 17};

// func/proc's perem/retval is always in stack, but for
// a0: calling level of the func/proc
// a1: defination level of the func/proc
// func/proc's Activation Record:
// ------------------------------ (High Address)
// |        Return Value        | {4 bytes, only for func, BasicRealType}
// |----------------------------|
// |          List of           | {Leftest in arg list is at lowest addr}
// |         Parameters         |
// |----------------------------|
// |    CALL Lv - DEF Lv + 1    | {4 bytes (one word), as indirect access link}
// |----------------------------|
// |         OLD ra(x1)         | {4 bytes (one word)}
// |----------------------------|
// |         OLD s1-s11         | {4 * 11 bytes (11 words)}
// |----------------------------|
// |        OLD fp(x8/s0)       | {4 bytes (one word), as control link}
// |----------------------------| <- fp(x8/s0) HERE!
// |       Local Variables      | {LEN varies with func as a const}
// |----------------------------| <- sp(x2) HERE!
// | (Saved REGs before a call) | {Saved temporary registers}
// |----------------------------|
// |       Free Stack Space     | {For inner func/proc's AR}
// ------------------------------ (Low Address)

// 1 - DO NOT USE s0(fp) for other purpose during a call!
// 2 - sp is never modified to another const after initialized
// 3 - "List of Params" and "RetVal" is established and cleaned by caller
//      and other parts of AR are maintained by callee

/** Basic Assembly Code Generation Function **/

bool start_asm(std::string filename, uint32_t init_stack_top = 0x1FFF0);

bool finish_asm(void);

bool remove_asm(void);

bool write_segment(std::string snippet, bool data_seg);

std::string get_define_global(std::string           name,
                              std::vector<uint8_t>  field_size,
                              std::vector<uint32_t> field_rep);

/** Register Operation **/

std::string get_load_imm(int32_t imm);

std::string get_reg_xchg(uint8_t dst_reg, uint8_t src_reg);

std::string get_reg_save(uint8_t reg);

std::string get_reg_restore(uint8_t reg);

/** ALU Operation with Register **/

std::string get_integer_calc(std::string operation, bool is_unsigned = false);

std::string get_float_calc(std::string operation);

/** Memory Operation **/

std::string get_local_addr(int32_t local_offset);

std::string get_global_addr(std::string name);

std::string get_param_addr(uint32_t param_offset);

// Always load memory cell to t0, and write t2 to memory cell
// Memory address is in t1

// size: 1, 2 or 4, and other value is illegal
// dir = false: load, from mem to reg(t0)
// dir = true: write, from reg(t1) to mem
// is_signed: only for read operation
std::string get_mem_access(uint8_t size, bool dir, bool is_signed);

// Copy func/proc perameters before func/proc call
// t0: temp regs for single load and store
// t1: dst base addr
// t2: src base addr

std::string get_mem_copy(uint32_t length);

/** Function Defination and Call **/

// a0: calling level of the func/proc
// a1: defination level of the func/proc
std::string get_func_framework(bool is_head, std::string name = "", uint32_t local_var_len = 0);

// t2: source of parameter
std::string get_param_copy(uint32_t length);

// Store single basic or ordinal type parameter in t2
std::string get_param_store(void);

std::string get_func_call(
    std::string name, std::string param_copy, bool has_retval, uint32_t call_lv, uint32_t def_lv);

std::string get_retval_read(uint32_t arg_len);

std::string get_retval_addr(uint32_t arg_len);

std::string get_func_cleanup(uint32_t arg_len, bool has_retval);

std::string get_fp_before(uint32_t level_to_jump);

std::string get_access_link(void);

/** Basic Statement and Dynamic Check **/

// Ordinal data for checking is in t1 register
std::string get_bound_check(uint32_t lower_bound, uint32_t upper_bound);

std::string get_stmt_cond(std::string calc_expr, std::string then_stmt, std::string else_stmt);

std::string get_stmt_while(std::string calc_expr, std::string stmt);

std::string get_stmt_until(std::string calc_expr, std::string stmt);

std::string get_read(std::string type);

std::string get_write(std::string type);

std::string get_exit(void);

#endif
