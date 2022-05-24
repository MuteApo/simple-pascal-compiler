#include <cstdio>
#include <cstring>
#include <string>

#include "include/inst2code.hpp"
#include "include/symbol.hpp"
#include "include/utility.hpp"

using namespace std;

// Suported Pseudo-instructions: la(non-PIC), li, call and load/store with symbol offset

// All comma is seen as delimiter, which is just as white space
// Only one instruction in one line, and comma between oprand is optional

/******************** Supported Assembler Directives ********************/
// .text	Store subsequent instructions in the text segment
// .data	Store subsequent items in the static segment
// .align	Can be only used in data segment, control the alignment below
// [Directives .align is only valid in data segment]
// .byte	Store listed values as 8-bit bytes
// .word	Store listed values as unaligned 32-bit words
// [ .byte and .word data is splitted with comma or space, format: bin('0b') / dec / hex('0x') ]
// .asciiz	Store subsequent string in the data segment and add '\0'

// '<label>:' is also supported to generate symbol table

int main(int argc, char *argv[]) {
    string output_filename = "target.hex";
    FILE  *output_file     = NULL;
    bool   print_symtbl    = false;
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
        } else if (strcmp(argv[i], "-s") == 0) {
            print_symtbl = true;
        } else if (strcmp(argv[i], "-h") == 0) {
            printf("Simple RISC-V Assembler, version 0.1\n");
            printf("-i <file> Use <file> as source input (default: stdin)\n");
            printf("-o <file> Place the HEX text output into <file> (default: target.hex)\n");
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
        printf("can not create HEX text file %s!\n", output_filename.data());
        return 1;
    }
    string input_buffer = "";
    string equiv_part = "", text_seg = "", data_seg = "";
    get_source(input_buffer);
    split_segment(input_buffer, text_seg, data_seg);
    split_equiv(text_seg, equiv_part);
    split_equiv(data_seg, equiv_part);
    if (!scan_symbol(equiv_part, text_seg, data_seg)) {
        fclose(output_file);
        remove(output_filename.data());
        return 1;
    }
    if (print_symtbl) print_symbol();
    if (!gen_hex(output_file, text_seg, data_seg)) {
        fclose(output_file);
        remove(output_filename.data());
        return 1;
    }
    fclose(output_file);
    return 0;
}