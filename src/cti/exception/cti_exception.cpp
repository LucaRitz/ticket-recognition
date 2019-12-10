#include <include/exception/cti_exception.hpp>

cti::CtiException::CtiException(const std::string& message) : exception(message.c_str()) {
}