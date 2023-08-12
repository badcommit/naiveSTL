//
// Created by Shawn Zhang on 8/8/23.
//


#include "naiveSTL/thread.h"


auto NaiveSTL::Thread::startThreadWrapper(void *obj) -> void * {
    auto *thread = static_cast<NaiveSTL::Thread *>(obj);
    thread->run();
    return nullptr;
}

void NaiveSTL::Thread::run() {
    try {
        func_();
    } catch (const std::exception &e) {
        fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
        fprintf(stderr, "reason: %s\n", e.what());
        throw;
    } catch (...) {
        fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
        throw;
    }
}

void NaiveSTL::Thread::start() {
    started_ = true;

    auto ret = pthread_create(&pthreadId_, nullptr, &Thread::startThreadWrapper, this);
    if(ret != 0){
        fprintf(stderr, "pthread_create error %d\n", ret);
        abort();
    }
}

int NaiveSTL::Thread::join() {
    joined_ = true;
    return pthread_join(pthreadId_, nullptr);
}

NaiveSTL::Thread::~Thread() {
    if (started_ && !joined_) {
        pthread_detach(pthreadId_);
    }
}