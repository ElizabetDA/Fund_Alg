cmake_minimum_required(VERSION 3.20)
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
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(my_lib
        src/big_int.cpp
        include/big_int.hpp
)
target_include_directories(my_lib PUBLIC include)

function(apply_flags TGT)
    target_compile_options(${TGT} PRIVATE ${COMMON_FLAGS}
            $<$<CONFIG:Debug>:${COVERAGE_FLAGS}>)
    target_link_options(${TGT} PRIVATE
            $<$<CONFIG:Debug>:${COVERAGE_FLAGS}>)
endfunction()
apply_flags(my_lib)

add_executable(tests tests/test_big_int.cpp)
apply_flags(tests)
target_link_libraries(tests PRIVATE my_lib GTest::gtest_main)

add_test(NAME MyTests COMMAND tests)

find_program(LCOV lcov)
find_program(GENHTML genhtml)
if(LCOV AND GENHTML)
    add_custom_target(coverage
            # обнуляем счётчики
            COMMAND ${LCOV} --zerocounters --directory .
            # запускаем тесты
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            # снова собираем данные
            COMMAND ${LCOV} --capture --directory . --output-file coverage.info --ignore-errors mismatch
            # фильтруем только наши заголовки/исходники
            COMMAND ${LCOV} --extract coverage.info "${PROJECT_SOURCE_DIR}/include/*" "${PROJECT_SOURCE_DIR}/src/*" --output-file coverage_filtered.info
            # HTML‑отчёт
            COMMAND ${GENHTML} coverage_filtered.info --output-directory coverage_report
            COMMENT "Генерация отчёта покрытия кода..."
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            VERBATIM
    )
    add_dependencies(coverage tests)   # библиотека + exe должны быть собраны
endif()


find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(cppcheck
            COMMAND ${CPPCHECK} --enable=all --inconclusive --quiet --suppress=missingIncludeSystem
            -I include src tests
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            VERBATIM
    )
endif()
