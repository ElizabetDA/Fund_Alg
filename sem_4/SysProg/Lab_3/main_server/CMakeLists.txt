add_executable(main_server
        src/main.cpp
        src/Server.cpp
)
target_include_directories(main_server PRIVATE include)
target_link_libraries(main_server pthread)
