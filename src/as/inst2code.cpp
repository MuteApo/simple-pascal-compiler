#include <cstdio>
#include <map>
#include <string>
#include <vector>

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

const map<string, uint8_t> pseudo_size = {{"la", 2}, {"li", 2}, {"call", 2}};

uint32_t addr_counter = 0;

uint8_t get_inst_size(string operation, vector<string> operands) {
    if (pseudo_size.count(operation) == 1) {
        return pseudo_size.find(operation)->second * INST_SIZE;
    } else if (operation == "lw" || operation == "lh" || operation == "lb" || operation == "lhu" ||
               operation == "lbu" || operation == "sb" || operation == "sh" || operation == "sw") {
        // lb lh lw lbu lhu sb sh sw, with symbol immediate
    } else if (inst_opcode_type.count(operation)) {
        return INST_SIZE;
    } else {
        return 0;
    }
}

void gen_hex(FILE *output, string text, string data) {}