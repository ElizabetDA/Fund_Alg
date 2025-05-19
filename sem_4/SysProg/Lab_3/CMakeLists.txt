cmake_minimum_required(VERSION 3.15)
project(SystemProgProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -fsanitize=address -fsanitize=leak")

add_subdirectory(client)
add_subdirectory(main_server)
add_subdirectory(compile_subserver)
add_subdirectory(game_subserver)