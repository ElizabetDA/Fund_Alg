cmake_minimum_required(VERSION 3.5)
project(lab2_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
)

# Общий логгер
set(LOGGER_SRCS
        src/logger.cpp
)

add_executable(server
        src/chat_server.cpp
        ${LOGGER_SRCS}
)
target_link_libraries(server
        pthread
)

add_executable(client
        src/chat_client.cpp
        ${LOGGER_SRCS}
)
target_link_libraries(client
        pthread
)
