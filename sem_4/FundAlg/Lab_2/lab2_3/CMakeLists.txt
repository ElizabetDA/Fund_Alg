cmake_minimum_required(VERSION 3.20)

# Устанавливаем политику CMP0135 в новое поведение,
# чтобы FetchContent не ругался на метки времени.
cmake_policy(SET CMP0135 NEW)

project(BigIntProject)

set(CMAKE_CXX_STANDARD 20)
enable_testing()

set(COMMON_FLAGS -Wall -Wextra -Wpedantic -Werror)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COVERAGE_FLAGS --coverage -fprofile-arcs -ftest-coverage)
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(BUILD_GMOCK  OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(my_lib
        src/big_int.cpp
        include/big_int.hpp
)
target_include_directories(my_lib PUBLIC include)

function(apply_flags TGT)
    target_compile_options(${TGT} PRIVATE
            ${COMMON_FLAGS}
            $<$<CONFIG:Debug>:${COVERAGE_FLAGS}>
    )
    target_link_options(${TGT} PRIVATE
            $<$<CONFIG:Debug>:${COVERAGE_FLAGS}>
    )
endfunction()
apply_flags(my_lib)

add_executable(tests
        tests/test_fft.cpp
)
apply_flags(tests)
target_link_libraries(tests PRIVATE my_lib GTest::gtest_main)
add_test(NAME MyTests COMMAND tests)

# Coverage (lcov + genhtml)
find_program(LCOV lcov)
find_program(GENHTML genhtml)
if(LCOV AND GENHTML)
    add_custom_target(coverage
            COMMENT "Generating code coverage report..."
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND ${LCOV} --zerocounters --directory .
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${LCOV} --capture --directory . --output-file coverage.info --ignore-errors mismatch
            COMMAND ${LCOV} --extract coverage.info "${PROJECT_SOURCE_DIR}/include/*" "${PROJECT_SOURCE_DIR}/src/*" --output-file coverage_filtered.info
            COMMAND ${GENHTML} coverage_filtered.info --output-directory coverage_report
    )
    add_dependencies(coverage tests)
endif()

# Static analysis with cppcheck
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    file(GLOB_RECURSE CPPCHECK_SOURCES
            ${CMAKE_SOURCE_DIR}/src/*.cpp
            ${CMAKE_SOURCE_DIR}/include/*.hpp
    )
    add_custom_target(cppcheck
            COMMENT "Running cppcheck static analysis..."
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            VERBATIM
            COMMAND ${CPPCHECK}
            --language=c++
            --enable=all
            --inconclusive
            --inline-suppr
            --suppress=missingIncludeSystem
            --suppress=unusedStructMember
            -I ${CMAKE_SOURCE_DIR}/include
            ${CPPCHECK_SOURCES}
    )
endif()
