cmake_minimum_required(VERSION 3.20)
project(BigIntProject)

set(CMAKE_CXX_STANDARD 20)

# --- Общие флаги ---
set(COMMON_FLAGS -Wall -Wextra -Wpedantic -Werror)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COVERAGE_FLAGS --coverage -fprofile-arcs -ftest-coverage)
endif()

# --- Fetch GoogleTest ---
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- Функция применения флагов ---
function(apply_flags TGT)
    target_compile_options(${TGT} PRIVATE
            ${COMMON_FLAGS}
            $<$<CONFIG:Debug>:${COVERAGE_FLAGS}>
    )
    target_link_options(${TGT} PRIVATE
            $<$<CONFIG:Debug>:${COVERAGE_FLAGS}>
    )
endfunction()

# --- Библиотека big_int ---
add_library(my_lib
        src/big_int.cpp
        include/big_int.hpp
)
target_include_directories(my_lib PUBLIC include)
apply_flags(my_lib)

# --- Тестовый бинарь ---
add_executable(tests
        tests/test_big_int.cpp
)
target_link_libraries(tests PRIVATE my_lib GTest::gtest_main)
apply_flags(tests)

# --- Интеграция GoogleTest ---
include(GoogleTest)
enable_testing()
gtest_discover_tests(tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        PROPERTIES TIMEOUT 120
        TEST_ARGS
        "--gtest_filter=-BigIntDivMod.NewtonGeneral:-BigIntDivMod.NewtonGeneralNegative"
)


# --- Coverage (lcov + genhtml) ---
find_program(LCOV lcov)
find_program(GENHTML genhtml)
if(LCOV AND GENHTML)
    add_custom_target(coverage
            COMMAND ${LCOV} --zerocounters --directory .
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${LCOV} --capture --directory . --output-file coverage.info --ignore-errors mismatch
            COMMAND ${LCOV} --extract coverage.info
            "${PROJECT_SOURCE_DIR}/include/*"
            "${PROJECT_SOURCE_DIR}/src/*"
            --output-file coverage_filtered.info
            COMMAND ${GENHTML} coverage_filtered.info --output-directory coverage_report
            COMMENT "Генерация отчёта покрытия кода..."
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            VERBATIM
    )
    add_dependencies(coverage tests)
endif()

# --- Static analysis (cppcheck) ---
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(cppcheck
            COMMAND ${CPPCHECK}
            --enable=all
            --inconclusive
            --quiet
            --suppress=missingIncludeSystem
            -I include
            src tests
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            VERBATIM
    )
endif()
