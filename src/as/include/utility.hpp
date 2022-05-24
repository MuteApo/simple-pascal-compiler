#ifndef _PREPROCESSOR_H_
#define _PREPROCESSOR_H_

#include <string>
#include <vector>

bool        is_literal_integer(std::string s, int32_t &val);
std::string trim(std::string str);
void        replace_all_char(std::string &s, char old_char, char new_char);
void        get_source(std::string &buffer);
void        split_segment(std::string buffer, std::string &text, std::string &data);
void        split_equiv(std::string &segment, std::string &equiv);

#endif