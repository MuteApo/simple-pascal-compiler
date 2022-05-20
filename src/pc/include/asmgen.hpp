#ifndef _ASM_GEN_
#define _ASM_GEN_

#include <string>
#include <vector>

typedef unsigned char uchar;

void init_asm(uint32_t init_stack_top);

string get_reg_xchg(uchar dst_reg, uchar src_reg);

string get_basic_calc(string operation, bool is_unsigned = true,
                      bool use_imm_src2 = false, int32_t imm = 0x0);

string get_mem_access(int32_t offset, uint32_t size, bool dir, bool is_signed,
                      bool is_globl = false, string name = "");

string get_mem_copy(uint32_t dst_offset, uint32_t src_offset, uint32_t size,
                    string src_name = "", bool mode = true);

bool start_asm(std::string filename, uint32_t init_stack_top = 0x1FFFF);

bool finish_asm(void);

bool write_segment(std::string snippet, bool data_seg);

void get_define_global(std::string name, std::vector<uchar> field_size);

string get_ordinal_bound_check();

string get_stmt_assign();

string get_stmt_if();

string get_stmt_case();

string get_stmt_while();

string get_stmt_repeat();

string get_stmt_for();

string get_read();

string get_write();

string get_func_def();

string get_func_call();

string get_ctrl_link();

#endif
