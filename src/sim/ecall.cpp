#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

#include "include/ram.hpp"
#include "include/register.hpp"

using namespace std;

void proc_ecall(bool &finish) {
    uint32_t func = read_register(10);
    uint32_t arg1 = read_register(11);
    uint32_t arg2 = read_register(12);
    if (func == 0) {  // exit
        finish = true;
    } else if (func == 1) {  // print_char
        finish = false;
        printf("%c", (char)arg1);
    } else if (func == 2) {  // print_str
        finish = false;
        printf("%s", get_ram_ptr(arg1));
    } else if (func == 3) {  // print_int
        finish = false;
        printf("%d", arg1);
    } else if (func == 4) {  // get_char
        finish = false;
        write_register(11, (uint32_t)getchar());
    } else if (func == 5) {  // get_line
        finish      = false;
        string line = "";
        getline(cin, line);
        memcpy(get_ram_ptr(arg1), line.data(), line.length() + 1);
    } else if (func == 6) {  // get_int
        finish = false;
        uint32_t num;
        scanf("%d", &num);
        write_register(11, num);
    } else if (func == 7) {  // mul
        write_register(10, arg1 * arg2);
    } else if (func == 8) {  // div
        write_register(10, arg1 / arg2);
    } else if (func == 9) {  // mod
        write_register(10, arg1 % arg2);
    }
    return;
}