add_executable(runTests
        test_BST.cpp
        test_MyMap.cpp
)
target_include_directories(runTests PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(runTests gtest_main)
# Чтобы в тестах не превращались в ошибку предупреждения об unused parameter
target_compile_options(runTests PRIVATE
        -Wno-error=unused-parameter
        -Wno-unused-parameter
)
add_test(NAME runTests COMMAND runTests)
