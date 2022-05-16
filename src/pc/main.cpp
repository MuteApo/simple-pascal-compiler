#include "parser.tab.h"
#include "semantics.hpp"
#include "tree.hpp"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    FILE* viz_file = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            if (i + 1 >= argc) {
                printf("Source file path is missing!\n");
                return 1;
            }
            if (freopen(argv[i + 1], "r", stdin) == NULL) {
                printf("Unable to open source file!\n");
                return 1;
            }
            i += 1;
        } else if (strcmp(argv[i], "-o") == 0) {
            // TODO
        } else if (strcmp(argv[i], "-V") == 0) {
            if (i + 1 >= argc) {
                printf("Visualizing file path is missing!\n");
                return 1;
            }
            viz_file = fopen(argv[i + 1], "w+");
            if (viz_file == NULL) {
                printf("Unable to create visualizing file!\n");
                return 1;
            }
            i += 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            // TODO
        } else {
            printf("Unknown Argument!\n");
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