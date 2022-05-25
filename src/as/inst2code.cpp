#include <cstdio>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "include/reader.hpp"
#include "include/symbol.hpp"
#include "include/utility.hpp"

using namespace std;

/******************** Supported RV32I Instruction ********************/
// I-Type: addi slti sltiu andi ori xori slli srli srai lb lh lw lbu lhu ecall
// R-Type: add sub slt sltu and or xor sll srl sra S-Type: sb sh sw
// B-Type: beq bne blt bltu bge bgeu
// J-Type: jal, I-Type: jalr
// U-Type: lui auipc

#define INST_SIZE 4

typedef enum {
    alu_reg = 101,  // R-Type Format
    alu_imm,        // I-Type Format
    load,           // I-Type Format
    store,          // S-Type Format
    branch,         // B-Type Format
    jal,            // J-Type Format
    jalr,           // I-Type Format
    lui,            // U-Type Format
    auipc,          // U-Type Format
    sys             // I-Type Format
} opcode_type;

const map<opcode_type, uint8_t> inst_opcode      = {{alu_reg, 0b0110011},
                                               {alu_imm, 0b0010011},
                                               {store, 0b0100011},
                                               {load, 0b0000011},
                                               {branch, 0b1100011},
                                               {lui, 0b0110111},
                                               {auipc, 0b0010111},
                                               {jal, 0b1101111},
                                               {jalr, 0b1100111},
                                               {sys, 0b1110011}};
const map<string, opcode_type>  inst_opcode_type = {
    {"addi", alu_imm}, {"slti", alu_imm}, {"sltiu", alu_imm}, {"andi", alu_imm}, {"ori", alu_imm},
    {"xori", alu_imm}, {"slli", alu_imm}, {"srli", alu_imm},  {"srai", alu_imm}, {"add", alu_reg},
    {"sub", alu_reg},  {"slt", alu_reg},  {"sltu", alu_reg},  {"and", alu_reg},  {"or", alu_reg},
    {"xor", alu_reg},  {"sll", alu_reg},  {"srl", alu_reg},   {"sra", alu_reg},  {"lb", load},
    {"lh", load},      {"lw", load},      {"lbu", load},      {"lhu", load},     {"sb", store},
    {"sh", store},     {"sw", store},     {"beq", branch},    {"bne", branch},   {"blt", branch},
    {"bltu", branch},  {"bge", branch},   {"bgeu", branch},   {"jal", jal},      {"jalr", jalr},
    {"lui", lui},      {"auipc", auipc},  {"ecall", sys}};

const map<string, uint8_t> inst_func3 = {
    // U-Type insts and J-Type insts have NO func3
    {"addi", 0b000}, {"slti", 0b010}, {"sltiu", 0b011}, {"andi", 0b111}, {"ori", 0b110},
    {"xori", 0b100}, {"slli", 0b001}, {"srli", 0b101},  {"srai", 0b101}, {"add", 0b000},
    {"sub", 0b000},  {"slt", 0b010},  {"sltu", 0b011},  {"and", 0b111},  {"or", 0b110},
    {"xor", 0b100},  {"sll", 0b001},  {"srl", 0b101},   {"sra", 0b101},  {"lb", 0b000},
    {"lh", 0b001},   {"lw", 0b010},   {"lbu", 0b100},   {"lhu", 0b101},  {"sb", 0b000},
    {"sh", 0b001},   {"sw", 0b010},   {"beq", 0b000},   {"bne", 0b001},  {"blt", 0b100},
    {"bltu", 0b110}, {"bge", 0b101},  {"bgeu", 0b111},  {"jalr", 0b000}, {"ecall", 0b000}};

const map<string, uint8_t> inst_func7 = {
    // Only R-Type insts and shift insts of I-Type have func7
    {"add", 0b0000000},
    {"sub", 0b0100000},
    {"slt", 0b0000000},
    {"sltu", 0b0000000},
    {"and", 0b0000000},
    {"or", 0b0000000},
    {"xor", 0b0000000},
    {"sll", 0b0000000},
    {"srl", 0b0000000},
    {"sra", 0b0100000},
    {"slli", 0b0000000},
    {"srli", 0b0000000},
    {"srai", 0b0100000}};

