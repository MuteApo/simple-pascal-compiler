#include <stdio.h>
#include <string>
#include <vector>

using namespace std;
#define XLEN 32

/******************** Supported Assembler Directives ********************/
// .org     Set origin of the progrom executable part
// .text	Store subsequent instructions in the text segment
// .data	Store subsequent items in the static segment
// .align	Can be only used in data segment, control the alignment below
// .byte	Store listed values as 8-bit bytes
// .word	Store listed values as unaligned 32-bit words
// [.byte and .word data is splitted with comma, format: dec or hex('0x')]
// .asciiz	Store subsequent string in the data segment and add '\0'

// 'label:' is also supported to generate symbol table

/******************** Supported RV32I Instruction ********************/
// I-Type: addi slti sltiu andi ori xori slli srli srai nop  
// R-Type: add sub slt sltu and or xor sll srl sra
// I-Type: lb lh lw ld lbu lhu lwu
// S-Type: sb sh sw sd
// B-Type: beq bne blt bltu bge
// J-Type: jal, I-Type: jalr
// U-Type: lui auipc
// SYSTEM-Type: ecall ebreak



string inst_table[] = {};

vector<string> scanner(void){ //TODO
    vector<string> tokens;
    string token = "";
    while(1){
        char c = getchar();
        if(!c) break;
        if(c == ' ' || c == '\t' || c == '\r' || c == '\n'){
            tokens.push_back(token);
            token = "";
        }else token += c;
    }
    return tokens;
}

void parser(vector<string> tokens, FILE *output){

}

int main(int argc, char* argv[]){
    string output_filename = "target.bin";
    FILE *output_file = NULL;
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-i") == 0){
            if(i + 1 >= argc || argv[i + 1][0] == '-'){
                printf("Input filename not found!\n");
                return 1;
            }
            if(freopen(argv[i + 1], "r", stdin) == NULL){
                printf("Unable to open file: %s!\n", argv[i + 1]);
                return 1;
            }
            i += 1;
        }else if(strcmp(argv[i], "-o") == 0){
            if(i + 1 >= argc || argv[i + 1][0] == '-'){
                printf("Output filename not found!\n");
                return 1;
            }
            output_filename = string(argv[i + 1]);
            i += 1;
        }else if(strcmp(argv[i], "-h") == 0){
            printf("Simple RISC-V Assembler, version 0.1, support all RV32I instruction.\n\n");
            printf("-i <Filename> Use the given input assembly file name (default: stdin)\n");
            printf("-o <Filename> Use the given binary file name (default: target.bin)");
            printf("-h Show this version and help information\n"); 
        }else{
            printf("Unknown argument: %s!\n", argv[i]);
            return 1;
        }
    }
    output_file = fopen(output_filename.data(), "w+");
    if(output_file == NULL){
        printf("Unable to create file: %s!\n", output_filename);
        return 1;
    }
    vector<string> tokens = scanner();
    parser(tokens, output_file);
    return 0;
}