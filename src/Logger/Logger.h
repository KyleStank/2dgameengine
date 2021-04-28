#ifndef ENGINE_LOGGER_H
#define ENGINE_LOGGER_H

#include <string>
#include <vector>

namespace engine
{
    static const std::string MONTH_STRING[12] = { "Jan", "Feb", "Mar", "Apr", "May", "June", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec" };

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
            static std::string GetFormattedDateTime();
            static std::string GetBaseLogMessage(const std::string& prefix, const std::string& msg);

        public:
            static std::vector<LogEntry> messages;

            static void Log(const std::string& msg);
            static void Log(const char& msg);

            static void Warn(const std::string& msg);
            static void Warn(const char& msg);

            static void Error(const std::string& msg);
            static void Error(const char& msg);
    };
}

#endif
