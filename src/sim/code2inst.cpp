#include <map>
#include <string>

#include "include/code2inst.hpp"

/******************** Supported RV32I Instruction ********************/
// I-Type: addi slti sltiu andi ori xori slli srli srai lb lh lw lbu lhu ecall
// R-Type: add sub slt sltu and or xor sll srl sra S-Type: sb sh sw
// B-Type: beq bne blt bltu bge bgeu
// J-Type: jal, I-Type: jalr
// U-Type: lui auipc
// Bit 6 - Bit 0, 7 bits

uint8_t get_opcode(uint32_t code) { return code & 0x7F; }

// Bit 14 - Bit 12, 3 bits
uint8_t get_func3(uint32_t code) {
    uint32_t mask = (uint32_t)(0x7) << 12;
    return (code & mask) >> 12;
}

// Bit 31 - Bit 25, 7 bits
uint8_t get_func7(uint32_t code) {
    uint32_t mask = (uint32_t)(0x7F) << 25;
    return (code & mask) >> 25;
}
// Bit 31 - Bit 20, 12 bits
uint32_t get_i_type_imm(uint32_t code) {
    uint32_t result = get_func7(code) << 5;
    result |= get_rs2(code);
    if (result & (0x01 << 11)) { // Always do sign extension
        result |= ~(uint32_t)0xFFF;
    }
    return result;
}

// Bit 31 - Bit 25(imm[11:5]) and
// Bit 11 - Bit 7(imm[4:0]), 12 bits
uint32_t get_s_type_imm(uint32_t code) {
    uint32_t result = get_func7(code) << 5;
    result |= get_rd(code);
    if (result & (0x01 << 11)) {
        result |= ~(uint32_t)0xFFF;
    }
    return result;
}

// Bit 31 - Bit 25(imm[12|10:5]) and
// Bit 11 - Bit 7(imm[4:1|11]), 13 bits with imm[0] = 0
uint32_t get_b_type_imm(uint32_t code) {
    uint32_t result = get_s_type_imm(code);
    result |= (result & (0x1 << 11)) << 1; // imm[12] = imm[11]
    result &= ~(0b1 << 11);                // imm[11] = 0
    result |= (result & 0x1) << 11;        // imm[11] = imm[0]
    result &= ~0b1;                        // imm[0] = 0
    if (result & (0x01 << 12)) {
        result |= ~(uint32_t)0x1FFF;
    }
    return result;
}

// Bit 31 - Bit 20(imm[20|10:1|11|19:12]), 21 bits with imm[0] = 0
uint32_t get_j_type_imm(uint32_t code) {
    uint32_t result = 0, temp;
    uint16_t bit_20, part_19_12, bit_11, part_10_1;
    temp = get_u_type_imm(code);
    part_10_1 = (result & (0x3FF << 9)) >> 9;
    bit_11 = (temp & (0x1 << 8)) >> 8;
    part_19_12 = result & 0xFF;
    bit_20 = (temp & (0x1 << 19)) >> 19;
    result |= part_10_1 << 1;
    result |= bit_11 << 11;
    result |= part_19_12 << 12;
    result |= bit_20 << 20;
    if (result & (0x01 << 20)) {
        result |= ~(uint32_t)0x1FFFFF;
    }
    return result;
}

// Bit 31 - Bit 12, 20 bits
uint32_t get_u_type_imm(uint32_t code) {
    uint32_t result = get_func3(code);
    result |= get_rs1(code) << 3;
    result |= get_i_type_imm(code) << (3 + 5);
    if (result & (0x01 << 19)) {
        result |= ~(uint32_t)0xFFFFF;
    }
    return result;
}

// Bit 19 - Bit 15, 5 bits
uint8_t get_rs1(uint32_t code) {
    uint32_t mask = (uint32_t)(0x1F) << 15;
    return (code & mask) >> 15;
}

// Bit 24 - Bit 20, 5 bits
uint8_t get_rs2(uint32_t code) {
    uint32_t mask = (uint32_t)(0x1F) << 20;
    return (code & mask) >> 20;
}

// Bit 11 - Bit 7, 5 bits
uint8_t get_rd(uint32_t code) {
    uint32_t mask = (uint32_t)(0x1F) << 7;
    return (code & mask) >> 7;
}

// With shamt field, func7[0] must be 0
uint8_t get_shamt(uint32_t code) { return get_rs2(code); }

const std::map<uint8_t, opcode_type> opcode_inst = {
    {0b0110011, alu_reg}, {0b0010011, alu_imm}, {0b0100011, store},
    {0b0000011, load},    {0b1100011, branch},  {0b0110111, lui},
    {0b0010111, auipc},   {0b1101111, jal},     {0b1100111, jalr},
    {0b1110011, sys}};

const std::map<uint8_t, std::string> func3_alu_op{
    // There's sub but NO subi
    {0b000, "add_or_sub"}, {0b010, "slt"}, {0b011, "sltu"},
    {0b111, "and"},        {0b110, "or"},  {0b100, "xor"},
    {0b001, "sll"},        {0b101, "srl"}, {0b101, "sra"},
};

const std::map<uint8_t, std::string> func3_mem_size{// NO sbu and shu inst
                                                    {0b000, "b"},
                                                    {0b001, "h"},
                                                    {0b010, "w"},
                                                    {0b100, "bu"},
                                                    {0b101, "hu"}};

const std::map<uint8_t, std::string> func3_branch_cond{
    {0b000, "eq"},  {0b001, "ne"}, {0b100, "lt"},
    {0b110, "ltu"}, {0b101, "ge"}, {0b111, "geu"}};

// For func7, sub/sra(i) has 0b0100000, and other has 0b0000000