const map<string, uint8_t> reg_alias2id = {
    {"zero", 0}, {"x0", 0},   {"ra", 1},   {"x1", 1},   {"sp", 2},   {"x2", 2},   {"gp", 3},
    {"x3", 3},   {"tp", 4},   {"x4", 4},   {"t0", 5},   {"x5", 5},   {"t1", 6},   {"x6", 6},
    {"t2", 7},   {"x7", 7},   {"s0", 8},   {"fp", 8},   {"x8", 8},   {"s1", 9},   {"x9", 9},
    {"a0", 10},  {"x10", 10}, {"a1", 11},  {"x11", 11}, {"a2", 12},  {"x12", 12}, {"a3", 13},
    {"x13", 13}, {"a4", 14},  {"x14", 14}, {"a5", 15},  {"x15", 15}, {"a6", 16},  {"x16", 16},
    {"a7", 17},  {"x17", 17}, {"s2", 18},  {"x18", 18}, {"s3", 19},  {"x19", 19}, {"s4", 20},
    {"x20", 20}, {"s5", 21},  {"x21", 21}, {"s6", 22},  {"x22", 22}, {"s7", 23},  {"x23", 23},
    {"s8", 24},  {"x24", 24}, {"s9", 25},  {"x25", 25}, {"s10", 26}, {"x26", 26}, {"s11", 27},
    {"x27", 27}, {"t3", 28},  {"x28", 28}, {"t4", 29},  {"x29", 29}, {"t5", 30},  {"x30", 30},
    {"t6", 31},  {"x31", 31}};

const map<string, uint8_t> pseudo_size = {{"la", 2}, {"li", 2}, {"call", 2}};

uint32_t addr_counter = 0;

uint8_t get_inst_size(string operation, vector<string> operands) {
    if (pseudo_size.count(operation) == 1) {
        return pseudo_size.find(operation)->second * INST_SIZE;
    } else if (inst_opcode_type.count(operation)) {
        return INST_SIZE;
    } else {
        return 0;
    }
}

bool get_reg_id(string reg_str, uint8_t &id) {
    if (reg_alias2id.count(reg_str) == 0) {
        printf("invalid register identifier\n");
        return false;
    } else {
        id = reg_alias2id.find(reg_str)->second;
        return true;
    }
}

bool get_r_type_inst(uint32_t &inst,
                     uint8_t   opcode,
                     uint8_t   func3,
                     uint8_t   func7,
                     uint8_t   rs1,
                     uint8_t   rs2,
                     uint8_t   rd) {
    inst = 0;
    inst |= opcode & 0x7F;
    inst |= (rd & 0x1F) << 7;
    inst |= (func3 & 0x7) << (5 + 7);
    inst |= (rs1 & 0x1F) << (3 + 5 + 7);
    inst |= (rs2 & 0x1F) << (5 + 3 + 5 + 7);
    inst |= (func7 & 0x7F) << (5 + 5 + 3 + 5 + 7);
    return true;
}

bool get_i_type_inst(
    uint32_t &inst, uint8_t opcode, uint8_t func3, uint8_t rs1, uint8_t rd, uint32_t imm_12bits) {
    bool is_overflow = !((imm_12bits & ~0xFFF) == 0x0 || (imm_12bits & ~0xFFF) == 0xFFFFF000);
    if (is_overflow) {
        printf("immediate of I-Type instruction is too big\n");
        return false;
    }
    inst = 0;
    inst |= opcode & 0x7F;
    inst |= (rd & 0x1F) << 7;
    inst |= (func3 & 0x7) << (5 + 7);
    inst |= (rs1 & 0x1F) << (3 + 5 + 7);
    inst |= (imm_12bits & 0xFFF) << (5 + 3 + 5 + 7);
    return true;
}

