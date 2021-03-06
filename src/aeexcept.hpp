////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef AEEXCEPT_HPP_INCLUDE_GUARD
#define AEEXCEPT_HPP_INCLUDE_GUARD 1

////////////////////////////////////////////////////////////////////////////////

#include <exception>
#include <string>

////////////////////////////////////////////////////////////////////////////////

class aeExceptionBase : public std::exception {
public:
    aeExceptionBase(): mMessage() {
    }

    aeExceptionBase(
        const std::string &message
    ): mMessage(message) {
    }

    ~aeExceptionBase() throw() {}

    const char *what() const noexcept {
        return mMessage.c_str();
    }

private:
    std::string mMessage;
};

////////////////////////////////////////////////////////////////////////////////

class aeException : public aeExceptionBase {
public:
    aeException() {}
    aeException(const std::string &message): aeExceptionBase(message) {}
};

////////////////////////////////////////////////////////////////////////////////

#define AE_EXCEPTION(B, T, P) \
class T : public B { \
public: \
    T(): B(P) {} \
    T(const std::string &message): B(std::string(P) + ": " + message) {} \
}

AE_EXCEPTION(aeException, aeInternalError, "internal error");
AE_EXCEPTION(aeException, aeNotImplementedError, "feature not implemented");
AE_EXCEPTION(aeException, aeArgumentError, "invalid argument");
AE_EXCEPTION(aeException, aeInvalidStateError, "invalid object state");

////////////////////////////////////////////////////////////////////////////////

#endif // AEEXCEPT_HPP_INCLUDE_GUARD

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

