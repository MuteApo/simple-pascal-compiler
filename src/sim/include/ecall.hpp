#ifndef _ECALL_H_
#define _ECALL_H_

#define ECALL_EXIT 0
#define ECALL_INT_MUL 1
#define ECALL_INT_DIV 2
#define ECALL_INT_MOD 3
#define ECALL_REAL_ADD 4
#define ECALL_REAL_SUB 5
#define ECALL_REAL_MUL 6
#define ECALL_REAL_DIV 7
#define ECALL_PRT_CHAR 8
#define ECALL_PRT_STR 9
#define ECALL_PRT_INT 10
#define ECALL_PRT_REAL 11
#define ECALL_READ_CHAR 12
#define ECALL_READ_STR 13
#define ECALL_READ_INT 14
#define ECALL_READ_REAL 15

void proc_ecall(bool &finish);

#endif