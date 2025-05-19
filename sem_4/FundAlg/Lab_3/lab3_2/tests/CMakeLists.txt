# tests/CMakeLists.txt
add_executable(runTests
        test_BST.cpp
        test_MyMap.cpp
        test_AVL.cpp
        test_MyMap_AVL.cpp
)
target_include_directories(runTests PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(runTests PRIVATE gtest_main)
target_compile_options(runTests PRIVATE
        -Wno-error=unused-parameter
        -Wno-unused-parameter
)
add_test(NAME runTests COMMAND runTests)
