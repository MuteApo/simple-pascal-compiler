#ifndef _ASM_GEN_
#define _ASM_GEN_

#include <string>
#include <vector>

/** Register Operation **/

std::string get_load_imm(int32_t imm);

std::string get_reg_xchg(uint8_t dst_reg, uint8_t src_reg);

std::string get_integer_calc(std::string operation, bool is_unsigned = true);

/** Memory Operation **/

// For non-local & non-global var, get control link first
std::string get_mem_access(uint8_t size, bool dir, bool is_signed);

std::string get_var_access(int32_t var_pos, int32_t offset, uint8_t size, bool dir, bool is_signed);

std::string
get_var_access(std::string name, int32_t offset, uint8_t size, bool dir, bool is_signed);

std::string get_reg_save(void);

std::string get_reg_restore(void);

std::string get_params_copy(uint32_t dst_stk_pos, uint32_t src_stk_pos, uint32_t length);

std::string get_params_copy(uint32_t dst_stk_pos, std::string src_name, uint32_t length);

/** Basic Assembly Code Generatio Function **/

bool start_asm(std::string filename, uint32_t init_stack_top = 0x1FFFF);

bool finish_asm(void);

bool write_segment(std::string snippet, bool data_seg);

/** High-Level Assembly Snippet Generation **/

std::string get_define_global(std::string           name,
                              std::vector<uint8_t>  field_size,
                              std::vector<uint32_t> field_rep);

std::string
get_func_def(std::string name, uint32_t args_len, uint32_t local_vars_len, std::string func_body);

std::string get_func_call(
    std::string name, std::string parem_copy, bool has_retval, uint32_t call_lv, uint32_t def_lv);

std::string get_func_cleanup(uint32_t args_len, bool has_retval);

std::string get_retval_read(uint8_t size, bool is_unsigned);

std::string get_retval_write(uint8_t size, bool is_unsigned);

std::string get_param_access(
    uint32_t param_pos, uint32_t offset, bool has_retval, uint8_t size, bool dir, bool is_signed);

std::string get_access_link(void);

std::string get_ordinal_bound_check(uint32_t lower_bound, uint32_t upper_bound);

std::string get_stmt_cond(std::string calc_expr, std::string then_stmts, std::string else_stmts);

std::string get_stmt_while(std::string calc_expr, std::string stmts);

std::string get_stmt_until(std::string calc_expr, std::string stmts);

std::string get_read(std::string type);

std::string get_write(std::string type);

std::string get_exit(void);

#endif
