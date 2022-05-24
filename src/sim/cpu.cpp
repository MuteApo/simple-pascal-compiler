#include <iostream>
#include <string>

#include "include/code2inst.hpp"
#include "include/ecall.hpp"
#include "include/ram.hpp"
#include "include/register.hpp"

#define CLS "clear"

using namespace std;

string exception_msg = "";

// Only disassembly when exec is false
bool exec_inst(uint32_t inst, bool exec, bool &finish, bool &skip_pc_inc, string &inst_asm) {
    inst_asm = "";
    finish = skip_pc_inc = false;
    if (opcode_inst.count(get_opcode(inst)) == 0) {
        exception_msg = "[Illegal instruction]";
        inst_asm      = "INOP";
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
                inst_asm      = "INOP";
                return false;
            }
            string alu_op = func3_alu_op.at(get_func3(inst));
            if (alu_op == "add_or_sub") {
                if (opcode_inst.at(get_opcode(inst)) == alu_imm) {
                    inst_asm += "add";
                    result = src1 + src2;
                } else if (get_func7(inst) == 0b0000000) {
                    inst_asm += "add";
                    result = src1 + src2;
                } else if (get_func7(inst) == 0b0100000) {
                    inst_asm += "sub";
                    result = src1 - src2;
                } else {
                    exception_msg = "[Illegal instruction]";
                    inst_asm      = "INOP";
                    printf("%x", get_func7(inst));
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
            if (exec) { write_register(get_rd(inst), result); }
            if (opcode_inst.at(get_opcode(inst)) == alu_imm) inst_asm += "i";
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
                inst_asm      = "INOP";
                return false;
            }
            uint32_t offset      = get_i_type_imm(inst);
            uint32_t addr        = read_register(get_rs1(inst)) + offset;
            string   mem_op_size = func3_mem_size.at(get_func3(inst));
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
                if (exec && (addr & 0b01)) {
                    exception_msg = "[Load address misaligned]";
                    return false;
                }
                mem_data = read_ram_halfword(addr, false);
            } else if (mem_op_size == "w") {
                if (exec && (addr & 0b11)) {
                    exception_msg = "[Load address misaligned]";
                    return false;
                }
                mem_data = read_ram_word(addr);
            } else if (mem_op_size == "bu") {
                mem_data = read_ram_byte(addr, true);
            } else if (mem_op_size == "hu") {
                if (exec && (addr & 0b01)) {
                    exception_msg = "[Load address misaligned]";
                    return false;
                }
                mem_data = read_ram_halfword(addr, true);
            }
            if (exec) { write_register(get_rd(inst), mem_data); }
            break;
        }
        case store: {
            if (func3_mem_size.count(get_func3(inst)) == 0) {
                exception_msg = "[Illegal instruction]";
                inst_asm      = "INOP";
                printf("%x", get_func3(inst));
                return false;
            }
            uint32_t offset      = get_s_type_imm(inst);
            uint32_t addr        = read_register(get_rs1(inst)) + offset;
            uint32_t data        = read_register(get_rs2(inst));
            string   mem_op_size = func3_mem_size.at(get_func3(inst));
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
                inst_asm      = "INOP";
                return false;
            } else if (mem_op_size == "b") {
                if (exec) write_ram(addr, data, 1);
            } else if (mem_op_size == "h") {
                if (exec) {
                    if (addr & 0b01) {
                        exception_msg = "[Store address misaligned]";
                        return false;
                    }
                    write_ram(addr, data, 2);
                }
            } else if (mem_op_size == "w") {
                if (exec) {
                    if (addr & 0b11) {
                        exception_msg = "[Store address misaligned]";
                        return false;
                    }
                    write_ram(addr, data, 4);
                }
            }
            break;
        }
        case branch: {
            if (func3_branch_cond.count(get_func3(inst)) == 0) {
                exception_msg = "[Illegal instruction]";
                inst_asm      = "INOP";
                return false;
            }
            bool     cond          = false;
            string   branch_cond   = func3_branch_cond.at(get_func3(inst));
            uint32_t src1          = read_register(get_rs1(inst));
            uint32_t src2          = read_register(get_rs2(inst));
            uint32_t pc_rel_offset = get_b_type_imm(inst);
            uint32_t target_pc     = get_pc() + pc_rel_offset;
            inst_asm += "b" + branch_cond;
            inst_asm += " " + reg2alias(get_rs1(inst));
            inst_asm += ", " + reg2alias(get_rs2(inst));
            inst_asm += ", " + to_string((int32_t)pc_rel_offset);
            if (branch_cond == "eq") {
                cond = src1 == src2;
            } else if (branch_cond == "ne") {
                cond = src1 != src2;
            } else if (branch_cond == "lt") {
                cond = (int32_t)src1 < (int32_t)src2;
            } else if (branch_cond == "ltu") {
                cond = src1 < src2;
            } else if (branch_cond == "ge") {
                cond = (int32_t)src1 > (int32_t)src2;
            } else if (branch_cond == "geu") {
                cond = src1 > src2;
            }
            if (exec && cond) {
                if (target_pc & 0b11) {
                    exception_msg = "[Instruction address misaligned]";
                    return false;
                }
                if (!set_pc(target_pc)) {
                    exception_msg = "[Instruction access fault]";
                    return false;
                }
                skip_pc_inc = true;
            }
            break;
        }
        case jal: {
            uint32_t saved_pc      = get_pc() + 4;
            uint32_t pc_rel_offset = get_j_type_imm(inst);
            uint32_t target_pc     = get_pc() + pc_rel_offset;
            inst_asm += "jal";
            inst_asm += " " + reg2alias(get_rd(inst));
            inst_asm += ", " + to_string((int32_t)get_j_type_imm(inst));
            if (exec) {
                write_register(get_rd(inst), saved_pc);
                if (target_pc & 0b11) {
                    exception_msg = "[Instruction address misaligned]";
                    return false;
                }
                if (!set_pc(target_pc)) {
                    exception_msg = "[Instruction access fault]";
                    return false;
                }
                skip_pc_inc = true;
            }
            break;
        }
        case jalr: {
            if (get_func3(inst) != 0x0) {
                exception_msg = "[Illegal instruction]";
                inst_asm      = "INOP";
                return false;
            }
            uint32_t saved_pc         = get_pc() + 4;
            uint32_t target_pc_base   = get_i_type_imm(inst);
            uint32_t target_pc_offset = read_register(get_rs1(inst));
            uint32_t target_pc        = target_pc_base + target_pc_offset;
            target_pc &= ~(uint32_t)0x1;
            inst_asm += "jalr";
            inst_asm += " " + reg2alias(get_rd(inst));
            inst_asm += ", " + to_string((int32_t)target_pc_base);
            inst_asm += '(' + reg2alias(get_rs1(inst)) + ')';
            if (exec) {
                write_register(get_rd(inst), saved_pc);
                if (target_pc & 0b11) {
                    exception_msg = "[Instruction address misaligned]";
                    return false;
                }
                if (!set_pc(target_pc)) {
                    exception_msg = "[Instruction access fault]";
                    return false;
                }
                skip_pc_inc = true;
            }
            break;
        }
        case lui: {
            uint32_t imm = get_u_type_imm(inst) << 12;
            inst_asm += "lui";
            inst_asm += " " + reg2alias(get_rd(inst));
            inst_asm += ", " + to_string((int32_t)get_u_type_imm(inst));
            if (exec) write_register(get_rd(inst), imm);
            break;
        }
        case auipc: {
            uint32_t imm = get_u_type_imm(inst) << 12;
            inst_asm += "auipc";
            inst_asm += " " + reg2alias(get_rd(inst));
            inst_asm += ", " + to_string((int32_t)get_u_type_imm(inst));
            if (exec) write_register(get_rd(inst), imm + get_pc());
            break;
        }
        case sys: {
            if (inst & ~((uint32_t)0b1111111)) {
                exception_msg = "[Instruction access fault]";
                return false;
            }
            inst_asm = "ecall";
            if (exec) proc_ecall(finish);
            break;
        }
    }
    write_register(0, 0x0);  // Make sure x0 is hard-wired
    return true;
}

