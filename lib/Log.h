#pragma once

#include <string>
#include <unordered_map>

namespace ohtuzh {


    class Log {
    public:
        enum LogLevel : int;

        explicit Log(const std::string& line);


        [[nodiscard]] const std::string& GetTimestamp() const noexcept;

        [[nodiscard]] LogLevel GetLogLevel() const noexcept;

        [[nodiscard]] const std::string& GetProcessName() const noexcept;

        [[nodiscard]] const std::string& GetLogMessage() const noexcept;

        enum LogLevel : int {
            kTrace = 0,
            kDebug = 1,
            kInfo = 2,
            kWarn = 3,
            kError = 4
        };

    private:
        std::string timestamp_;
        LogLevel log_level_;
        std::string process_name_;
        std::string log_message_;
    };

    static const std::unordered_map<std::string, Log::LogLevel> LogLevelConverter = {
            {"Trace", Log::LogLevel::kTrace},
            {"Debug", Log::LogLevel::kDebug},
            {"Info",  Log::LogLevel::kInfo},
            {"Warn",  Log::LogLevel::kWarn},
            {"Error", Log::LogLevel::kError}
    };

}
