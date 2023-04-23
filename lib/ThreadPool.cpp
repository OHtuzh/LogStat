#include "ThreadPool.h"

namespace ohtuzh {
    ThreadPool::ThreadPool(unsigned int amount_of_threads) : kMaxAmountThreads(amount_of_threads) {
        for (auto i = 0; i < amount_of_threads; ++i) {
            free_threads_.push_back(i);
        }
    }

    unsigned int ThreadPool::GetMaxThreads() const noexcept {
        return kMaxAmountThreads;
    }

    unsigned int ThreadPool::GetNumberOfFreeThreads() noexcept {
        std::lock_guard lg(thread_pool_mutex_);
        return free_threads_.size();
    }

    ThreadPool::~ThreadPool() {
        for (auto& [id, th]: threads_) {
            th.join();
        }
    }
}