#pragma once

#include <exception>
#include <string>
#include <include/dllmacro.h>

namespace cti {
    class EXPORT CtiException : public std::exception {
    public:
        explicit CtiException(const std::string& message);
    };
}