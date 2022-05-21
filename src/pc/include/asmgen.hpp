#ifndef _ASM_GEN_
#define _ASM_GEN_

#include <string>
#include <vector>

typedef unsigned char uchar;

void init_asm(uint32_t init_stack_top);

std::string get_load_imm(int32_t imm);

std::string get_reg_xchg(uchar dst_reg, uchar src_reg);

std::string get_basic_calc(std::string operation, bool is_unsigned = true);

std::string get_mem_access(int32_t stk_pos, int32_t offset, uchar size,
                           bool dir, bool is_signed);

std::string get_mem_access(std::string name, int32_t offset, uchar size,
                           bool dir, bool is_signed);

string get_mem_copy(uint32_t dst_stk_pos, uint32_t src_stk_pos,
                    uint32_t length);

string get_mem_copy(uint32_t dst_stk_pos, string src_name, uint32_t length);

bool start_asm(std::string filename, uint32_t init_stack_top = 0x1FFFF);

bool finish_asm(void);

bool write_segment(std::string snippet, bool data_seg);

void get_define_global(std::string name, std::vector<uchar> field_size);

std::string get_ordinal_bound_check();

std::string get_stmt_assign();

std::string get_stmt_if();

std::string get_stmt_case();

std::string get_stmt_while();

std::string get_stmt_repeat();

std::string get_stmt_for();

std::string get_read();

std::string get_write();

std::string get_func_def();

std::string get_func_call();

std::string get_ctrl_link();

#endif
