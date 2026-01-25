# 设置当前 target 名
set(UNITTEST unit_test)

# 引入 FetchContent 管理依赖
include(FetchContent)

# Google Test
FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.17.0
    GIT_SHALLOW TRUE
)

# 安装依赖 Google Test
FetchContent_MakeAvailable(gtest)

# 添加测试运行文件
add_executable(${UNITTEST}
    util/clamped_test.cpp
)

# 引用头文件
target_include_directories(${UNITTEST} PRIVATE
    ${CMAKE_SOURCE_DIR}
)

# 链接 gtest 库
target_link_libraries(${UNITTEST} PRIVATE
    gtest
    gtest_main
)