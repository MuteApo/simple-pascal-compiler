#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include "defs.hpp"
#include <exception>
#include <iostream>
#include <string>

class Exception {
  private:
    int         line;
    std::string msg;

  public:
    Exception(int l, std::string m) : line(l), msg(m) {}

    bool operator<(const Exception &other) const {
        return line < other.line;
    }

    std::string what() {
        return "Line " + to_string(line) + msg;
    }
};

class RedefineError : public Exception {
  public:
    RedefineError(int line_no, std::string id)
            : Exception(line_no,
                        "(\033[31mRedefineError\033[0m): Identifier \"\033[33m" + id +
                            "\033[0m\" is redefined.") {
        std::cout << what() << std::endl;
    }
};

class UndefineError : public Exception {
  public:
    UndefineError(int line_no, std::string id)
            : Exception(line_no,
                        "(\033[31mUndefineError\033[0m): Identifier \"\033[33m" + id +
                            "\033[0m\" is undefined.") {
        std::cout << what() << std::endl;
    }
};

class ExpressionTypeError : public Exception {
  public:
    ExpressionTypeError(int line_no, std::string expected, std::string got)
            : Exception(line_no,
                        "(\033[31mExpressionTypeError\033[0m): Expect type \"\033[32m" + expected +
                            "\033[0m\" but got \"\033[33m" + got + "\033[0m\".") {
        std::cout << what() << std::endl;
    }
};

class IndexDimensionError : public Exception {
  public:
    IndexDimensionError(int line_no, int expected, int got)
            : Exception(line_no,
                        "(\033[31mIndexDimensionError\033[0m): Expect index dimension of \033[32m" +
                            to_string(expected) + "\033[0m but got \033[33m" + to_string(got) +
                            "\033[0m.") {
        std::cout << what() << std::endl;
    }
};

class IndexTypeError : public Exception {
  public:
    IndexTypeError(int line_no, int order, std::string expected, std::string got)
            : Exception(line_no,
                        "(\033[31mIndexTypeError\033[0m): Expect type \"\033[32m" + expected +
                            "\033[0m\" but got \"\033[33m" + got + "\033[0m\" for index \033[34m" +
                            to_string(order) + "\033[0m.") {
        std::cout << what() << std::endl;
    }
};

class MemberNotFoundError : public Exception {
  public:
    MemberNotFoundError(int line_no, std::string record, std::string member)
            : Exception(line_no,
                        "(\033[31mMemberNotFoundError\033[0m): Record \"\033[33m" + record +
                            "\033[0m\" does not have member named \"\033[33m" + member +
                            "\033[0m\".") {
        std::cout << what() << std::endl;
    }
};

class ArgumentNumberError : public Exception {
  public:
    ArgumentNumberError(int line_no, int expected, int got)
            : Exception(line_no,
                        "(\033[31mArgumentNumberError\033[0m): Expect \033[32m" +
                            to_string(expected) + "\033[0m arguments but got \033[33m" +
                            to_string(got) + "\033[0m.") {
        std::cout << what() << std::endl;
    }
};

class ArgumentTypeError : public Exception {
  public:
    ArgumentTypeError(int line_no, int order, std::string expected, std::string got)
            : Exception(line_no,
                        "(\033[31mArgumentTypeError\033[0m): Expect type \"\033[32m" + expected +
                            "\033[0m\" but got \"\033[33m" + got +
                            "\033[0m\" for argument \033[34m" + to_string(order) + "\033[0m.") {
        std::cout << what() << std::endl;
    }
};

#endif