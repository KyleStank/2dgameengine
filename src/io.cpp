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

// Returns the number of lines within a file.
int engine::io::get_line_count(const std::string& path)
{
    int count = 0;
    std::ifstream ifs(path);
    std::string current_line;
    while (std::getline(ifs, current_line))
    {
        count++;
    }
    return count;
}

// Returns the value of a specific line.
std::string engine::io::read_line(const std::string& path, const int& line)
{
    int line_count = get_line_count(path);
    std::ifstream ifs(path);
    std::string current_line;
    for (int i = 0; std::getline(ifs, current_line) && i < line_count; i++)
    {
        if (i + 1 == line)
        {
            return current_line;
        }
    }

    return "";
}
