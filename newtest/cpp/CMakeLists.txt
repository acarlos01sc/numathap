add_executable(
    numathap_public_api_test
    test_public_api.cpp
)

target_link_libraries(
    numathap_public_api_test
    PRIVATE
        numathap::numathap
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(numathap_public_api_test)