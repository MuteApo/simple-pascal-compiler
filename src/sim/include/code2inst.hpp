#ifndef _RV32I_INST_H_
#define _RV32I_INST_H_

#include <map>
#include <string>

typedef enum {
    alu_reg = 101, // R-Type Format
    alu_imm,       // I-Type Format
    load,          // I-Type Format
    store,         // S-Type Format
    branch,        // B-Type Format
    jal,           // J-Type Format
    jalr,          // I-Type Format
    lui,           // U-Type Format
    auipc,         // U-Type Format
    sys            // I-Type Format
} opcode_type;

uint8_t get_opcode(uint32_t code);
uint8_t get_func3(uint32_t code);
uint8_t get_func7(uint32_t code);
uint32_t get_i_type_imm(uint32_t code);
uint32_t get_s_type_imm(uint32_t code);
uint32_t get_b_type_imm(uint32_t code);
uint32_t get_j_type_imm(uint32_t code);
uint32_t get_u_type_imm(uint32_t code);
uint8_t get_rs1(uint32_t code);
uint8_t get_rs2(uint32_t code);
uint8_t get_rd(uint32_t code);
uint8_t get_shamt(uint32_t code);

extern const std::map<uint8_t, opcode_type> opcode_inst;
extern const std::map<uint8_t, std::string> func3_alu_op;
extern const std::map<uint8_t, std::string> func3_mem_size;
extern const std::map<uint8_t, std::string> func3_branch_cond;

#endif