
set(TEST_SOURCES test_main.cpp)

foreach(name test_composite_structure test_composite_behaviour test_builder)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp")
        list(APPEND TEST_SOURCES ${name}.cpp)
    endif()
endforeach()

add_executable(tividy_tests ${TEST_SOURCES})
target_link_libraries(tividy_tests PRIVATE tividy_core)
target_include_directories(tividy_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

add_test(NAME tividy_tests COMMAND tividy_tests)
