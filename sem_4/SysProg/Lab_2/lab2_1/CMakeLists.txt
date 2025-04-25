cmake_minimum_required(VERSION 3.10)
project(Lab2_Logger)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Включаем предупреждения для компилятора
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# Подключаем заголовочные файлы
include_directories(${PROJECT_SOURCE_DIR}/include)

# Добавляем исходники логгера
add_library(logger
        src/logger.cpp
)

# Основной исполняемый файл
add_executable(main
        src/main.cpp
)

# Линкуем исполняемый файл с библиотекой логгера
target_link_libraries(main PRIVATE logger)
