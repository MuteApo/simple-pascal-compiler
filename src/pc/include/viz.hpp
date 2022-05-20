#ifndef __VIZ_H__
#define __VIZ_H__

#include <sstream>
#include <string>

std::string vizNode(int uid, std::string info);

std::string vizChildEdge(int fa_uid, int son_uid);

template <class T> std::string to_string(T v) {
    std::ostringstream oss;
    oss << std::boolalpha << v;
    return oss.str();
}

#endif