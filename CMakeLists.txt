cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

project(jsonast-cpp VERSION 0.1 LANGUAGES CXX)

add_compile_options(-Wall -Wextra -Wswitch -Wimplicit-fallthrough)
add_compile_definitions("-DLOGS") # used for LOG_* and COUT/CERR macros
add_compile_definitions("-D_DEFAULT_SOURCE") # needed for reallocarray
add_compile_definitions("-DJSONASTCPP_VERSION=${CMAKE_PROJECT_VERSION}")
message(STATUS "CMAKE_PROJECT_VERSION: '${CMAKE_PROJECT_VERSION}'")
include_directories(BEFORE "src/adt")

message (STATUS "CMAKE_CXX_COMPILER_ID: '${CMAKE_CXX_COMPILER_ID}'")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-class-memaccess)
endif()

add_executable(
    ${CMAKE_PROJECT_NAME}
    "src/main.cc"
    "src/json/lex.cc"
    "src/json/parser.cc"
)

if (CMAKE_BUILD_TYPE MATCHES "Asan")
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fsanitize=address")
    add_link_options(-fsanitize=undefined -fsanitize=address)
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_compile_definitions("-DDEBUG")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function)
endif()

cmake_host_system_information(RESULT OS_NAME QUERY OS_NAME)
message(STATUS "OS_NAME: '${OS_NAME}'")

message(STATUS "CMAKE_BUILD_TYPE: '${CMAKE_BUILD_TYPE}'")
