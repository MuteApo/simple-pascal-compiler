#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include "defs.hpp"
#include <exception>
#include <iostream>
#include <string>

class Exception {
  private:
    std::string msg;

  public:
    Exception(std::string m) : msg(m) {}
    const std::string &what() {
        return msg;
    }
};

class RedefineError : public Exception {
  public:
    RedefineError(int line_no, std::string id)
            : Exception("RedefineError at line " + to_string(line_no) + ": Identifier \"" + id +
                        "\" is redefined.") {
        std::cout << what() << std::endl;
    }
};

class UndefineError : public Exception {
  public:
    UndefineError(int line_no, std::string id)
            : Exception("UndefineError at line " + to_string(line_no) + ": Identifier \"" + id +
                        "\" is undefined.") {
        std::cout << what() << std::endl;
    }
};

class ExpressionTypeError : public Exception {
  public:
    ExpressionTypeError(int line_no, std::string expected, std::string got)
            : Exception("ExpressionTypeError at line " + to_string(line_no) + ": Expect type \"" +
                        expected + "\" but got \"" + got + "\".") {
        std::cout << what() << std::endl;
    }
};

class LeftValueError : public Exception {
  public:
    LeftValueError(int line_no, std::string lval)
            : Exception("LeftValueError at line " + to_string(line_no) + ": " + to_string(lval) +
                        "\" is not a legal left-value.") {
        std::cout << what() << std::endl;
    }
};

class IndexDimensionError : public Exception {
  public:
    IndexDimensionError(int line_no, int expected, int got)
            : Exception("IndexDimensionError at line " + to_string(line_no) +
                        ": Expect index dimension of " + to_string(expected) + " but got " +
                        to_string(got) + ".") {
        std::cout << what() << std::endl;
    }
};

class IndexTypeError : public Exception {
  public:
    IndexTypeError(int line_no, int order, std::string expected, std::string got)
            : Exception("IndexTypeError at line " + to_string(line_no) + ": Expect type \"" +
                        expected + "\" but got \"" + got + "\" for index " + to_string(order) +
                        ".") {
        std::cout << what() << std::endl;
    }
};

class MemberNotFoundError : public Exception {
  public:
    MemberNotFoundError(int line_no, std::string record, std::string member)
            : Exception("MemberNotFoundError at line " + to_string(line_no) + ": Record \"" +
                        record + "\" does not have member named \"" + member + "\".") {
        std::cout << what() << std::endl;
    }
};

class ArgumentNumberError : public Exception {
  public:
    ArgumentNumberError(int line_no, int expected, int got)
            : Exception("ArgumentNumberError at line " + to_string(line_no) + ": Expect " +
                        to_string(expected) + " arguments but got " + to_string(got) + ".") {
        std::cout << what() << std::endl;
    }
};

class ArgumentTypeError : public Exception {
  public:
    ArgumentTypeError(int line_no, int order, std::string expected, std::string got)
            : Exception("ArgumentTypeError at line " + to_string(line_no) + ": Expect type \"" +
                        expected + "\" but got \"" + got + "\" for argument " + to_string(order) +
                        ".") {
        std::cout << what() << std::endl;
    }
};

#endif