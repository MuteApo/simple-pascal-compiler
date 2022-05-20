#ifndef _ASM_GEN_
#define _ASM_GEN_

#include <string>
#include <vector>

typedef unsigned char uchar;

bool start_asm(std::string filename, uint32_t init_stack_top = 0x1FFFF);

void add_internal(uint32_t init_stack_top);

bool finish_asm(void);
bool write_segment(std::string snippet, bool data_seg);

void insert_data(std::string name, std::vector<uchar> field_size);

std::string get_expr();

std::string get_stmt_while(std::string);

std::string get_stmt_for();

std::string get_stmt_if();

std::string get_stmt_case();

std::string get_complex_calc(std::string complex_op, bool is_unsigned = true,
                             bool use_imm_src2 = false, int32_t imm = 0);

std::string get_basic_calc(std::string basic_op, bool is_unsigned = true,
                           bool use_imm_src2 = false, int32_t imm = 0);

std::string get_reg_xchg(uchar dst_reg, uchar src_reg);

std::string get_copy_local2local(uint32_t dst_offset, uint32_t src_offset,
                                 uint32_t size);

std::string get_copy_globl2local(uint32_t dst_offset, std::string globl_name,
                                 uint32_t size);

std::string get_local_access(uint32_t offset, uint32_t size, bool dir,
                             bool is_signed = false);

std::string get_globl_access(std::string name, uint32_t offset, uint32_t size,
                             bool dir, bool is_signed = false);
std::string get_func(bool is_proc);

#endif
