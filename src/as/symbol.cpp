#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "include/inst2code.hpp"
#include "include/reader.hpp"

using namespace std;

map<string, uint32_t> label_table;
map<string, int32_t>  const_table;

bool insert_labels(string &segment, bool is_data_segment) {
    string       line, buf = "";
    stringstream ss(segment);
    uint32_t     line_size;
    while (getline(ss, line)) {
        vector<string> labels;
        labels = read_label(line, line_size, is_data_segment);
        for (int i = 0; i < labels.size(); i++) {
            if (label_table.count(labels[i]) != 0) {
                if (is_data_segment) {
                    cout << "In .data: ";
                } else {
                    cout << "In .text: ";
                }
                cout << "label redefined" << endl;
                return false;
            }
            label_table[labels[i]] = addr_counter;
        }
        if (line != "") {
            buf += line;
            addr_counter += line_size;
        }
    }
    segment = buf;
    return true;
}

bool scan_symbol(string equiv, string &text, string &data) {
    string       line;
    stringstream ss(equiv);
    string       const_name;
    int32_t      const_val;
    while (getline(ss, line)) {
        bool succeed = read_const(line, const_name, const_val);
        if (!succeed) return false;
        if (const_table.count(const_name) != 0) {
            cout << "In .equiv: ";
            cout << "constant redefined" << endl;
            return false;
        }
        const_table[const_name] = const_val;
    }
    addr_counter = 0;
    if (!insert_labels(text, false)) return false;
    if (!insert_labels(data, true)) return false;
    return true;
}

void print_symbol(void) {
    map<string, int32_t>::iterator const_p;
    cout << "Constants: " << endl;
    if (const_table.size() == 0) cout << "<Empty>" << endl;
    for (const_p = const_table.begin(); const_p != const_table.end(); const_p++) {
        cout << "Name: " << const_p->first << "\t\t";
        cout << "Value: " << const_p->second << endl;
    }
    map<string, uint32_t>::iterator label_p;
    cout << endl << "Labels: " << endl;
    if (label_table.size() == 0) cout << "<Empty>" << endl;
    for (label_p = label_table.begin(); label_p != label_table.end(); label_p++) {
        cout << "Name: " << label_p->first << "\t\t";
        printf("Address: 0x%08X\n", label_p->second);
    }
}