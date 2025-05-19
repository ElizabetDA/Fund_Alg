cmake_minimum_required(VERSION 3.10)

project(TCPLogSystem LANGUAGES CXX)

# Require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Collect source files
set(SOURCES
        src/main.cpp
        src/logger.cpp
)

# Include current directory for headers
# Create executable target using project name
add_executable(${PROJECT_NAME} ${SOURCES})

# Include current source directory for headers
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Add compiler warnings
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Link threading library
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
