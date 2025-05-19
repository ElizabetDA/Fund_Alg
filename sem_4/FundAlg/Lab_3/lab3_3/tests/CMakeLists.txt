add_executable(runTests
        test_Splay.cpp
        test_MyMap_Splay.cpp
)
target_include_directories(runTests PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(runTests PRIVATE gtest_main)
target_compile_options(runTests PRIVATE
        -Wno-error=unused-parameter
        -Wno-unused-parameter
)
add_test(NAME runTests COMMAND runTests)
