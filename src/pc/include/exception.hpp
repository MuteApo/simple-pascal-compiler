#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include "defs.hpp"
#include <exception>
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
            : Exception("Redefine Error at line " + to_string(line_no) + ": Identifier \"" + id +
                        "\" is redefined.") {}
};

class UndefineError : public Exception {
  public:
    UndefineError(int line_no, std::string id)
            : Exception("Undefine Error at line " + to_string(line_no) + ": Identifier \"" + id +
                        "\" is undefined.") {}
};

class ExpressionTypeError : public Exception {
  public:
    ExpressionTypeError(int line_no, std::string expected, std::string got)
            : Exception("Expression Type Error at line " + to_string(line_no) + ": Expect type \"" +
                        expected + "\" but got \"" + got + "\".") {}
};

class LeftValueError : public Exception {
  public:
    LeftValueError(int line_no, std::string lval)
            : Exception("Left Value Error at line " + to_string(line_no) + ": " + to_string(lval) +
                        "\" is not a legal left-value.") {}
};

class IndexDimensionError : public Exception {
  public:
    IndexDimensionError(int line_no, int expected, int got)
            : Exception("Index Dimension Error at line " + to_string(line_no) +
                        ": Expect index dimension of " + to_string(expected) + " but got " +
                        to_string(got) + "\".") {}
};

class IndexTypeError : public Exception {
  public:
    IndexTypeError(int line_no, int order, std::string expected, std::string got)
            : Exception("Index Type Error at line " + to_string(line_no) + ": Expect type \"" +
                        expected + "\" but got \"" + got + "\" for index " + to_string(order) +
                        ".") {}
};

class MemberNotFoundError : public Exception {
  public:
    MemberNotFoundError(int line_no, std::string record, std::string member)
            : Exception("Member Not Found Error at line " + to_string(line_no) + ": Record \"" +
                        record + "\" does not have member named \"" + member + "\".") {}
};

class ArgumentNumberError : public Exception {
  public:
    ArgumentNumberError(int line_no, int expected, int got)
            : Exception("Argument Number Error at line " + to_string(line_no) + ": Expect " +
                        to_string(expected) + " arguments but got " + to_string(got) + "\".") {}
};

class ArgumentTypeError : public Exception {
  public:
    ArgumentTypeError(int line_no, int order, std::string expected, std::string got)
            : Exception("Argument Type Error at line " + to_string(line_no) + ": Expect type \"" +
                        expected + "\" but got \"" + got + "\" for argument " + to_string(order) +
                        ".") {}
};

#endif