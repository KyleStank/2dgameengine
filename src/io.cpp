#include <fstream>
#include <string>
#include <vector>
#include "io.h"

// Reads content of file and returns as string.
std::string engine::io::read(const std::string& path)
{
    std::ifstream ifs(path);
    return std::string(
        std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>()
    );
}

// Reads every line of a file and returns the result as an array.
std::vector<std::string> engine::io::read_all_lines(const std::string& path)
{
    std::vector<std::string> lines;
    std::ifstream ifs(path);
    std::string current_line;
    while (std::getline(ifs, current_line))
    {
        lines.push_back(current_line);
    }
    return lines;
}
