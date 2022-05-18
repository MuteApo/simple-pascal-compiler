#ifndef _RAM_H_
#define _RAM_H_

#include <string>

extern int avail_size;

bool init_ram(FILE *binary_file, uint32_t load_base, uint32_t ram_size);

uint32_t read_ram_byte(uint32_t addr, bool is_unsigned);

uint32_t read_ram_halfword(uint32_t addr, bool is_unsigned);

uint32_t read_ram_word(uint32_t addr);

bool write_ram(uint32_t addr, uint32_t data, unsigned char byte_cnt);

uint8_t *get_ram_ptr(uint32_t addr);

#endif