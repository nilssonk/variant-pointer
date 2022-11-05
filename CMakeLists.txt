cmake_minimum_required(VERSION 3.14)

project(variant_pointer VERSION 0.1 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(lto.helper)
include(with.helper)
include(tidy.helper)

option(VARIANT_POINTER_CLANG_TIDY "Build with clang-tidy" ON)
option(VARIANT_POINTER_TESTS "Build and add tests" ON)
option(VARIANT_POINTER_COVERAGE "Build with coverage support" OFF)
option(VARIANT_POINTER_LTO "Build with Link-Time Optimization" OFF)
option(VARIANT_POINTER_AS_SYSTEM_INCLUDE "Treat variant-pointer as a header only library" OFF)

WithMsg(tests ${VARIANT_POINTER_TESTS})
WithMsg(clang-tidy ${VARIANT_POINTER_CLANG_TIDY})
WithMsg(coverage ${VARIANT_POINTER_COVERAGE})
WithMsg(LTO ${VARIANT_POINTER_LTO})
WithMsg(as-system-include ${VARIANT_POINTER_AS_SYSTEM_INCLUDE})

if (VARIANT_POINTER_CLANG_TIDY)
    include(clang-tidy)
endif()

if (VARIANT_POINTER_TESTS)
    include(CTest)
    enable_testing()
endif()

set(VARIANT_POINTER_EXTRA_CFLAGS -O3 -ffast-math -fvisibility=hidden)

set_property(GLOBAL PROPERTY VARIANT_POINTER_RELEASE_EXTRA_CFLAGS ${VARIANT_POINTER_EXTRA_CFLAGS})

add_subdirectory(3rdparty)

add_library(variant_pointer_copts_common INTERFACE)
target_compile_options(variant_pointer_copts_common INTERFACE
    -pedantic
    -W
    -Wall
    -Wextra
    -Wcast-align
    -Wcast-qual
    -Wmissing-declarations
    -Wwrite-strings
    -Wundef
    -Wswitch-enum
    -Wshadow
    -Werror
    -Wstrict-aliasing=2
    $<$<C_COMPILER_ID:Clang>:-Wshadow-all>
    $<$<CXX_COMPILER_ID:Clang>:-Wshadow-all>
    $<$<STREQUAL:$<CONFIG>,Release>:${VARIANT_POINTER_EXTRA_CFLAGS}>)

# Main project
file(GLOB_RECURSE INC "include/*.hh")
file(GLOB_RECURSE SRC "src/*.cc")

add_library(variant_pointer)
target_sources(variant_pointer PRIVATE ${SRC} ${INC})
target_link_libraries(variant_pointer PRIVATE variant_pointer_copts_common)
if(VARIANT_POINTER_AS_SYSTEM_INCLUDE)
    target_include_directories(variant_pointer
        SYSTEM PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
else()
    target_include_directories(variant_pointer
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
endif()

file(GLOB_RECURSE TEST_INC "test/*.hh")
file(GLOB_RECURSE TEST_SRC "test/*.cc")

add_executable(variant_pointer_test)
target_sources(variant_pointer_test PRIVATE ${TEST_SRC} ${TEST_INC})
target_link_libraries(variant_pointer_test
    PRIVATE
    variant_pointer_copts_common
    variant_pointer
    Catch2::Catch2WithMain)
add_test(
    NAME variant_pointer_test
    COMMAND variant_pointer_test)