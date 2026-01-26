add_library(util
    STATIC
        color.cpp
)

target_include_directories(util PRIVATE
    ${imgui_SOURCE_DIR}
)