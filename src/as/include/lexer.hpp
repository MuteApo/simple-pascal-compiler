#ifndef _LEXER_H_
#define _LEXER_H_

#include <string>

void get_source(std::string &buffer);
void split_segment(std::string buffer, std::string &equiv, std::string &text, std::string &data);

#endif