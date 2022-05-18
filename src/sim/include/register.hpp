#ifndef _REG_H_
#define _REG_H_

#include <string>

uint32_t read_register(unsigned char id);

bool write_register(unsigned char id, uint32_t data);

std::string reg2alias(unsigned char id);

bool set_pc(uint32_t val);

uint32_t get_pc(void);

bool inc_pc(void);

#endif