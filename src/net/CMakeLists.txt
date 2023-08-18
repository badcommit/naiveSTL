add_library(naiveSTL_net OBJECT channel.cpp common.cpp event_loop.cpp poll_poller.cpp event_loop_thread.cpp event_loop_thread_pool.cpp
        tcp_connection.cpp
        inet_address.cpp
        socket.cpp)

set(
        ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES}
        $<TARGET_OBJECTS:naiveSTL_net>
        PARENT_SCOPE
)