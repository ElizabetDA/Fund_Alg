# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(ElevatorSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# заголовки
include_directories(${PROJECT_SOURCE_DIR}/include)

# все .cpp
file(GLOB SRC
        ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(elevator_sim ${SRC})

# для std::thread
find_package(Threads REQUIRED)
target_link_libraries(elevator_sim PRIVATE Threads::Threads)
