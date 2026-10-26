cmake_minimum_required(VERSION 3.10)

project(TIVIDY LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -pedantic)
endif()

file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

list(FILTER ALL_SOURCES EXCLUDE REGEX "/build/")
list(FILTER ALL_SOURCES EXCLUDE REGEX "/CMakeFiles/")
list(FILTER ALL_SOURCES EXCLUDE REGEX "/tests/")

set(MAIN_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

list(REMOVE_ITEM ALL_SOURCES "${MAIN_SOURCE}")

if(ALL_SOURCES)
    add_library(tividy_core STATIC ${ALL_SOURCES})

    target_include_directories(tividy_core PUBLic
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        )
    endif()

    if(EXISTS "${MAIN_SOURCE}")
        add_executable(tividy "${MAIN_SOURCE}")

        if(TARGET tividy_core)
            target_link_libraries(tividy PRIVATE tividy_core)
        endif()
    endif()

    enable_testing()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
        add_subdirectory(tests)
    endif()