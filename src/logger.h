#ifndef ENGINE_LOGGER_H
#define ENGINE_LOGGER_H

#include <string>
#include <vector>

namespace engine::logger
{
    enum log_type
    {
        INFO = 0,
        WARNING = 1,
        ERROR = 2
    };

    struct log_entry
    {
        log_type type;
        std::string message;
    };

    void log(const std::string& msg);
    void log(const char& msg);
    void warn(const std::string& msg);
    void warn(const char& msg);
    void error(const std::string& msg);
    void error(const char& msg);
}

#endif
