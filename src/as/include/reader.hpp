#ifndef _READER_H_
#define _READER_H_

#include <string>
#include <vector>

bool read_const(std::string const_def, std::string &const_name, int32_t &const_val);
std::vector<std::string> read_label(std::string &line, uint32_t &size, bool is_data_segment);
bool read_inst(std::string line, std::string &operation, std::vector<std::string> &operands);
bool read_data(std::string line, std::vector<std::string> &hex_strings, uint32_t &size);

#endif