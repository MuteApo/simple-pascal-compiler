#ifndef _ASM_GEN_
#define _ASM_GEN_

#include <string>
#include <vector>

std::string asm_code;
bool in_code_segment;
bool is_asm_empty;

bool start_asm(std::string filename, std::string origin_label = "main"){
    asm_code = "";
    in_code_segment = true;
    is_asm_empty = true;
}

void enter_segment(bool seg_sel){ // seg_sel: 0-code segment, 1-data segment

}

void insert_data(char* data_buf){

}

void insert_stmt_if(){
    
}

void insert_func(){

}




#endif
