#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <string>

using namespace std;

void remove_all(string &s, char c) {
    while (s.find(c) != string::npos) {
        size_t space_pos = s.find(c);
        s.replace(space_pos, 1, "");
    }
}

int main(int argc, char *argv[]) {
    string output_filename = "output.bin";
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            if (i + 1 >= argc || argv[i + 1][0] == '-') {
                printf("missing filename after '-i'\n");
                return 1;
            }
            if (freopen(argv[i + 1], "r", stdin) == NULL) {
                printf("can not open file %s\n", argv[i + 1]);
                return 1;
            }
            i += 1;
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc || argv[i + 1][0] == '-') {
                printf("missing filename after '-o'\n");
                return 1;
            }
            output_filename = string(argv[i + 1]);
            i += 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            printf("HEX Vector Text File to Binary File Converter\n");
            printf("HEX vector can be 1, 2, 4 bytes wide");
            printf("Binary is in little-endian\n");
            printf("Please put each vector in a line\n");
            printf("-i <file> Use text <file> as input (default: stdin)\n");
            printf("-o <file> Place output into <file>\n");
            printf("-h Show this information\n");
            return 0;
        } else {
            printf("unrecognized command line option %s\n", argv[i]);
            return 1;
        }
    }
    FILE *binary_file = fopen(output_filename.data(), "wb+");
    if (binary_file == NULL) {
        printf("can not create file %s\n", output_filename.data());
        return 1;
    }
    fseek(binary_file, 0, SEEK_SET);
    string hex_vector;
    while (!cin.eof()) {
        getline(cin, hex_vector);
        if (hex_vector.length() > 2) {
            string lead = hex_vector.substr(0, 2);
            if (lead == "0x" || lead == "0X") {
                hex_vector = hex_vector.substr(2);
            }
        }
        remove_all(hex_vector, ' ');
        remove_all(hex_vector, '\t');
        remove_all(hex_vector, '\r');
        remove_all(hex_vector, '\n');
        int vector_size = hex_vector.length();
        if (vector_size != 0 && vector_size != 2 && vector_size != 4 &&
            vector_size != 8) {
            printf("unsupported vector format\n");
            return 1;
        }
        stack<uint8_t> bytes;
        uint8_t byte = 0;
        for (int i = 0; i < hex_vector.length(); i++) {
            if (hex_vector[i] >= 'a' && hex_vector[i] <= 'z') {
                hex_vector[i] = hex_vector[i] - 'a' + 'A';
            }
            if (!(hex_vector[i] >= '0' && hex_vector[i] <= '9') &&
                !(hex_vector[i] >= 'A' && hex_vector[i] <= 'Z')) {
                printf("illegal character %c\n", hex_vector[i]);
                return 1;
            }
            if (i % 2 == 0)
                byte = 0;
            if (hex_vector[i] >= '0' && hex_vector[i] <= '9') {
                byte |= (hex_vector[i] - '0') << ((i % 2 == 0) ? 4 : 0);
            } else if (hex_vector[i] >= 'A' && hex_vector[i] <= 'Z') {
                byte |= (hex_vector[i] - 'A' + 10) << ((i % 2 == 0) ? 4 : 0);
            }
            if (i % 2 != 0)
                bytes.push(byte);
        }
        while (!bytes.empty()) {
            byte = bytes.top();
            fwrite(&byte, 1, 1, binary_file);
            bytes.pop();
        }
    }
    fclose(binary_file);
    return 0;
}