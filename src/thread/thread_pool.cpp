//
// Created by Shawn Zhang on 8/8/23.
//
#include "naiveSTL/memory.h"
#include "naiveSTL/thread_pool.h"
#include "naiveSTL/vector.h"

NaiveSTL::ThreadPool::~ThreadPool() {
    if(running_){
        stop();
    }
}

void NaiveSTL::ThreadPool::start(){
    running_ = true;
    for (int i = 0; i < numThreads_; ++i) {
        std::function<void()> func = [this] { runInThread(); };
        string name = "ThreadPool";
        auto ptr = NaiveSTL::make_unique<Thread>(std::move(func), std::move(name));

        threads_.push_back(std::move(ptr));

    }
    for (int i = 0; i < numThreads_; ++i) {
        threads_[i]->start();
    }
}



void NaiveSTL::ThreadPool::stop() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        running_ = false;
        task_cond_.notify_all();
    }
    for (int i = 0; i < numThreads_; ++i) {
        threads_[i]->join();
    }
}

void NaiveSTL::ThreadPool::waitUntilAllTasksPicked(){
    std::unique_lock<std::mutex> lock(mutex_);
    joined_ = true;
    while(!tasks_.empty()){
        slot_cond_.wait(lock);
    }
}

void NaiveSTL::ThreadPool::submit(Task &&func) {
    std::unique_lock<std::mutex> lock(mutex_);
    if(joined_ || !running_){
        throw std::runtime_error("submit on stopped ThreadPool");
    }
    if(threads_.empty()){
        func();
    }else{

        while(tasks_.size() >= maxQueueSize_){
            slot_cond_.wait(lock);
        }
        tasks_.emplace_back(std::move(func));
        task_cond_.notify_all();
    }
}



void NaiveSTL::ThreadPool::runInThread() {


    while(running_){

        std::unique_lock<std::mutex> lock(mutex_);

        while(tasks_.empty() && running_){
            task_cond_.wait(lock);
        }
        Task task;
        if(!tasks_.empty()){
            task = std::move(tasks_.front());
            tasks_.pop_front();
            slot_cond_.notify_all();
        }
        lock.unlock();
        if(task){
            task();
        }
    }

}

