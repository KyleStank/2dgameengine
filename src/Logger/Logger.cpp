#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include "logger.h"

std::vector<engine::LogEntry> engine::Logger::messages;

std::string engine::Logger::GetFormattedDateTime()
{
    std::time_t now;
    std::time(&now);
    std::tm* tm = std::localtime(&now);

    std::ostringstream oss;
    oss << tm->tm_mon + 1 << '/'
        << MONTH_STRING[tm->tm_mon] << '/'
        << tm->tm_year + 1900 << ' '
        << tm->tm_hour << ':'
        << tm->tm_min << ':'
        << tm->tm_sec;
    return oss.str();
}

std::string engine::Logger::GetBaseLogMessage(const std::string& prefix, const std::string& msg)
{
    std::ostringstream oss;
    oss << "[" << prefix << "] ["
        << GetFormattedDateTime() << "] " << msg;
    return oss.str();
}

void engine::Logger::Log(const std::string& msg)
{
    LogEntry logEntry;
    logEntry.type = LogType::INFO;
    logEntry.message = Logger::GetBaseLogMessage("INFO", msg);

    std::cout << "\033[1;37m" << logEntry.message << "\033[0m" << std::endl;

    Logger::messages.push_back(logEntry);
}

void engine::Logger::Warn(const std::string& msg)
{
    LogEntry logEntry;
    logEntry.type = LogType::WARNING;
    logEntry.message = Logger::GetBaseLogMessage("WARN", msg);

    std::cout << "\033[1;33m" << logEntry.message << "\033[0m" << std::endl;

    Logger::messages.push_back(logEntry);
}

void engine::Logger::Error(const std::string& msg)
{
    LogEntry logEntry;
    logEntry.type = LogType::ERROR;
    logEntry.message = Logger::GetBaseLogMessage("ERROR", msg);

    std::cerr << "\033[1;31m" << logEntry.message << "\033[0m" << std::endl;

    Logger::messages.push_back(logEntry);
}
