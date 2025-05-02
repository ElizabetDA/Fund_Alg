cmake_minimum_required(VERSION 3.10)
project(Fund_Alg)

set(CMAKE_C_STANDARD 99)

# Добавление исходных файлов
set(SOURCES
        3.c
)

# Добавление исполняемого файла
add_executable(Fund_Alg ${SOURCES})

# Указание директории для поиска заголовочных файлов
target_include_directories(Fund_Alg PRIVATE .)