#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include <string>

bool scan_symbol(std::string equiv, std::string &text, std::string &data);
bool get_symbol_const(std::string name, int32_t &val);
bool get_symbol_label(std::string name, uint32_t &val);
void print_symbol(void);

#endif