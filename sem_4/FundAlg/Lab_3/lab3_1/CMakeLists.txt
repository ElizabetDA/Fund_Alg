cmake_minimum_required(VERSION 3.10)
project(FundamentalAlgorithms)

# Опционально: включаем покрытие в конфигурации Debug
option(ENABLE_COVERAGE "Enable coverage reporting" ON)
if(ENABLE_COVERAGE)
    message(STATUS "Building with coverage enabled")
    set(COVERAGE_FLAGS "-O0 -g --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_FLAGS}")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${COVERAGE_FLAGS}")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -fsanitize=address -fsanitize=leak")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(lab1 src/main.cpp)

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

# --- Coverage target ---
find_program(LCOV lcov)
find_program(GENHTML genhtml)
if(LCOV AND GENHTML)
    add_custom_target(coverage
            # обнуляем счётчики
            COMMAND ${LCOV} --zerocounters --directory ${CMAKE_BINARY_DIR}
            # запускаем тесты
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            # собираем сырые данные покрытия
            COMMAND ${LCOV} --capture --directory ${CMAKE_BINARY_DIR} --output-file coverage.info --ignore-errors mismatch
            # удаляем лишние (gtest, тесты и пр.), игнорируя неиспользованные паттерны
            COMMAND ${LCOV} --remove coverage.info
            "*/_deps/*"
            "*/tests/*"
            --ignore-errors=unused
            --output-file coverage.cleaned.info
            # фильтруем только наши исходники и заголовки
            COMMAND ${LCOV} --extract coverage.cleaned.info
            "${PROJECT_SOURCE_DIR}/src/*"
            "${PROJECT_SOURCE_DIR}/include/*"
            --output-file coverage_filtered.info
            --ignore-errors=unused
            # генерируем HTML-отчёт
            COMMAND ${GENHTML} coverage_filtered.info --output-directory coverage_report
            COMMENT "Генерация отчёта покрытия кода..."
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            VERBATIM
    )
    add_dependencies(coverage runTests)
endif()

# --- Cpplint / cppcheck target ---
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(cppcheck
            COMMAND ${CPPCHECK}
            --enable=all
            --inconclusive
            --quiet
            --suppress=missingIncludeSystem
            -I ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_SOURCE_DIR}/tests
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            VERBATIM
    )
endif()
