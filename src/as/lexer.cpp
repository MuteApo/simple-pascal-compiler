#include <map>
#include <string>

#include "include/symbol.hpp"

using namespace std;

static map<string, uint8_t> pseudo_size = {{"la", 2}, {"li", 2}, {"j", 1}, {"jr", 1}};

void get_source(string &buffer) {
    char c = getchar();
    while (c != EOF) {
        buffer += c;
        c = getchar();
    }
}

void split_segment(string buffer, string &equiv, string &text, string &data) {
    for (int i = 0; i < buffer.length(); i++) {}
}