void stat_cpu(void) {
    system(CLS);
    printf("PC: 0x%08X\n", get_pc());
    printf("\nGeneral Registers:\n");
    for (int reg_id = 0; reg_id < 32; reg_id++) {
        uint32_t reg_data  = read_register(reg_id);
        string   reg_alias = reg2alias(reg_id).data();
        printf("x%d(%s):0x%08X\t", reg_id, reg_alias.data(), reg_data);
        if (reg_id % 4 == 3) printf("\n");
    }
    printf("\nContents of Stack:\n");
    uint32_t sp = read_register(2);
    for (int i = 0; i < 96; i++) {
        uint32_t elem_addr = sp + i;
        if (elem_addr >= avail_size) break;
        if (i % 24 == 0) {
            if (i != 0) printf("\n");
            printf("  0x%08X: ", elem_addr);
        }
        printf("%02X ", read_ram_byte(elem_addr, true));
    }
    printf("\n\nInstructions:\n");
    for (int i = 0; i < 5; i++) {
        bool     finish, skip_pc_inc;
        string   inst_asm;
        uint32_t inst_addr = get_pc() + i * 4;
        if (inst_addr >= avail_size) break;
        uint32_t inst = read_ram_word(inst_addr);
        if (i == 0)
            printf("->");
        else
            printf("  ");
        printf("0x%08X:0x%08X\t", inst_addr, inst);
        exec_inst(inst, false, finish, skip_pc_inc, inst_asm);
        puts(inst_asm.data());
    }
    printf("\n");
}

