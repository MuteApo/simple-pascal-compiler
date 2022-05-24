#ifndef _RV32I_INST_H_
#define _RV32I_INST_H_

#include <string>
#include <vector>

extern uint32_t addr_counter;

uint8_t get_inst_size(std::string operation, std::vector<std::string> operands);
bool    gen_hex(FILE *output, std::string text, std::string data);

#endif