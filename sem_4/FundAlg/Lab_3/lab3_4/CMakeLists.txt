cmake_minimum_required(VERSION 3.10)
project(FundAlg_Lab3_4 LANGUAGES CXX)

# Опция покрытия
option(ENABLE_COVERAGE "Enable coverage reporting" ON)
if(ENABLE_COVERAGE)
    message(STATUS "Building with coverage enabled")
    set(COVERAGE_FLAGS "-O0 -g --coverage")
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS}   ${COVERAGE_FLAGS}")
    set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS}     ${COVERAGE_FLAGS}")
endif()

# C++17 + строгие предупреждения / санитайзеры
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -fsanitize=address -fsanitize=leak")

# Заголовки
include_directories(${CMAKE_SOURCE_DIR}/include)

# Демка
add_executable(lab3_4_demo src/main.cpp)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)

# Coverage (только для RedBlack.hpp)
find_program(LCOV lcov)
find_program(GENHTML genhtml)
if(LCOV AND GENHTML)
    add_custom_target(coverage
            COMMENT "Генерация отчёта покрытия кода для RedBlack.hpp"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            VERBATIM
            COMMAND ${LCOV} --zerocounters --directory .
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${LCOV} --capture --directory . --output-file coverage.info --ignore-errors mismatch
            COMMAND ${LCOV} --remove coverage.info '*/_deps/*' '*/tests/*' --ignore-errors=unused --output-file coverage.cleaned.info
            COMMAND ${LCOV} --extract coverage.cleaned.info
            "${PROJECT_SOURCE_DIR}/include/RedBlack.hpp"
            --ignore-errors=unused
            --output-file coverage.rb.info
            COMMAND ${GENHTML} coverage.rb.info --output-directory coverage_rb
    )
    add_dependencies(coverage runTests)
endif()

# cppcheck
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(cppcheck
            COMMAND ${CPPCHECK}
            --enable=all --inconclusive --quiet --suppress=missingIncludeSystem
            -I ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_SOURCE_DIR}/tests
            VERBATIM
    )
endif()
