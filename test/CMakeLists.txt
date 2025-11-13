

add_executable(my_test_runner gold_code_test.cpp ../src/C_A__code_generator.cpp)
target_include_directories(my_test_runner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

target_link_libraries(my_test_runner PRIVATE GTest::gtest GTest::gtest_main)