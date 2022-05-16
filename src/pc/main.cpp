#include "include/tree.hpp"
#include "include/semantics.hpp"
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    FILE* viz_file = NULL;
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
            if (i + 1 >= argc || argv[i + 1][0] == '-') {
                printf("missing filename after '-V'\n");
                return 1;
            }
            viz_file = fopen(argv[i + 1], "w+");
            if (viz_file == NULL) {
                printf("can not create visualization script %s\n", argv[i + 1]);
                return 1;
            }
            i += 1;
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
    semanticAnalyser sem = semanticAnalyser();
    sem.analyseTree(root);
    sem.st.printTable();
    sem.err.printError();
    if (viz_file != NULL) {
        fprintf(viz_file, "digraph g {");
        treeNode::traverse(viz_file, 0, root);
        fprintf(viz_file, "}");
        fclose(viz_file);
    }
    return 0;
}