#include "Log.h"

#include <algorithm>

namespace ohtuzh {
    Log::Log(const std::string& line) {
        auto first_closed_bracket = std::find(line.begin(), line.end(), ']');
        timestamp_ = std::string(line.begin() + 1, first_closed_bracket);
        auto second_closed_bracket = std::find(first_closed_bracket + 1, line.end(), ']');

        log_level_ = LogLevelConverter.at(std::string(first_closed_bracket + 2, second_closed_bracket));

        first_closed_bracket = second_closed_bracket;
        second_closed_bracket = std::find(first_closed_bracket + 2, line.end(), ']');

        process_name_ = std::string(first_closed_bracket + 2, second_closed_bracket);

        log_message_ = std::string(second_closed_bracket + 2, line.end());
    }

    const std::string& Log::GetTimestamp() const noexcept {
        return timestamp_;
    }

    Log::LogLevel Log::GetLogLevel() const noexcept {
        return log_level_;
    }

    const std::string& Log::GetProcessName() const noexcept {
        return process_name_;
    }

    const std::string& Log::GetLogMessage() const noexcept {
        return log_message_;
    }


}