bool get_s_type_inst(
    uint32_t &inst, uint8_t opcode, uint8_t func3, uint8_t rs1, uint8_t rs2, uint32_t imm_12bits) {
    bool is_overflow = !((imm_12bits & ~0xFFF) == 0x0 || (imm_12bits & ~0xFFF) == 0xFFFFF000);
    if (is_overflow) {
        printf("immediate of S-Type instruction is too big\n");
        return false;
    }
    inst = 0;
    inst |= opcode & 0x7F;
    inst |= (imm_12bits & 0x1F) << 7;
    inst |= (func3 & 0x7) << (5 + 7);
    inst |= (rs1 & 0x1F) << (3 + 5 + 7);
    inst |= (rs2 & 0x1F) << (5 + 3 + 5 + 7);
    inst |= ((imm_12bits & 0xFE0) >> 5) << (5 + 5 + 3 + 5 + 7);
    return true;
}

bool get_b_type_inst(
    uint32_t &inst, uint8_t opcode, uint8_t func3, uint8_t rs1, uint8_t rs2, uint32_t imm_13bits) {
    bool is_overflow = !((imm_13bits & ~0x1FFF) == 0x0 || (imm_13bits & ~0x1FFF) == 0xFFFFE000);
    if (is_overflow) {
        printf("immediate of B-Type instruction is too big\n");
        return false;
    } else if (imm_13bits & 0x1) {
        printf("illegal offset of B-Type instruction\n");
        return false;
    }
    inst = 0;
    inst |= opcode & 0x7F;
    uint32_t rearr_imm_low = 0;
    rearr_imm_low |= (imm_13bits & (0x1 << 11)) >> 11;
    rearr_imm_low |= imm_13bits & (0xF << 1);
    inst |= rearr_imm_low << 7;
    inst |= (func3 & 0x7) << (5 + 7);
    inst |= (rs1 & 0x1F) << (3 + 5 + 7);
    inst |= (rs2 & 0x1F) << (5 + 3 + 5 + 7);
    uint32_t rearr_imm_high = 0;
    rearr_imm_high |= (imm_13bits & (0x3F << 5)) >> 5;
    rearr_imm_high |= (imm_13bits & (0x1 << 12)) ? 0x40 : 0x00;
    inst |= rearr_imm_high << (5 + 5 + 3 + 5 + 7);
    return true;
}

bool get_u_type_inst(uint32_t &inst, uint8_t opcode, uint8_t rd, uint32_t imm_high_20bits) {
    bool is_overflow =
        (!(imm_high_20bits & ~0xFFFFF) == 0x0 || (imm_high_20bits & ~0xFFFFF) == 0xFFF00000);
    if (is_overflow) {
        printf("immediate of U-Type instruction is too big\n");
        return false;
    }
    inst = 0;
    inst |= opcode & 0x7F;
    inst |= (rd & 0x1F) << 7;
    inst |= (imm_high_20bits & 0xFFFFF) << (5 + 7);
    return true;
}

bool get_j_type_inst(uint32_t &inst, uint8_t opcode, uint8_t rd, uint32_t imm_21bits) {
    bool is_overflow = !((imm_21bits & ~0x1FFFFF) == 0x0 || (imm_21bits & ~0x1FFFFF) == 0xFFE00000);
    if (is_overflow) {
        printf("immediate of J-Type instruction is too big\n");
        return false;
    } else if (imm_21bits & 0x1) {
        printf("illegal offset of J-Type instruction\n");
        return false;
    }
    inst = 0;
    inst |= opcode & 0x7F;
    inst |= (rd & 0x1F) << 7;
    uint32_t rearr_imm = 0;
    rearr_imm |= (imm_21bits & (0xFF << 12)) >> 12;
    rearr_imm |= (imm_21bits & (0x1 << 11)) ? 0x100 : 0x0;
    rearr_imm |= ((imm_21bits & (0x3FF << 1)) >> 1) << 9;
    rearr_imm |= (imm_21bits & (0x1 << 20)) ? (0x1 << 19) : 0x0;
    inst |= rearr_imm << (5 + 7);
    return true;
}

