#include <cstdio>
#include <cstring>
#include <map>
#include <string>

#include "inst2code.hpp"

using namespace std;

/******************** Supported Assembler Directives ********************/
// .text	Store subsequent instructions in the text segment
// .data	Store subsequent items in the static segment
// .align	Can be only used in data segment, control the alignment below
// .byte	Store listed values as 8-bit bytes
// .word	Store listed values as unaligned 32-bit words
// [ .byte and .word data is splitted with comma or space,
// format: bin('0b') / dec / hex('0x') ]
// .asciiz	Store subsequent string in the data segment and add '\0'

// '<label>:' is also supported to generate symbol table

// only one instruction in one line, and comma between oprand is optional

map<string, uint32_t> label_table;
map<string, uint32_t> equiv_table;

void get_source(string &buffer) {
    char c = getchar();
    while (c != EOF) {
        buffer += c;
        c = getchar();
    }
}

void split_segment(string buffer, string &equiv, string &text, string &data) {
    for (int i = 0; i < buffer.length(); i++) {
    }
}

void scan_symbol(string equiv, string text, string data) {}

// Suported Pseudo-instructions: li, la, j
void gen_hex(FILE *output, string text, string data) {}

int main(int argc, char *argv[]) {
    string output_filename = "target.hex";
    FILE *output_file = NULL;
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
            printf("Simple RISC-V Assembler, version 0.1\n");
            printf("-i <file> Use <file> as source input (default: stdin)\n");
            printf("-o <file> Place the HEX text output into <file> (default: "
                   "target.hex)\n");
            printf("-s Print symbol table during assembling\n");
            printf("-h Show this information\n");
            return 0;
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
    string input_buffer = "";
    string equiv_part = "", text_seg = "", data_seg = "";
    get_source(input_buffer);
    split_segment(input_buffer, equiv_part, text_seg, data_seg);
    scan_symbol(equiv_part, text_seg, data_seg);
    gen_hex(output_file, text_seg, data_seg);
    fclose(output_file);
    return 0;
}