//
// Created by Shawn Zhang on 8/8/23.
//

#ifndef NAIVESTL_THREAD_H
#define NAIVESTL_THREAD_H

#include <pthread.h>
#include <functional>
#include <latch>
#include <mutex>
#include "naiveSTL/string.h"

#include "naiveSTL/utility.h"

namespace NaiveSTL{
    class Thread final : public noncopyable  {
        using thread_func = std::function<void()>;

    public:
        explicit Thread(thread_func && func, string &&name):func_(std::move(func)), name_(std::move(name)), started_(false), joined_(false) {};

        Thread(Thread &&rhs) noexcept : pthreadId_(rhs.pthreadId_), func_(std::move(rhs.func_)), name_(std::move(rhs.name_)), started_(bool(rhs.started_)), joined_(bool(rhs.joined_)){
            rhs.~Thread();
        }

        auto operator=(Thread &&rhs) noexcept -> Thread & {
            if (this != &rhs) {
                func_ = std::move(rhs.func_);
                pthreadId_ = rhs.pthreadId_;
                name_ = std::move(rhs.name_);
                started_ = bool(rhs.started_);
                joined_ = bool(rhs.joined_);
                rhs.~Thread();
            }
            return *this;
        }

        Thread (const Thread &) = delete;
        Thread &operator=(const Thread &) = delete;

        void start();
        auto join() -> int;

        [[nodiscard]] auto name() const -> const string & { return name_; }

        [[nodiscard]] pthread_t pthreadId() const { return pthreadId_; }

        [[nodiscard]] bool started() const { return started_; }

        [[nodiscard]] bool joined() const { return joined_; }



        ~Thread();
    private:
        void run();
        static auto startThreadWrapper(void *obj) -> void*;

        std::mutex mutex_;
        pthread_t pthreadId_{};
        thread_func func_;
        string name_;
        std::atomic<bool> started_;
        std::atomic<bool> joined_;
    };
}

#endif //NAIVESTL_THREAD_H
