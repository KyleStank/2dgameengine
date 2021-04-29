#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include "logger.h"

std::vector<engine::logger::log_entry> _log_entries;

// Private function that returns the formatted date/time.
std::string get_formatted_date_time()
{
    std::time_t now;
    std::time(&now);
    std::tm* tm = std::localtime(&now);

    const std::string months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "June", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec" };
    std::ostringstream oss;
    oss << tm->tm_mon + 1 << '/'
        << months[tm->tm_mon] << '/'
        << tm->tm_year + 1900 << ' '
        << tm->tm_hour << ':'
        << tm->tm_min << ':'
        << tm->tm_sec;
    return oss.str();
}

// Private function that formats a custom prefix and message into a log-ready string.
std::string format_log_message(const std::string& prefix, const std::string& msg)
{
    std::ostringstream oss;
    oss << "[" << prefix << "] ["
        << get_formatted_date_time() << "] " << msg;
    return oss.str();
}

// Logs a message to the console.
void engine::logger::log(const std::string& msg)
{
    log_entry le;
    le.type = log_type::INFO;
    le.message = format_log_message("INFO", msg);

    std::cout << "\033[1;37m" << le.message << "\033[0m" << std::endl;

    _log_entries.push_back(le);
}

// Logs a character to the console.
void engine::logger::log(const char& msg)
{
    log(std::string(1, msg));
}

// Logs a warning message to the console.
void engine::logger::warn(const std::string& msg)
{
    log_entry le;
    le.type = log_type::WARNING;
    le.message = format_log_message("WARN", msg);

    std::cout << "\033[1;33m" << le.message << "\033[0m" << std::endl;

    _log_entries.push_back(le);
}

// Logs a warning character to the console.
void engine::logger::warn(const char& msg)
{
    warn(std::string(1, msg));
}

// Logs an error message to the console.
void engine::logger::error(const std::string& msg)
{
    log_entry le;
    le.type = log_type::ERROR;
    le.message = format_log_message("ERROR", msg);

    std::cerr << "\033[1;31m" << le.message << "\033[0m" << std::endl;

    _log_entries.push_back(le);
}

// Logs an error character to the console.
void engine::logger::error(const char& msg)
{
    error(std::string(1, msg));
}
