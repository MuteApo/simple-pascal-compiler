#ifndef _ERRORHANDLER_H
#define _ERRORHANDLER_H

#include "viz.hpp"
#include <iostream>
#include <string>
#include <vector>

class errorRecord {
  private:
    errorType   err_type;
    std::string name;
    int         line;

  public:
    errorRecord(errorType e, std::string n, int l) : err_type(e), name(n), line(l) {}
    std::string toString() {
        switch (err_type) {
            case UNDEFINED:
                return "line " + std::to_string(line) + ": Undefined identifier \"" + name + "\"";
            case REDEFINE:
                return "line " + std::to_string(line) + ": Duplicate identifier \"" + name + "\"";
        }
        return "";
    }
};

class errorHandler {
  public:
    std::vector<errorRecord> errors;

    errorHandler() {}
    void add(errorType e, std::string n, int l) {
        errorRecord newrecord = errorRecord(e, n, l);
        errors.push_back(newrecord);
    }
    std::string toString() {
        std::string s = "";
        for (int i = 0; i < errors.size(); i++) {
            s = s + "[" + std::to_string(i) + "]" + errors[i].toString() + "\n";
        }
        return s;
    }
    void printError() {
        std::cout << toString() << std::endl;
    }
};

#endif