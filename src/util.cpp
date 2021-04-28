#include <sstream>
#include <string>
#include <vector>
#include "util.h"

// Splits a string into an array based on the provided delimiter.
std::vector<std::string> engine::util::str_split(const std::string& str, const char& delimiter)
{
    std::vector<std::string> results;
    std::stringstream sstream(str);
    while (sstream.good())
    {
        std::string substr;
        getline(sstream, substr, delimiter);
        results.push_back(substr);
    }
    return results;
}
