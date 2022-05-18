#include <map>
#include <string>

#include "include/ram.hpp"

using namespace std;

static uint32_t registers[32];
uint32_t pc = 0x0;

bool set_pc(uint32_t val) {
    if (val >= avail_size)
        return false;
    pc = val;
    return true;
}

uint32_t get_pc(void) { return pc; }

bool inc_pc(void) {
    if (pc + 4 >= avail_size)
        return false;
    pc += 4;
    return true;
}

uint32_t read_register(unsigned char id) {
    if (id > 31)
        return 0;
    else
        return registers[id];
}

bool write_register(unsigned char id, uint32_t data) {
    if (id > 31)
        return false;
    registers[id] = data;
    return true;
}

string reg2alias(unsigned char id) {
    if (id > 31)
        return "illegal";
    if (id >= 5 && id <= 7) {
        return "t" + to_string(id - 5);
    } else if (id >= 28 && id <= 31) {
        return "t" + to_string(id - 28 + 3);
    } else if (id >= 8 && id <= 9) {
        if (id == 8)
            return "s0/fp";
        return "s1";
    } else if (id >= 18 && id <= 27) {
        return "s" + to_string(id - 18 + 2);
    } else if (id >= 10 && id <= 17) {
        return "a" + to_string(id - 10);
    }
    switch (id) {
    case 0:
        return "zero";
    case 1:
        return "ra";
    case 2:
        return "sp";
    case 3:
        return "gp";
    case 4:
        return "tp";
    }
}