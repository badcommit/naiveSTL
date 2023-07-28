cmake_minimum_required(VERSION 3.10)

include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

foreach (test_source ${TEST_SOURCES})
    get_filename_component(filename ${test_source} NAME)
    string(REPLACE ".cpp" "" test_name ${filename})
    add_executable(${test_name} EXCLUDE_FROM_ALL ${test_source})
    target_link_libraries(${test_name} naiveSTL gtest gtest_main)
    gtest_discover_tests(${test_name} PROPERTIES TIMEOUT 10)

endforeach ()