cmake_minimum_required(VERSION 3.10)
project(FileProcessor C)

# Установка стандарта C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Включение директорий с исходными файлами
include_directories(${CMAKE_SOURCE_DIR})

# Добавление основного исполняемого файла
add_executable(file_processor main.c file_operations.c)

# Включение директории с тестами (если она существует)
if (EXISTS ${CMAKE_SOURCE_DIR}/tests)
    enable_testing()
    add_subdirectory(tests)
endif()