bool step_cpu(bool &normal_exit) {
    uint32_t inst        = read_ram_word(get_pc());
    string   inst_asm    = "";
    bool     skip_pc_inc = false;
    if (!exec_inst(inst, true, normal_exit, skip_pc_inc, inst_asm)) return false;
    if (!skip_pc_inc) {
        if (!inc_pc()) {
            exception_msg = "[Instruction access fault]";
            return false;
        }
    }
    return true;
}

bool run_cpu(uint32_t init_pc) {
    bool normal_exit = false, step_succ;
    if (!set_pc(init_pc)) {
        puts("[Instruction access fault]");
        return false;
    }
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
    bool     normal_exit = false, step_succ;
    bool     brk, step = true;
    uint32_t brk_addr = 0;
    if (!set_pc(init_pc)) {
        puts("[Instruction access fault]");
        return false;
    }
    while (1) {
        stat_cpu();
        brk       = get_pc() == brk_addr;
        step_succ = step_cpu(normal_exit);
        if (normal_exit) {
            break;
        } else if (!step_succ) {
            puts(exception_msg.data());
            break;
        }
        if (!step)
            if (!brk) continue;
        static string cmd = "";
        while (1) {
            printf("<Commands: s(step), v(view), b(breakpoint), q(quit)>\n");
            string cmd_buf;
            getline(cin, cmd_buf);
            if (cmd_buf != "") cmd = cmd_buf;
            if (cmd == "s" || cmd == "step") {
                step = true;
                break;
            } else if (cmd == "q" || cmd == "quit") {
                return true;
            } else if (cmd == "v" || cmd == "view") {
                printf("RAM Address in HEX:");
                string addr_str;
                getline(cin, addr_str);
                uint32_t addr = strtol(addr_str.data(), NULL, 16);
                uint32_t data = read_ram_word(addr);
                printf("0x%08X:0x%08X\n", addr, data);
                printf("\n");
                continue;
            } else if (cmd == "b" || cmd == "breakpoint") {
                printf("Break next time at (in HEX):");
                string addr_str;
                getline(cin, addr_str);
                brk_addr = strtol(addr_str.data(), NULL, 16);
                step     = false;
                break;
            } else {
                printf("unknown command, retry\n");
                continue;
            }
        }
    }
    return normal_exit;
}
