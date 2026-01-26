# 设置目标名
set(VIEW_WINDOWS view_windows)

# 添加静态库
add_library(${VIEW_WINDOWS}
    STATIC
        status.cpp
)

target_compile_features(${VIEW_WINDOWS} PRIVATE
    cxx_std_23
)

target_include_directories(${VIEW_WINDOWS} PRIVATE
    ${imgui_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${VIEW_WINDOWS} PUBLIC
    imgui
)