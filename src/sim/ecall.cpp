#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

#include "include/ecall.hpp"
#include "include/ram.hpp"
#include "include/register.hpp"

using namespace std;

void proc_ecall(bool &finish) {
    uint32_t func = read_register(10);
    uint32_t arg1 = read_register(11);
    uint32_t arg2 = read_register(12);
    finish        = false;
    switch (func) {
        case ECALL_EXIT: finish = true; break;
        case ECALL_INT_MUL: write_register(10, arg1 * arg2); break;
        case ECALL_INT_DIV: write_register(10, arg1 / arg2); break;
        case ECALL_INT_MOD: write_register(10, arg1 % arg2); break;
        case ECALL_REAL_ADD: {
            float res = *(float *)&arg1 + *(float *)&arg2;
            write_register(10, *(uint32_t *)&res);
            break;
        }
        case ECALL_REAL_SUB: {
            float res = *(float *)&arg1 - *(float *)&arg2;
            write_register(10, *(uint32_t *)&res);
            break;
        }
        case ECALL_REAL_MUL: {
            float res = *(float *)&arg1 * *(float *)&arg2;
            write_register(10, *(uint32_t *)&res);
            break;
        }
        case ECALL_REAL_DIV: {
            float res = *(float *)&arg1 / *(float *)&arg2;
            write_register(10, *(uint32_t *)&res);
            break;
        }
        case ECALL_REAL_FIT: {
            float res = arg1;
            write_register(10, *(uint32_t *)&res);
            break;
        }
        case ECALL_PRT_CHAR: printf("%c", (char)arg1); break;
        case ECALL_PRT_STR: printf("%s", get_ram_ptr(arg1)); break;
        case ECALL_PRT_INT: printf("%d", arg1); break;
        case ECALL_PRT_REAL: printf("%.1f", *(float *)&arg1); break;
        case ECALL_READ_CHAR: write_register(11, (uint32_t)getchar()); break;
        case ECALL_READ_STR: {
            string line = "";
            getline(cin, line);
            memcpy(get_ram_ptr(arg1), line.data(), line.length() + 1);
            break;
        }
        case ECALL_READ_INT: {
            uint32_t num;
            scanf("%d", &num);
            write_register(11, num);
            break;
        }
        case ECALL_READ_REAL: {
            float num;
            scanf("%f", &num);
            write_register(11, *(uint32_t *)&num);
            break;
        }
    }
}