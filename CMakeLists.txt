cmake_minimum_required(VERSION 3.25)
project(NaiveSTL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


message("Build mode: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

set(NAIVE_STL_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/includes/)
set(NAIVE_STL_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${NAIVE_STL_SRC_INCLUDE_DIR} ${NAIVE_STL_TEST_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(test)



add_executable(NaiveSTLMain main.cpp)

message("NaiveSTL_LIBS: ${NAIVE_STL_LIBS}")

target_link_libraries(NaiveSTLMain ${NAIVE_STL_LIBS})