void split_int32(int32_t integer_32bits, uint32_t &low_12bits, uint32_t &high_20bits) {
    int32_t LoSEXT;
    low_12bits  = integer_32bits & 0xFFF;
    LoSEXT      = (low_12bits & 0x800) ? (low_12bits | 0xFFFFF000) : low_12bits;
    high_20bits = ((integer_32bits - LoSEXT) & 0xFFFFF000) >> 12;
}

bool gen_machine_code(FILE *output, string inst) {
    string         operation;
    vector<string> operands;
    uint32_t       machine_code = 0;
    read_inst(inst, operation, operands);
    if (operands.size() > 3) {
        printf("excessive operand\n");
        return false;
    } else {
        if (operation == "la") {  // Non-PIC, or 'lla'
            if (operands.size() != 2) {
                printf("wrong number of operand\n");
                return false;
            }
            uint8_t rd;
            int32_t imm_addr;
            if (!get_reg_id(operands[0], rd)) return false;
            if (!is_literal_integer(operands[1], imm_addr)) {
                uint32_t label_addr;
                if (!get_symbol_label(operands[1], label_addr)) {
                    printf("label not found\n");
                    return false;
                }
                imm_addr = label_addr;
            }
            uint32_t offsetHi, offsetLo, offset;
            offset = imm_addr - addr_counter;
            split_int32(offset, offsetLo, offsetHi);
            // auipc rd, offsetHi
            get_u_type_inst(machine_code, inst_opcode.find(auipc)->second, rd, offsetHi);
            fprintf(output, "0x%08X\n", machine_code);
            // addi rd, rd, offsetLo
            get_i_type_inst(machine_code,
                            inst_opcode.find(alu_imm)->second,
                            inst_func3.find("addi")->second,
                            rd,
                            rd,
                            offsetLo);
            fprintf(output, "0x%08X\n", machine_code);
        } else if (operation == "li") {
            if (operands.size() != 2) {
                printf("wrong number of operand\n");
                return false;
            }
            uint8_t rd;
            int32_t imm;
            if (!get_reg_id(operands[0], rd)) return false;
            if (!is_literal_integer(operands[1], imm)) {
                if (!get_symbol_const(operands[1], imm)) {
                    printf("constant not found\n");
                    return false;
                }
            }
            uint32_t immHi, immLo;
            split_int32(imm, immLo, immHi);
            // lui rd, immHi
            get_u_type_inst(machine_code, inst_opcode.find(lui)->second, rd, immHi);
            fprintf(output, "0x%08X\n", machine_code);
            // addi rd, rd, immLo
            get_i_type_inst(machine_code,
                            inst_opcode.find(alu_imm)->second,
                            inst_func3.find("addi")->second,
                            rd,
                            rd,
                            immLo);
            fprintf(output, "0x%08X\n", machine_code);
        } else if (operation == "call") {
            if (operands.size() != 1) {
                printf("wrong number of operand\n");
                return false;
            }
            int32_t imm_addr;
            if (!is_literal_integer(operands[0], imm_addr)) {
                uint32_t label_addr;
                if (!get_symbol_label(operands[0], label_addr)) {
                    printf("label not found\n");
                    return false;
                }
                imm_addr = label_addr;
            }
            imm_addr = imm_addr - addr_counter;
            uint8_t reg_ra;
            get_reg_id("ra", reg_ra);
            uint32_t addrHi, addrLo;
            split_int32(imm_addr, addrLo, addrHi);
            // auipc ra, addrHi
            get_u_type_inst(machine_code, inst_opcode.find(auipc)->second, reg_ra, addrHi);
            fprintf(output, "0x%08X\n", machine_code);
            // jalr ra, addrLo(ra)
            get_i_type_inst(machine_code,
                            inst_opcode.find(jalr)->second,
                            inst_func3.find("jalr")->second,
                            reg_ra,
                            reg_ra,
                            addrLo);
            fprintf(output, "0x%08X\n", machine_code);
        } else if (inst_opcode_type.count(operation) == 0) {
            printf("unknown instruction: '%s'\n", operation.data());
            return false;
        } else {
            opcode_type type = inst_opcode_type.find(operation)->second;
            switch (type) {
                case alu_reg: {
                    if (operands.size() != 3) {
                        printf("wrong number of operand\n");
                        return false;
                    }
                    uint8_t rs1, rs2, rd;
                    if (!get_reg_id(operands[0], rd)) return false;
                    if (!get_reg_id(operands[1], rs1)) return false;
                    if (!get_reg_id(operands[2], rs2)) return false;
                    if (!get_r_type_inst(machine_code,
                                         inst_opcode.find(alu_reg)->second,
                                         inst_func3.find(operation)->second,
                                         inst_func7.find(operation)->second,
                                         rs1,
                                         rs2,
                                         rd))
                        return false;
                    fprintf(output, "0x%08X\n", machine_code);
                    break;
                }
                case alu_imm: {
                    if (operands.size() != 3) {
                        printf("wrong number of operand\n");
                        return false;
                    }
                    uint8_t rs1, rd;
                    if (!get_reg_id(operands[0], rd)) return false;
                    if (!get_reg_id(operands[1], rs1)) return false;
                    int32_t imm_val;
                    if (!is_literal_integer(operands[2], imm_val)) {
                        if (!get_symbol_const(operands[2], imm_val)) {
                            printf("constant not found\n");
                            return false;
                        }
                    }
                    if (operation == "slli" || operation == "srli" || operation == "srai") {
                        if ((imm_val & 0x1F) != imm_val) {
                            printf("immediate for shift instruction is too big\n");
                            printf("%s\n", operation.data());
                            return false;
                        } else {
                            imm_val &= 0x1F;
                            imm_val |= (inst_func7.find(operation)->second & 0x7F) << 5;
                        }
                    }
                    if (!get_i_type_inst(machine_code,
                                         inst_opcode.find(alu_imm)->second,
                                         inst_func3.find(operation)->second,
                                         rs1,
                                         rd,
                                         imm_val))
                        return false;
                    fprintf(output, "0x%08X\n", machine_code);
                    break;
                }
                case store: {
                    if (operands.size() != 3) {
                        printf("wrong number of operand\n");
                        return false;
                    }
                    int32_t offset;
                    uint8_t rs1, rs2;
                    if (!get_reg_id(operands[0], rs2)) return false;
                    if (!get_reg_id(operands[1], rs1)) return false;
                    if (!is_literal_integer(operands[2], offset)) {
                        uint32_t label_offset;
                        if (!get_symbol_label(operands[2], label_offset)) {
                            printf("label not found\n");
                            return false;
                        }
                        offset = label_offset;
                    }
                    if (!get_s_type_inst(machine_code,
                                         inst_opcode.find(store)->second,
                                         inst_func3.find(operation)->second,
                                         rs1,
                                         rs2,
                                         offset))
                        return false;
                    fprintf(output, "0x%08X\n", machine_code);
                    break;
                }
                case load: {
                    if (operands.size() != 3) {
                        printf("wrong number of operand\n");
                        return false;
                    }
                    int32_t offset;
                    uint8_t rs1, rd;
                    if (!get_reg_id(operands[0], rd)) return false;
                    if (!get_reg_id(operands[1], rs1)) return false;
                    if (!is_literal_integer(operands[2], offset)) {
                        uint32_t label_offset;
                        if (!get_symbol_label(operands[2], label_offset)) {
                            printf("label not found\n");
                            return false;
                        }
                        offset = label_offset;
                    }
                    if (!get_i_type_inst(machine_code,
                                         inst_opcode.find(load)->second,
                                         inst_func3.find(operation)->second,
                                         rs1,
                                         rd,
                                         offset))
                        return false;
                    fprintf(output, "0x%08X\n", machine_code);
                    break;
                }
                case branch: {
                    if (operands.size() != 3) {
                        printf("wrong number of operand\n");
                        return false;
                    }
                    int32_t offset;
                    uint8_t rs1, rs2;
                    if (!get_reg_id(operands[0], rs1)) return false;
                    if (!get_reg_id(operands[1], rs2)) return false;
                    if (!is_literal_integer(operands[2], offset)) {
                        uint32_t target;
                        if (!get_symbol_label(operands[2], target)) {
                            printf("label not found\n");
                            return false;
                        }
                        offset = target - addr_counter;
                    }
                    if (!get_b_type_inst(machine_code,
                                         inst_opcode.find(branch)->second,
                                         inst_func3.find(operation)->second,
                                         rs1,
                                         rs2,
                                         offset))
                        return false;
                    fprintf(output, "0x%08X\n", machine_code);
                    break;
                }
                case lui:
                case auipc: {
                    if (operands.size() != 2) {
                        printf("wrong number of operand\n");
                        return false;
                    }
                    uint8_t rd;
                    int32_t imm_high;
                    if (!is_literal_integer(operands[1], imm_high)) {
                        if (!get_symbol_const(operands[1], imm_high)) {
                            printf("constant not found\n");
                            return false;
                        }
                    }
                    if (!get_reg_id(operands[0], rd)) return false;
                    if (!get_u_type_inst(
                            machine_code, inst_opcode.find(type)->second, rd, imm_high))
                        return false;
                    fprintf(output, "0x%08X\n", machine_code);
                    break;
                }
                case jal: {
                    if (operands.size() != 2) {
                        printf("wrong number of operand\n");
                        return false;
                    }
                    uint8_t rd;
                    int32_t offset;
                    if (!get_reg_id(operands[0], rd)) return false;
                    if (!is_literal_integer(operands[1], offset)) {
                        uint32_t target;
                        if (!get_symbol_label(operands[1], target)) {
                            printf("label not found\n");
                            return false;
                        }
                        offset = target - addr_counter;
                    }
                    if (!get_j_type_inst(machine_code, inst_opcode.find(jal)->second, rd, offset))
                        return false;
                    fprintf(output, "0x%08X\n", machine_code);
                    break;
                }
                case jalr: {
                    if (operands.size() != 3) {
                        printf("wrong number of operand\n");
                        return false;
                    }
                    uint8_t rs1, rd;
                    int32_t offset;
                    if (!get_reg_id(operands[0], rd)) return false;
                    if (!get_reg_id(operands[1], rs1)) return false;
                    if (!is_literal_integer(operands[2], offset)) {
                        if (!get_symbol_const(operands[1], offset)) {
                            printf("constant not found\n");
                            return false;
                        }
                    }
                    if (!get_i_type_inst(machine_code,
                                         inst_opcode.find(jalr)->second,
                                         inst_func3.find(operation)->second,
                                         rs1,
                                         rd,
                                         offset))
                        return false;
                    fprintf(output, "0x%08X\n", machine_code);
                    break;
                }
                case sys: {
                    if (operands.size() != 0) {
                        printf("wrong number of operand\n");
                        return false;
                    }
                    get_i_type_inst(machine_code,
                                    inst_opcode.find(sys)->second,
                                    inst_func3.find(operation)->second,
                                    0,
                                    0,
                                    0);
                    fprintf(output, "0x%08X\n", machine_code);
                    break;
                }
            }
        }
    }
    addr_counter += get_inst_size(operation, operands);
    return true;
}

bool gen_hex(FILE *output, string text, string data) {
    stringstream ss;
    string       line;
    ss           = stringstream(text);
    addr_counter = 0;
    while (getline(ss, line)) {
        if (!gen_machine_code(output, line)) return false;
    }
    ss = stringstream(data);
    while (getline(ss, line)) {
        uint32_t       size;
        vector<string> data_hex_strs;
        read_data(line, data_hex_strs, size);
        for (int i = 0; i < data_hex_strs.size(); i++) {
            fprintf(output, "%s\n", data_hex_strs[i].data());
        }
    }
    return true;
}