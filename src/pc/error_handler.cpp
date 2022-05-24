#include "include/error_handler.hpp"

ErrorHandler error_handler;

ErrorHandler::ErrorHandler() {
    errors.clear();
}

void ErrorHandler::addMsg(Exception e) {
    errors.push_back(e);
}

std::string ErrorHandler::dump() {
    std::string result = "";
    for (Exception e : errors) result += e.what();
    return result;
}