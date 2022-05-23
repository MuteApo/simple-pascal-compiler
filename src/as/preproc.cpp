#include <map>
#include <string>
#include <vector>

#include "include/symbol.hpp"

using namespace std;

bool is_blank(char c) {
    if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
        return true;
    else
        return false;
}

bool is_cr_lf(char c) {
    if (c == '\n' || c == '\r')
        return true;
    else
        return false;
}

string trim(string str) {
    size_t first_valid_char = str.find_first_not_of(" \t");
    if (first_valid_char == string::npos) {
        return "";
    } else {
        size_t last_valid_char = str.find_last_not_of(" \t");
        return str.substr(first_valid_char, last_valid_char - first_valid_char + 1);
    }
}

void remove_empty_line(string &str) {
    string buf = "", line = "";
    for (int i = 0; i < str.length(); i++) {
        if (is_cr_lf(str[i])) {
            line = trim(line);
            if (line != "") buf += line + "\n";
            line = "";
            continue;
        }
        line += str[i];
    }
    if (line != "") buf += line + "\n";
    str = buf;
}

void replace_all_char(string &s, char old_char, char new_char) {
    string new_str = "";
    new_str += new_char;
    while (s.find(old_char) != string::npos) {
        size_t pos = s.find(old_char);
        s          = s.replace(pos, 1, new_str);
    }
}

void get_source(string &buffer) {
    char c = getchar();
    while (c != EOF) {
        buffer += c;
        c = getchar();
    }
}

void split_segment(string buffer, string &text, string &data) {
    string word = "", temp = "";
    bool   in_text = true, in_comment = false;
    for (int i = 0; i < buffer.length(); i++) {
        if (buffer[i] == '#') {
            in_comment = true;
            continue;
        } else if (in_comment && is_cr_lf(buffer[i])) {
            in_comment = false;
            continue;
        } else if (in_comment) {
            continue;
        }
        word += buffer[i];
        temp += buffer[i];
        if (is_blank(buffer[i])) {
            if (word.substr(0, word.length() - 1) == ".data") {
                // Remove '.data' and the blank after it
                if (in_text) {
                    text += temp.substr(0, temp.length() - 6);
                    in_text = false;
                    temp = word = "";
                } else {
                    temp = temp.substr(0, temp.length() - 6);
                    word = "";
                }
            } else if (word.substr(0, word.length() - 1) == ".text") {
                if (!in_text) {
                    data += temp.substr(0, temp.length() - 6);
                    in_text = true;
                    temp = word = "";
                } else {
                    temp = temp.substr(0, temp.length() - 6);
                    word = "";
                }
            } else
                word = "";
        }
        if (i == (buffer.length() - 1)) {
            if (in_text)
                text += temp;
            else
                data += temp;
        }
    }
    remove_empty_line(text);
    remove_empty_line(data);
}

void split_equiv(string &segment, string &equiv) {
    string line = "", buf = "";
    for (int i = 0; i < segment.length(); i++) {
        line += segment[i];
        if (is_cr_lf(segment[i])) {
            if (trim(line).substr(0, 6) == ".equiv") {
                equiv += trim(line).substr(6);
            } else {
                buf += line;
            }
            line = "";
        }
    }
    segment = buf;
    remove_empty_line(segment);
    remove_empty_line(equiv);
}