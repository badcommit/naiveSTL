//
// Created by Shawn Zhang on 8/12/23.
//


#include <gtest/gtest.h>
#include <thread>

#include "naiveSTL/net/event_loop.h"
#include "naiveSTL/memory.h"
#include "naiveSTL/string.h"
#include "naiveSTL/net/poll_poller.h"
#include "naiveSTL/net/event_loop_thread.h"
#include "naiveSTL/net/event_loop_thread_pool.h"

namespace NaiveSTL::event_loop_test {


    using namespace NaiveSTL::Net;

    class MockPoller : public Poller {
    public:
        MockPoller() = default;

        MockPoller(std::function<void()> &&cb): cb_(std::move(cb)){};

        ~MockPoller() = default;

        auto poll(vector<shared_ptr<Channel>> &a __attribute__((unused)),
                  vector<shared_ptr<Channel>> &active __attribute__((unused))) -> void override {
            if(cb_) cb_();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        };

        virtual void notifyUpdateChannel(const shared_ptr<Channel> &a __attribute__((unused))) override {};

        virtual void notifyNewChannel(const shared_ptr<Channel> &__attribute__((unused))) override {};

        virtual void notifyRemoveChannel(const shared_ptr<Channel> &__attribute__((unused))) override {};

        virtual shared_ptr<Channel> getChannelByFd(const vector<shared_ptr<Channel>> &chans __attribute__((unused)),
                                                   int fd __attribute__((unused))) override { throw; };


        [[nodiscard]] virtual auto hasChannel(const vector<shared_ptr<Channel>> &a __attribute__((unused)),
                                              int fd __attribute__((unused))) const -> bool override { return true; };
    private:
        std::function<void()> cb_;
    };

    TEST(EVENT_LOOP, CHANNEL_TIRGGER_UPDATE_CALLBACK) {
        auto poller = make_unique<MockPoller>();
        std::atomic<int> counter = 0;
        Channel channel(1, [&](auto &&) {
            counter++;
        });

        Thread thread{[&]() {
            channel.enableReading();
        }, "loop!"};
        thread.start();
        thread.join();

        EXPECT_EQ(counter, 1);


    }

    TEST(EVENT_LOOP, LOOP_TRIGGER_POLLER) {
        std::atomic<int> counter = 0;
        auto poller = make_unique<MockPoller>([&](){
            counter++;
        });


        EventLoop loop(std::move(poller));

        Thread thread{[&]() {
            loop.loop();
        }, "loop!"};
        thread.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        loop.quit();
        thread.join();
        EXPECT_TRUE(counter>0);

    }

    TEST(EVENT_LOOP, EVENT_LOOP_THREAD) {
        std::atomic<int> counter = 0;
        auto poller = make_unique<MockPoller>([&](){
            counter++;
        });

        EventLoopThread thread(std::move(poller));

        thread.start();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        thread.stop();
        EXPECT_TRUE(counter>0);
    }

    TEST(EVENT_LOOP, EVENT_LOOP_POOL){
        std::atomic<int> counter = 0;
        auto poller = make_unique<MockPoller>([&](){
            counter++;
        });

        EventLoopThreadPool thread_pool(std::move(poller), 3);
    }

}