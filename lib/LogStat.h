#pragma once

#include <filesystem>
#include <unordered_map>
#include <array>
#include <vector>
#include <thread>

#include "Log.h"
#include "ThreadPool.h"

namespace ohtuzh {

    class LogStat {
    public:
        class LogStatEntity;

        LogStat();

        void CollectStat(const std::filesystem::path& directory);

        const std::unordered_map<std::string, LogStatEntity>& GetStats() const noexcept;

        std::vector<LogStatEntity> GetSortedStat() const noexcept;

        std::string GetSortedStatTable() const noexcept;
    private:
        void CollectDirectoryStat(const std::filesystem::path& directory);

        void CollectFileStat(const std::filesystem::path& file);

        ThreadPool thread_pool_;

        std::mutex stat_mutex_;
        std::unordered_map<std::string, LogStatEntity> stats_;
    };


    class LogStat::LogStatEntity {
    public:
        explicit LogStatEntity(std::string process_name);

        [[nodiscard]] const std::string& GetProcessName() const noexcept;

        [[nodiscard]] const std::array<size_t, 5>& GetLogLevelCounter() const noexcept;

        void AddMention(Log::LogLevel log_level) noexcept;

        bool operator<(const LogStat::LogStatEntity& rhs) const noexcept;

    private:

        std::string process_name_;
        std::array<size_t, 5> log_level_counter_{0, 0, 0, 0, 0};
    };
}

