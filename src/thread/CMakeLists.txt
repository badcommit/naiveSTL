add_library(naiveSTL_thread OBJECT thread.cpp thread_pool.cpp)

set(
        ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES}
        $<TARGET_OBJECTS:naiveSTL_thread>
        PARENT_SCOPE
)