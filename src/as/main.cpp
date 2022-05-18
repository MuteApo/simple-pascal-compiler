#include <cstdio>
#include <cstring>
#include <map>
#include <string>

#include "inst2code.hpp"

using namespace std;

/******************** Supported Assembler Directives ********************/
// .entry   Set entrance of the progrom executable part
// .text	Store subsequent instructions in the text segment
// .data	Store subsequent items in the static segment
// .align	Can be only used in data segment, control the alignment below
// .byte	Store listed values as 8-bit bytes
// .word	Store listed values as unaligned 32-bit words
// [.byte and .word data is splitted with comma, format: dec or hex('0x')]
// .asciiz	Store subsequent string in the data segment and add '\0'

// '<label>:' is also supported to generate symbol table

void get_source(string &buffer) {
    char c = getchar();
    while (c != EOF) {
        buffer += c;
        c = getchar();
    }
}
void scan_symbol(string buffer) {}
void gen_binary(FILE *output) {}

int main(int argc, char *argv[]) {
    string output_filename = "target.bin";
    FILE *output_file = NULL;
    string input_buffer = "";
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            if (i + 1 >= argc || argv[i + 1][0] == '-') {
                printf("missing filename after '-i'\n");
                return 1;
            }
            if (freopen(argv[i + 1], "r", stdin) == NULL) {
                printf("can not open source file %s!\n", argv[i + 1]);
                return 1;
            }
            i += 1;
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc || argv[i + 1][0] == '-') {
                printf("missing filename after '-o'\n");
                return 1;
            }
            output_filename = string(argv[i + 1]);
            i += 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            printf("Simple RISC-V Assembler, version 0.1, support all RV32I "
                   "instruction.\n");
            printf("-i <file> Use <file> as source input (default: stdin)\n");
            printf("-o <file> Place the binary output into <file> (default: "
                   "target.bin)");
            printf("-h Show this information\n");
        } else {
            printf("unrecognized command line option %s\n", argv[i]);
            return 1;
        }
    }
    output_file = fopen(output_filename.data(), "w+");
    if (output_file == NULL) {
        printf("can not create binary file %s!\n", output_filename.data());
        return 1;
    }
    get_source(input_buffer);
    scan_symbol(input_buffer);
    gen_binary(output_file);
    fclose(output_file);
    return 0;
}