cmake_minimum_required(VERSION 3.10.0)
project(game VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

add_executable(game main.cpp)

target_compile_options(game PRIVATE
    -O2
    -Wall
    -Wextra
    -pedantic
    -std=c++20
)

target_link_libraries(game PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
    atomic
)