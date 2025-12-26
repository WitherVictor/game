include(FetchContent)

FetchContent_Declare(gtest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG v1.17.0
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(gtest)

enable_testing()
add_executable(unit_test
    util/clamped.cpp
)

target_include_directories(unit_test PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(unit_test PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_test)