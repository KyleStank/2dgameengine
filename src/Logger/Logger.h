#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

enum LogType
{
    INFO = 0,
    WARNING = 1,
    ERROR = 2
};

struct LogEntry
{
    LogType type;
    std::string message;
};

class Logger
{
    private:
        static const std::string _monthString[];

        static std::string GetFormattedDateTime();
        static std::string GetBaseLogMessage(const std::string& prefix, const std::string& msg);

    public:
        static std::vector<LogEntry> messages;
        static void Log(const std::string& msg);
        static void Warn(const std::string& msg);
        static void Error(const std::string& msg);
};

#endif
