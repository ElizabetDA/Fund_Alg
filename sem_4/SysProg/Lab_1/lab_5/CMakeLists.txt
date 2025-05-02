cmake_minimum_required(VERSION 3.10)
project(lab_5 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

# Укажите явный путь к исходному файлу, если нужно
add_executable(lab_5
        5.c
)

target_link_libraries(lab_5 PRIVATE Threads::Threads)