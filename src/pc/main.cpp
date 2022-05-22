#include "include/node_block.hpp"
#include "include/node_const.hpp"
#include "include/node_expr.hpp"
#include "include/node_func.hpp"
#include "include/node_stmt.hpp"
#include "include/node_type.hpp"
#include "include/node_var.hpp"
// #include "include/semantics.hpp"
#include "parser.tab.h"
#include <cstring>
#include <fstream>
#include <iostream>

#include "include/asmgen.hpp"

extern ProgramNode *root;

int main(int argc, char *argv[]) {
    FILE *viz_file_0 = NULL, *viz_file_1 = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            if (i + 1 >= argc || argv[i + 1][0] == '-') {
                printf("missing filename after '-i'\n");
                return 1;
            }
            if (freopen(argv[i + 1], "r", stdin) == NULL) {
                printf("can not open source file %s\n", argv[i + 1]);
                return 1;
            }
            i += 1;
        } else if (strcmp(argv[i], "-o") == 0) {
            // TODO
        } else if (strcmp(argv[i], "-V") == 0) {
            if (i + 2 >= argc || argv[i + 1][0] == '-' || argv[i + 2][0] == '-') {
                printf("missing filename after '-V'\n");
                return 1;
            }
            viz_file_0 = fopen(argv[i + 1], "w+");
            if (viz_file_0 == NULL) {
                printf("can not create visualization script %s\n", argv[i + 1]);
                return 1;
            }
            viz_file_1 = fopen(argv[i + 2], "w+");
            if (viz_file_1 == NULL) {
                printf("can not create visualization script %s\n", argv[i + 2]);
                return 1;
            }
            i += 2;
        } else if (strcmp(argv[i], "-h") == 0) {
            printf("Simple Pascal Compiler, with ISO 7185 language standard, version 0.1:\n");
            printf("-i <file> Use <file> as source input (default: stdin)\n");
            printf("-o <file> Place the assembly output into <file> (default: assembly.S)\n");
            printf("-V <file> Generate Graphviz visualization script <file> for parse tree\n");
            printf("-h Show this information\n");
            return 0;
        } else {
            printf("unrecognized command line option %s\n", argv[i]);
            return 1;
        }
    }
    yyparse();
    if (viz_file_0 != NULL) {
        fprintf(viz_file_0, "%s", root->gen_viz_code().c_str());
        fclose(viz_file_0);
    }
    root->visit();
    if (viz_file_1 != NULL) {
        fprintf(viz_file_1, "%s", root->gen_viz_code().c_str());
        fclose(viz_file_1);
    }
    start_asm("bin/test.S");
    write_segment(get_integer_calc("mod", false), false);
    finish_asm();
    return 0;
}