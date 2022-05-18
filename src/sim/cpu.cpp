#include <string>

#include "include/code2inst.hpp"
#include "include/ecall.hpp"
#include "include/ram.hpp"
#include "include/register.hpp"

#define CLS "clear"

using namespace std;

string exception_msg = "";

// Only disassembly when exec is false
bool exec_inst(uint32_t inst, bool exec, bool &finish, string &inst_asm) {
    inst_asm = "";
    if (opcode_inst.count(get_opcode(inst)) == 0) {
        exception_msg = "[Illegal instruction]";
        inst_asm = "INOP";
        return false;
    }
    switch (opcode_inst.at(get_opcode(inst))) {
    case alu_reg:
    case alu_imm: {
        uint32_t src1, src2, result;
        src1 = read_register(get_rs1(inst));
        if (opcode_inst.at(get_opcode(inst)) == alu_reg)
            src2 = read_register(get_rs2(inst));
        else
            src2 = get_i_type_imm(inst);
        if (func3_alu_op.count(get_func3(inst)) == 0) {
            exception_msg = "[Illegal instruction]";
            inst_asm = "INOP";
            return false;
        }
        string alu_op = func3_alu_op.at(get_func3(inst));
        if (alu_op == "add_or_sub") {
            if (get_func7(inst) == 0b0100000) {
                if (opcode_inst.at(get_opcode(inst)) == alu_imm) {
                    exception_msg = "[Illegal instruction]";
                    inst_asm = "INOP";
                    return false;
                } else {
                    inst_asm += "sub";
                    result = src1 - src2;
                }
            } else if (get_func7(inst) == 0b0000000) {
                inst_asm += "add";
                result = src1 + src2;
            } else {
                exception_msg = "[Illegal instruction]";
                inst_asm = "INOP";
                return false;
            }
        } else {
            inst_asm += alu_op;
            if (alu_op == "slt") {
                result = ((int32_t)src1 < (int32_t)src2);
            } else if (alu_op == "sltu") {
                result = (src1 < src2);
            } else if (alu_op == "and") {
                result = src1 & src2;
            } else if (alu_op == "or") {
                result = src1 | src2;
            } else if (alu_op == "xor") {
                result = src1 ^ src2;
            } else if (alu_op == "sll") {
                result = src1 << src2;
            } else if (alu_op == "srl") {
                result = src1 >> src2;
            } else if (alu_op == "sra") {
                result = (uint32_t)((int32_t)src1 >> src2);
            }
        }
        if (exec) {
            write_register(get_rd(inst), result);
        }
        if (opcode_inst.at(get_opcode(inst)) == alu_imm)
            inst_asm += "i";
        inst_asm += " " + reg2alias(get_rd(inst));
        inst_asm += ", " + reg2alias(get_rs1(inst));
        if (opcode_inst.at(get_opcode(inst)) == alu_imm) {
            inst_asm += ", " + to_string((int16_t)get_i_type_imm(inst));
        } else {
            inst_asm += ", " + reg2alias(get_rs2(inst));
        }
        break;
    }
    case load: {
        if (func3_mem_size.count(get_func3(inst)) == 0) {
            exception_msg = "[Illegal instruction]";
            inst_asm = "INOP";
            return false;
        }
        uint32_t offset = get_i_type_imm(inst);
        uint32_t addr = read_register(get_rs1(inst)) + offset;
        string mem_op_size = func3_mem_size.at(get_func3(inst));
        inst_asm += "l" + mem_op_size;
        inst_asm += ' ' + reg2alias(get_rd(inst));
        inst_asm += ", " + to_string((int32_t)offset);
        inst_asm += '(' + reg2alias(get_rs1(inst)) + ')';
        if (addr >= avail_size) {
            exception_msg = "[Load access fault]";
            return false;
        }
        uint32_t mem_data;
        if (mem_op_size == "b") {
            mem_data = read_ram_byte(addr, false);
        } else if (mem_op_size == "h") {
            mem_data = read_ram_halfword(addr, false);
        } else if (mem_op_size == "w") {
            mem_data = read_ram_word(addr);
        } else if (mem_op_size == "bu") {
            mem_data = read_ram_byte(addr, true);
        } else if (mem_op_size == "hu") {
            mem_data = read_ram_halfword(addr, true);
        }
        if (exec) {
            write_register(get_rd(inst), mem_data);
        }
        break;
    }
    case store: {
        if (func3_mem_size.count(get_func3(inst)) == 0) {
            exception_msg = "[Illegal instruction]";
            inst_asm = "INOP";
            return false;
        }
        uint32_t offset = get_s_type_imm(inst);
        uint32_t addr = read_register(get_rs1(inst)) + offset;
        uint32_t data = read_register(get_rs2(inst));
        string mem_op_size = func3_mem_size.at(get_func3(inst));
        inst_asm += "s" + mem_op_size;
        inst_asm += " " + reg2alias(get_rs2(inst));
        inst_asm += ", " + to_string((int32_t)offset);
        inst_asm += '(' + reg2alias(get_rs1(inst)) + ')';
        if (addr >= avail_size) {
            exception_msg = "[Store access fault]";
            return false;
        }
        if (mem_op_size == "bu" || mem_op_size == "hu") {
            exception_msg = "[Illegal instruction]";
            inst_asm = "INOP";
            return false;
        } else if (mem_op_size == "b") {
            if (exec)
                write_ram(addr, data, 1);
        } else if (mem_op_size == "h") {
            if (exec)
                write_ram(addr, data, 2);
        } else if (mem_op_size == "w") {
            if (exec)
                write_ram(addr, data, 4);
        }
        break;
    }
    case branch: {
        uint32_t new_addr;
        // TODO
        if (func3_branch_cond.count(get_func3(inst)) == 0) {
            exception_msg = "[Illegal instruction]";
            inst_asm = "INOP";
            return false;
        }
        string branch_cond = func3_branch_cond.at(get_func3(inst));
        inst_asm += "s" + branch_cond;
        if (!set_pc(new_addr)) {
            exception_msg = "[Instruction access fault]";
            return false;
        }
        return true;
        break;
    }
    case jal: {

        return true;
        break;
    }
    case jalr: {

        return true;
        break;
    }
    case lui: {

        break;
    }
    case auipc: {

        break;
    }
    case sys: {

        break;
    }
    }
    return true;
}

