#ifndef ENGINE_IO_H
#define ENGINE_IO_H

#include <fstream>
#include <string>
#include <vector>

namespace engine::io
{
    std::string Read(const std::string& path);
    std::vector<std::string> ReadAllLines(const std::string& path);

    int GetLineCount(const std::string& path);
    std::string ReadLine(const std::string& path, const int& line);
}

#endif
