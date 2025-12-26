cmake_minimum_required(VERSION 3.10.0)
project(game VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FETCHCONTENT_QUIET OFF)

include(FetchContent)
 
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9
)
 
FetchContent_MakeAvailable(ftxui)

# 添加目标
file(GLOB_RECURSE PROJECT_INCLUDE_DIRS include/*.hpp)
add_executable(game src/main.cpp)

target_include_directories(game PRIVATE include)

# 设置标准为 C++20
target_compile_options(game
    PRIVATE -std=c++20
    PRIVATE -Wall
    PRIVATE -Wextra
    PRIVATE -pedantic
)

# 链接 FTXUI 的库
target_link_libraries(game
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)