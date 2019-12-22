#include <string>
#include <algorithm>
#include "file_util.hpp"

const char kPathSeparator =
#ifdef _WIN32
        '\\';
#else
        '/';
#endif

std::string cti::io::replaceDirectorySeparator(std::string_view value) {
    std::string normalizedPath(value);
    if ('\\' == kPathSeparator) {
        std::replace(normalizedPath.begin(), normalizedPath.end(), '/', kPathSeparator);
    }
    return normalizedPath;
}
