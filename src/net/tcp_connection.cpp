//
// Created by Zhang Shawn on 8/17/23.
//

#include <utility>

#include "naiveSTL/algorithm.h"
#include "naiveSTL/net/tcp_connection.h"

namespace NaiveSTL::Net {
    void TCPConnection::send(const NaiveSTL::string &message) {
        //since event_loop is single threaded, we just copy it
        event_loop_->queueCallback([this, message](){
            copy(message.begin(), message.end(), output_buffer_.end());
            this->handleChannelWrite();
        });
    }

    void TCPConnection::close() {
        event_loop_->queueCallback([this](){
           channel_->socket().close();
        });
    }

    TCPConnection::TCPConnection(shared_ptr<NaiveSTL::Net::EventLoop> loop,
                                 int socketfd) : event_loop_(std::move(loop)), channel_(make_shared<Channel>(Socket{socketfd}))
                                                                               {
            //since we get the unique pointer to channel this is valid util we destructor
            channel_->setReadCallback([this](timestamp ts){
                this->handleChannelRead(ts);
            });

            channel_->setCloseCallback([this](){
                this->handleChannelClose();
            });

            channel_->setWriteCallback([this](){
                this->handleChannelWrite();
            });

            channel_->setErrorCallback([this](){
                this->handleChannelError();
            });



    }


    void TCPConnection::handleChannelWrite() {
        if(channel_->isWriting()){
            size_t size = SocketOps::write(this->channel_->fd(), output_buffer_.begin(), output_buffer_.size());
            if(size != static_cast<size_t>(output_buffer_.size())){
                throw;
            }
            output_buffer_.clear();
            channel_->disableWriting();
        }
    }

    void TCPConnection::handleChannelRead(timestamp t) {
        char buf[1024];
        size_t read = 0;
        while((read = SocketOps::read(channel_->fd(), buf, sizeof buf)) > 0){
            //ugly... but since this is a toy project...
            size_t r = 0;
            while(read > 0){
                read -- ;
                input_buffer_.push_back(buf[r]);
                r++;
            }
        }
        //event loop is created by tcp server or client, it holds tcp connection until it desctructs
        //so this is valid until tcp server is desctruct
        event_loop_->queueCallback([this, t](){
            this->messageCallback_(*this, t);
        });
    }

    void TCPConnection::handleChannelError() {

    }

    void TCPConnection::handleChannelClose() {

    }
}
