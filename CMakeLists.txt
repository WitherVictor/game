cmake_minimum_required(VERSION 3.14)
project(game
    VERSION 0.1.0
    DESCRIPTION "A text ui idle game using FTXUI"
    LANGUAGES CXX
)

# 设置标准版本以及导出编译指令
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
 
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9
  GIT_SHALLOW TRUE
)
 
FetchContent_MakeAvailable(ftxui)

# 添加目标
add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE include)

# 设置标准为 C++20
target_compile_options(${PROJECT_NAME}
    PRIVATE -Wall
    PRIVATE -Wextra
    PRIVATE -pedantic
)

# 链接 FTXUI 的库
target_link_libraries(${PROJECT_NAME}
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)