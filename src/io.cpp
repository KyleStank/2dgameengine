#include <fstream>
#include <string>
#include <vector>
#include "io.h"

// Reads content of file and returns as string.
std::string engine::io::Read(const std::string& path)
{
    std::ifstream ifs(path);
    return std::string(
        std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>()
    );
}

// Reads every line of a file and returns the result as an array.
std::vector<std::string> engine::io::ReadAllLines(const std::string& path)
{
    std::vector<std::string> lines;

    std::ifstream ifs(path);
    std::string currentLine;
    while (std::getline(ifs, currentLine))
    {
        lines.push_back(currentLine);
    }

    return lines;
}

// Returns the number of lines within a file.
int engine::io::GetLineCount(const std::string& path)
{
    int count = 0;

    std::ifstream ifs(path);
    std::string currentLine;
    while (std::getline(ifs, currentLine))
    {
        count++;
    }

    return count;
}

// Returns the value of a specific line.
std::string engine::io::ReadLine(const std::string& path, const int& line)
{
    int lineCount = GetLineCount(path);

    std::ifstream ifs(path);
    std::string currentLine;
    for (int i = 0; std::getline(ifs, currentLine) && i < lineCount; i++)
    {
        if (i + 1 == line)
        {
            return currentLine;
        }
    }

    return "";
}
