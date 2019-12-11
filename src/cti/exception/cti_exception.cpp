#include <include/exception/cti_exception.hpp>

cti::CtiException::CtiException(std::string message) : _message(std::move(message)) {
}

const char* cti::CtiException::what() const noexcept {
    return _message.c_str();
}