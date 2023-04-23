#pragma once

#include <thread>
#include <vector>
#include <optional>
#include <unordered_map>

namespace ohtuzh {

    class ThreadPool {
    public:
        explicit ThreadPool(unsigned int amount_of_threads);


        template<typename Function, typename... Args>
        bool TryToRun(Function function, Args&&... arguments);

        unsigned int GetMaxThreads() const noexcept;

        unsigned int GetNumberOfFreeThreads() noexcept;

        ~ThreadPool();

    private:
        const unsigned int kMaxAmountThreads;
        std::unordered_map<int, std::thread> threads_;
        std::vector<int> free_threads_;
        std::mutex thread_pool_mutex_;
    };

    template<typename Function, typename... Args>
    bool ThreadPool::TryToRun(Function function, Args&& ... arguments) {
        std::lock_guard lg(thread_pool_mutex_);
        if (free_threads_.empty()) {
            return false;
        }
        auto id = free_threads_.back();
        free_threads_.pop_back();
        if (threads_.contains(id)) {
            threads_.at(id).join();
            threads_.erase(id);
        }
        threads_[id] = std::thread([this](int thread_id, Function func, Args&& ... args) {
            func(args...);
            std::lock_guard lock_g(thread_pool_mutex_);
            free_threads_.push_back(thread_id);
        }, id, function, std::forward<Args>(arguments)...);
        return true;
    }
}
