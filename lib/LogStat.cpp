#include "LogStat.h"

#include <numeric>
#include <mutex>
#include <fstream>
#include <algorithm>
#include <ranges>

namespace ohtuzh {
    // LogStat
    LogStat::LogStat() : thread_pool_(std::thread::hardware_concurrency()) {}

    void LogStat::CollectStat(const std::filesystem::path& directory) {
        if (!is_directory(directory)) {
            throw std::runtime_error("This is not a directory!");
        }
        stats_.clear();
        CollectDirectoryStat(directory);
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);
        while (thread_pool_.GetMaxThreads() != thread_pool_.GetNumberOfFreeThreads()) {}
    }

    void LogStat::CollectDirectoryStat(const std::filesystem::path& directory) {
        for (std::filesystem::directory_iterator d_entity{directory};
             d_entity != std::filesystem::directory_iterator{}; ++d_entity) {
            if (is_directory(*d_entity)) {
                bool result = thread_pool_.TryToRun([this](const std::filesystem::path& dr) {
                    CollectDirectoryStat(dr);
                }, *d_entity);
                if (!result) {
                    CollectDirectoryStat(*d_entity);
                }
            } else if (std::filesystem::is_regular_file(*d_entity) && d_entity->path().extension() == ".log") {
                bool result = thread_pool_.TryToRun([this](const std::filesystem::path& file) {
                    CollectFileStat(file);
                }, *d_entity);
                if (!result) {
                    CollectFileStat(*d_entity);
                }
            }
        }
    }

    void LogStat::CollectFileStat(const std::filesystem::path& file) {
        std::ifstream fin(file);
        std::string line;
        std::lock_guard lg(stat_mutex_);
        while (std::getline(fin, line)) {
            Log log(line);
            if (!stats_.contains(log.GetProcessName())) {
                stats_.emplace(log.GetProcessName(), log.GetProcessName());
            }
            stats_.at(log.GetProcessName()).AddMention(log.GetLogLevel());
        }
    }

    const std::unordered_map<std::string, LogStat::LogStatEntity>& LogStat::GetStats() const noexcept {
        return stats_;
    }

    std::vector<LogStat::LogStatEntity> LogStat::GetSortedStat() const noexcept {
        std::vector<LogStatEntity> sorted_stat;
        sorted_stat.reserve(stats_.size());
        for (const auto& [name, log_stat_entity]: stats_) {
            sorted_stat.push_back(log_stat_entity);
        }
        std::sort(sorted_stat.begin(), sorted_stat.end());
        return sorted_stat;
    }

    std::string LogStat::GetSortedStatTable() const noexcept {
        const auto sorted_stat = GetSortedStat();
        size_t longest_process_name = 0;
        for (const auto& log_stat_entity: sorted_stat) {
            longest_process_name = std::max(longest_process_name, log_stat_entity.GetProcessName().length());
        }
        std::string result = "Process";
        for (int i = 0; i < std::max(0uz, longest_process_name - 7); ++i) {
            result += ' ';
        }
        result += "\tTrace\tDebug\tInfo\tWarn\tError\n";
        for (const auto& log_stat_entity: std::ranges::reverse_view(sorted_stat)) {
            result += log_stat_entity.GetProcessName();
            for (int i = 0; i < longest_process_name - log_stat_entity.GetProcessName().length(); ++i) {
                result += ' ';
            }
            for (int i = 0; i < 5; ++i) {
                result += '\t' + std::to_string(log_stat_entity.GetLogLevelCounter()[i]);
            }
            result += '\n';
        }
        result.pop_back();
        return result;
    }

    // LogStat::LogStatEntity
    LogStat::LogStatEntity::LogStatEntity(std::string process_name)
            : process_name_(std::move(process_name)) {}

    const std::string& LogStat::LogStatEntity::GetProcessName() const noexcept {
        return process_name_;
    }

    const std::array<size_t, 5>& LogStat::LogStatEntity::GetLogLevelCounter() const noexcept {
        return log_level_counter_;
    }

    void LogStat::LogStatEntity::AddMention(Log::LogLevel log_level) noexcept {
        ++log_level_counter_[log_level];
    }

    bool LogStat::LogStatEntity::operator<(const LogStat::LogStatEntity& rhs) const noexcept {
        return std::accumulate(log_level_counter_.begin(), log_level_counter_.end(), 0llu) <
               std::accumulate(rhs.log_level_counter_.begin(), rhs.log_level_counter_.end(), 0llu);
    }
}