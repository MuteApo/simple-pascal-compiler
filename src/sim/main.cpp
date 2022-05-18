#include <cstdio>
#include <cstring>

#include "include/cpu.hpp"
#include "include/ram.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    uint32_t load_base = 0, ram_size = -1, init_pc = 0;
    bool debug_flag = false;
    if (argc < 2 || argv[1][0] == '-') {
        printf("missing binary filename\n");
        return 1;
    }
    string binary_filename = string(argv[1]);
    FILE *binary_file = fopen(binary_filename.data(), "rb");
    if (binary_file == NULL) {
        printf("can not open binary file\n");
        return 1;
    }
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            debug_flag = true;
        } else if (strcmp(argv[i], "-s") == 0) {
            if (i + 1 >= argc || argv[i + 1][0] == '-') {
                printf("missing size after '-s'\n");
                return 1;
            }
            ram_size = strtol(argv[i + 1], NULL, 16);
            i += 1;
        } else if (strcmp(argv[i], "-b") == 0) {
            if (i + 1 >= argc || argv[i + 1][0] == '-') {
                printf("missing address after '-b'\n");
                return 1;
            }
            load_base = strtol(argv[i + 1], NULL, 16);
            i += 1;
        } else if (strcmp(argv[i], "-r") == 0) {
            if (i + 1 >= argc || argv[i + 1][0] == '-') {
                printf("missing address after '-r'\n");
                return 1;
            }
            strtol(argv[i + 1], NULL, 16);
            i += 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            printf("Simple RISC-V VM Simulator, version 0.1\n");
            printf("Usage: rvsim <binary> (<options>)\n");
            printf("-d Start up simulation with debug mode\n");
            printf("-s <size> Set maximum RAM <size> from 0x0\n");
            printf(
                "-b <addr> Load binary into base <addr> (HEX, default: 0x0)\n");
            printf(
                "-r <addr> Set PC to <addr> when reset (HEX, default: 0x0)\n");
            printf("-h Show this information\n");
        } else {
            printf("unrecognized command line option %s\n", argv[i]);
            return 1;
        }
    }
    init_ram(binary_file, load_base, ram_size);
    fclose(binary_file);
    if (debug_flag)
        return (debug_cpu(init_pc) == false);
    else
        return (run_cpu(init_pc) == false);
}