#ifndef __ERROR_HANDLER_H__
#define __ERROR_HANDLER_H__

#include "exception.hpp"
#include <string>
#include <vector>

class ErrorHandler {
  private:
    std::vector<Exception> errors;

  public:
    ErrorHandler();

    int getErrorCount() ;

    void addMsg(Exception e);

    std::string dump();
};

extern ErrorHandler error_handler;

#endif