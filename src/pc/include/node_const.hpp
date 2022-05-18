#ifndef _NODE_CONST_H_
#define _NODE_CONST_H_

#include <string>

enum const_kind { const_int = 200001, const_real, const_char, const_bool, const_string };

class ConstDefNode {
  public:
    std::string name;
    const_kind  kind;

    std::string toString() {
        return "";
    }
};

#endif