void stat_cpu(void) {
    system(CLS);
    printf("PC: 0x%08X", get_pc());
    printf("\n\nGeneral Registers:\n");
    for (int reg_id = 0; reg_id < 32; reg_id++) {
        uint32_t reg_data = read_register(reg_id);
        const char *reg_alias = reg2alias(reg_id).data();
        printf("x%d(%s):0x%08X\t", reg_id, reg_alias, reg_data);
        if (reg_id % 4 == 0)
            printf("\n");
    }
    printf("\n\nContent of Stack:\n");
    uint32_t sp = read_register(2);
    for (int i = 0; i < 96; i++) {
        uint32_t elem_addr = sp + i;
        if (elem_addr >= avail_size)
            break;
        if (i % 24 == 0) {
            if (i != 0)
                printf("\n");
            printf("  0x%08x: ", elem_addr);
        }
        printf("%02x ", read_ram_byte(elem_addr, true));
    }
    // printf("\n");
    printf("\n\nInstructions:\n");
    for (int i = 0; i < 5; i++) {
        bool finish;
        string inst_asm;
        uint32_t inst_addr = get_pc() + i * 4;
        if (inst_addr >= avail_size)
            break;
        uint32_t inst = read_ram_word(inst_addr);
        if (i == 0)
            printf("->");
        else
            printf("  ");
        printf("0x%08x:0x%08x\t", inst_addr, inst);
        exec_inst(inst, false, finish, inst_asm);
        puts(inst_asm.data());
    }
    printf("\n");
}

bool step_cpu(bool &normal_exit) {
    uint32_t inst = read_ram_word(get_pc());
    string inst_asm = "";
    if (!exec_inst(inst, true, normal_exit, inst_asm))
        return false;
    if (!inc_pc()) {
        exception_msg = "[Instruction access fault]";
        return false;
    }
    return true;
}

bool run_cpu(uint32_t init_pc) {
    bool normal_exit = false, step_succ;
    while (1) {
        step_succ = step_cpu(normal_exit);
        if (normal_exit) {
            break;
        } else if (!step_succ) {
            puts(exception_msg.data());
            break;
        }
    }
    return normal_exit;
}

bool debug_cpu(uint32_t init_pc) {
    bool normal_exit = false, step_succ;
    while (1) {
        stat_cpu();
        step_succ = step_cpu(normal_exit);
        if (normal_exit) {
            break;
        } else if (!step_succ) {
            puts(exception_msg.data());
            break;
        }
        printf("<Press Any Key to Step>");
        getchar();
    }
    return normal_exit;
}
