cmake_minimum_required(VERSION 3.10)

project(FundAlg_Lab3_2 LANGUAGES CXX)

# --- Опция покрытия ---
option(ENABLE_COVERAGE "Enable coverage reporting" ON)
if(ENABLE_COVERAGE)
    message(STATUS "Building with coverage enabled")
    set(COVERAGE_FLAGS "-O0 -g --coverage")
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS}   ${COVERAGE_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COVERAGE_FLAGS}")
    set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS}     ${COVERAGE_FLAGS}")
endif()

# Стандарт и предупреждения
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -fsanitize=address -fsanitize=leak")

# Пути
include_directories(${CMAKE_SOURCE_DIR}/include)

# Собираем демонстрационный исполняемый
add_executable(lab3_2_demo src/main.cpp)

# GoogleTest через FetchContent
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# Включаем тестирование
enable_testing()

# Собираем юнит-тесты
add_subdirectory(tests)

# --- Coverage target (только AVL) ---
find_program(LCOV lcov)
find_program(GENHTML genhtml)
if(LCOV AND GENHTML)
    add_custom_target(coverage
            COMMENT "Генерация отчёта покрытия кода (только AVL)..."
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            VERBATIM

            # 1) сбрасываем старые счётчики
            COMMAND ${LCOV} --zerocounters --directory .

            # 2) прогоняем тесты
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure

            # 3) собираем полные данные
            COMMAND ${LCOV} --capture --directory . --output-file coverage.info --ignore-errors mismatch

            # 4) удаляем всю постороннюю логику (gtest, тесты и т.п.)
            COMMAND ${LCOV}
            --remove coverage.info
            '*/_deps/*'
            '*/tests/*'
            --ignore-errors=unused
            --output-file coverage.cleaned.info

            # 5) извлекаем ТОЛЬКО include/AVL.hpp
            COMMAND ${LCOV}
            --extract coverage.cleaned.info
            "${PROJECT_SOURCE_DIR}/include/AVL.hpp"
            --ignore-errors=unused
            --output-file coverage.avl.info

            # 6) генерируем HTML-отчёт в каталог coverage_avl
            COMMAND ${GENHTML} coverage.avl.info --output-directory coverage_avl
    )
    add_dependencies(coverage runTests)
endif()

# --- cppcheck target ---
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
