add_library(
    naiveSTL_allocator
        OBJECT
        alloc.cpp
        allocator.cpp
)

set(
        ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES}
        $<TARGET_OBJECTS:naiveSTL_allocator>
        PARENT_SCOPE
)