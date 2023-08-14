//
// Created by Shawn Zhang on 8/8/23.
//

#ifndef NAIVESTL_THREAD_POOL_H
#define NAIVESTL_THREAD_POOL_H


#include <functional>
#include <deque>
#include <mutex>

#include "naiveSTL/vector.h"
#include "naiveSTL/memory.h"
#include "naiveSTL/thread.h"
#include "naiveSTL/utility.h"

namespace NaiveSTL {
    class ThreadPool final : public noncopyable{
        using Task = std::function<void()>;
    public:
        explicit ThreadPool(int numThreads, size_t maxQueueSize = 1): maxQueueSize_(maxQueueSize), numThreads_(numThreads), running_(false), joined_(false) {

        }
        ~ThreadPool();

        void start();
        void stop();


        void submit(Task &&func);

        void waitUntilAllTasksPicked();


    private:
        void runInThread();

        std::mutex mutex_;
        size_t maxQueueSize_;
        std::condition_variable slot_cond_;
        std::condition_variable task_cond_;
        int numThreads_;
        std::atomic<bool> running_;
        std::atomic<bool> joined_;
        std::deque<Task> tasks_;
        vector<unique_ptr<Thread>> threads_;
    };
}


#endif //NAIVESTL_THREAD_POOL_H
