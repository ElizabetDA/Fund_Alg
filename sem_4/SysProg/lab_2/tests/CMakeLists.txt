cmake_minimum_required(VERSION 3.10)
project(FileProcessorTests C)

# Добавление тестового исполняемого файла
add_executable(test_file_processor test_main.c ../file_operations.c)

# Подключение исходного кода из основной директории
target_include_directories(test_file_processor PRIVATE ${CMAKE_SOURCE_DIR})

# Добавление тестов
add_test(NAME test_file_processor COMMAND test_file_processor)