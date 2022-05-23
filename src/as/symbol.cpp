#include <map>
#include <string>

#include "include/inst2code.hpp"
#include "include/symbol.hpp"

using namespace std;

map<string, uint32_t> label_table;
map<string, uint32_t> equiv_table;

void scan_symbol(string equiv, string text, string data) {}