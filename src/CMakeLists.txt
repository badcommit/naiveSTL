add_subdirectory(allocator)

add_library(naiveSTL STATIC ${ALL_OBJECT_FILES})

set(NAIVE_STL_LIBS
        naiveSTL_allocator)

target_link_libraries(naiveSTL ${NAIVE_STL_LIBS})

target_include_directories(
        naiveSTL PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)