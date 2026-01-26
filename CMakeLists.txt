cmake_minimum_required(VERSION 3.10.0)
project(game VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)              # 设置 C++ 标准为 23
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # 启用最小标准版本要求
set(CMAKE_CXX_EXTENSIONS OFF)           # 禁用语言扩展
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # 导出编译指令

# 引入 FetchContent 管理依赖
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.5
)

# 安装依赖 glfw imgui
FetchContent_MakeAvailable(glfw imgui)

add_subdirectory(util)
add_subdirectory(view/window)

add_executable(${PROJECT_NAME} main.cpp)

# 引用 imgui 头文件
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${PROJECT_SOURCE_DIR}
)

# 添加源文件
target_sources(${PROJECT_NAME}
    PRIVATE
        # ImGui
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp

        # 窗口以及渲染后端
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# 链接后端依赖库
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        util

        glfw
        opengl32

        ${VIEW_WINDOW}     
)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_subdirectory(tests)
endif ()