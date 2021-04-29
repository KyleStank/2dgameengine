#ifndef ENGINE_IO_H
#define ENGINE_IO_H

#include <fstream>
#include <string>
#include <vector>

namespace engine::io
{
    std::string read(const std::string& path);
    std::vector<std::string> read_all_lines(const std::string& path);

    int get_line_count(const std::string& path);
    std::string read_line(const std::string& path, const int& line);
}

#endif
