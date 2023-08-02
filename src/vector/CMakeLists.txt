add_library(naiveSTL_vector
        OBJECT
        vector.impl.cpp)

set(
        ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES}
        $<TARGET_OBJECTS:naiveSTL_vector>
        PARENT_SCOPE
)