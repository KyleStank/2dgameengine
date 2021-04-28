#ifndef ENGINE_UTIL_H
#define ENGINE_UTIL_H

#include <string>
#include <vector>

namespace engine::util
{
    std::vector<std::string> str_split(const std::string& str, const char& delimiter);
}

#endif
