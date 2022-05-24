#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "include/inst2code.hpp"
#include "include/preproc.hpp"
#include "include/reader.hpp"

using namespace std;

bool read_const(string const_def, string &const_name, int32_t &const_val) {
    replace_all_char(const_def, ',', ' ');
    stringstream ss(const_def);
    string       name_str, val_str;
    ss >> name_str >> val_str;
    char   *endptr = nullptr;
    int32_t val    = strtol(val_str.data(), &endptr, 0);
    if (val == 0 && endptr == val_str.data()) {
        cout << "In '.equiv': ";
        cout << "wrong format of constant" << endl;
        return false;
    }
    if (!ss.eof()) {
        cout << "In '.equiv': ";
        cout << "wrong number of arguments" << endl;
        return false;
    }
    const_name = name_str;
    const_val  = val;
    return true;
}

uint32_t id = 0;

vector<string> read_label(string &line, uint32_t &size, bool is_data_segment) {
    vector<string> labels;
    string         label;
    while (line.find_first_of(":") != string::npos) {
        label = line.substr(0, line.find_first_of(":"));  // ":" is not included
        line  = line.substr(line.find_first_of(":") + 1);
        if (label.find(' ') != string::npos) {
            if (is_data_segment) {
                cout << "In '.data': ";
            } else {
                cout << "In '.text': ";
            }
            cout << "illegal label in the middle" << endl;
            return vector<string>();
        }
        labels.push_back(label);
    }
    if (line != "") {  // Call read_data for calculating data size only
        if (is_data_segment) {
            vector<string> hex_strings;
            read_data(line, hex_strings, size);
        } else {
            string         operation;
            vector<string> operands;
            read_inst(line, operation, operands);
            size = get_inst_size(operation, operands);
        }
        line += "\n";
    } else
        size = 0;
    return labels;
}

bool read_inst(string line, string &operation, vector<string> &operands) {
    string operand;
    replace_all_char(line, ',', ' ');
    line = trim(line);
    stringstream ss(line);
    ss >> operation;
    while (ss >> operand) {
        if (operand.find_first_of('(') != string::npos) {
            if (operand.find_first_of('(') != operand.find_last_of('(')) {
                cout << "In instruction: ";
                cout << "multiple left parenthesis" << endl;
                return false;
            } else if (operand[operand.length() - 1] != ')') {
                cout << "In instruction: ";
                cout << "missing right parenthesis" << endl;
                return false;
            } else if (operand.find_first_of(')') != operand.find_last_of(')')) {
                cout << "In instruction: ";
                cout << "multiple right parenthesis" << endl;
                return false;
            }
            string operand_2 = operand.substr(0, operand.find('('));
            string operand_1 = operand.substr(operand.find('(') + 1);
            operand_1        = operand_1.substr(0, operand_1.length() - 1);
            operands.push_back(operand_1);
            operands.push_back(operand_2);
        } else {
            operands.push_back(operand);
        }
    }
    return true;
}

uint8_t data_align = 1;

bool read_data(string line, vector<string> &hex_strings, uint32_t &size) {
    // BUG: Comma in string is also substituted
    replace_all_char(line, ',', ' ');
    line = trim(line);
    stringstream ss(line);
    string       mode;
    ss >> mode;
    if (mode == ".asciiz") {
        string str;
        getline(ss, str);
        str = trim(str);
        // BUG: Weak check for parenthesis matching
        if (str[0] != '\"' || str[str.length() - 1] != '\"') {
            cout << "In '.data': ";
            cout << "wrong format of string" << endl;
            return false;
        }
        str = str.substr(1, str.length() - 2);
        char hex[5];
        for (int i = 0; i < str.length(); i++) {
            sprintf(hex, "0x%02X", str[i]);
            hex_strings.push_back(string(hex));
        }
        hex_strings.push_back("0x00");  // '\0'
        size = str.length() + 1;
        if (size % data_align != 0) {
            uint32_t byte_to_fill = data_align - size % data_align;
            for (int i = 0; i < byte_to_fill; i++) {
                hex_strings.push_back("0x00");
                size++;
            }
        }
    } else if (mode == ".align") {
        size = 0;
        string align_str;
        ss >> align_str;
        if (!ss.eof()) {
            cout << "In '.data': ";
            cout << "wrong number of argument" << endl;
            return false;
        }
        char   *endptr = nullptr;
        int32_t val    = strtol(align_str.data(), &endptr, 0);
        if (val == 0 && endptr == align_str.data()) {
            cout << "In '.data': ";
            cout << "wrong format of argument" << endl;
            return false;
        } else if (val == 0 || val % 2 != 0) {
            cout << "In '.data': ";
            cout << "align value is not times of 2" << endl;
            return false;
        }
        data_align = val;
    } else if (mode == ".byte" || mode == ".word") {
        string elem;
        size = 0;
        while (ss >> elem) {
            char   *endptr = nullptr;
            int32_t val    = strtol(elem.data(), &endptr, 0);
            if (val == 0 && endptr == elem.data()) {
                cout << "In '.data': ";
                cout << "wrong format of element" << endl;
                return false;
            }
            char hex[11];
            if (mode == ".byte") {
                if ((val & 0xFF) != val) {
                    cout << "In '.data': ";
                    cout << "element is too big" << endl;
                    return false;
                }
                sprintf(hex, "0x%02X", val);
                size += 1;
            } else if (mode == ".word") {
                sprintf(hex, "0x%08X", val);
                size += 4;
            }
            hex_strings.push_back(string(hex));
        }
        if (size % data_align != 0) {
            uint32_t byte_to_fill = data_align - size % data_align;
            for (int i = 0; i < byte_to_fill; i++) {
                hex_strings.push_back("0x00");
                size++;
            }
        }
    } else {
        cout << "In .data: ";
        cout << "directive not found" << endl;
        return false;
    }
    return true;
}