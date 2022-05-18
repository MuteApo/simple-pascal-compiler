#include <cstdio>
#include <string>

uint8_t *ram = NULL;
int avail_size = 0;

bool init_ram(FILE *binary_file, uint32_t load_base, uint32_t ram_size) {
    fseek(binary_file, 0, SEEK_END);
    int binary_size = ftell(binary_file);
    if (ram_size == -1) {
        ram = (uint8_t *)malloc(binary_size);
        avail_size = binary_size;
    } else if (ram_size - load_base < binary_size) {
        return false;
    } else {
        ram = (uint8_t *)malloc(ram_size);
        avail_size = ram_size;
    }
    fseek(binary_file, 0, SEEK_SET);
    fread(ram + load_base, 1, binary_size, binary_file);
    return true;
}

uint32_t read_ram_byte(uint32_t addr, bool is_unsigned) {
    if (addr >= avail_size)
        return 0x0;
    if (is_unsigned) { // Do zero extension
        return ram[addr];
    } else { // Do sign extension
        uint32_t result = ram[addr];
        if (result & (0x1 << 7)) {
            result |= ~(uint32_t)0xFF;
        }
        return result;
    }
}

uint32_t read_ram_halfword(uint32_t addr, bool is_unsigned) {
    if (addr + 1 >= avail_size)
        return 0x0;
    if (is_unsigned) { // Little-Endian
        return ram[addr] | (ram[addr + 1] << 8);
    } else {
        uint32_t result = ram[addr] | (ram[addr + 1] << 8);
        if (result & (0x1 << 15)) {
            result |= ~(uint32_t)0xFFFF;
        }
        return result;
    }
}

uint32_t read_ram_word(uint32_t addr) {
    if (addr + 3 >= avail_size)
        return 0x0;
    uint32_t result = 0;
    for (int i = 0; i < 4; i++) {
        result |= ram[addr + i] << (8 * i);
    }
    return result;
}

bool write_ram(uint32_t addr, uint32_t data, unsigned char byte_cnt) {
    if (addr + byte_cnt - 1 >= avail_size)
        return false;
    uint32_t mask = 0xFF;
    for (int i = 0; i < byte_cnt; i++) {
        ram[addr + i] = data & mask;
        mask <<= 8;
    }
    return true;
}

uint8_t *get_ram_ptr(uint32_t addr) {
    if (addr > avail_size)
        return NULL;
    return &ram[addr];
}
