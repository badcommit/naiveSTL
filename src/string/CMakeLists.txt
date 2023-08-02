add_library(
        naiveSTL_string
        OBJECT
        string.impl.cpp
)

set(
        ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES}
        $<TARGET_OBJECTS:naiveSTL_string>
        PARENT_SCOPE
)