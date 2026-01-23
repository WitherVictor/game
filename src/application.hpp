#pragma once

// STL
#include <cstdio>
#include <stdexcept>

// ImGui
#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"

// Project
#include "util/types.hpp"

// 全局 glfw 错误处理回调函数
static void glfw_error_callback(int error_code, cstring description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error_code, description);
}

class application {
public:
    // 定义标题，窗口宽度与长度
    constexpr inline static cstring title = "Escape from Charlie Station";
    constexpr inline static std::size_t window_width = 1280;
    constexpr inline static std::size_t window_height = 800;

    // 定义后端版本字符串
    inline static cstring backend_version = nullptr;

    application() {
        window_init();
        check_backend_version();
        imgui_init();
    }

    ~application() {
        // 清理资源
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    //  使用 glfw 创建窗口
    void window_init() {
        //  设置全局回调函数
        glfwSetErrorCallback(glfw_error_callback);

        // 如果 glfw 初始化失败，则直接抛出异常
        if (!glfwInit())
            throw std::runtime_error{"GLFW initialization failed!"};

        // 初始化窗口
        auto main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
        window = glfwCreateWindow(
            (int)(window_width * main_scale),
            (int)(window_height * main_scale),
            title,
            nullptr,
            nullptr
        );
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
    }

    // 判断后端使用的版本
    void check_backend_version() {
        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100 (WebGL 1.0)
        backend_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
        // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
        backend_version = "#version 300 es";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
        backend_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
        // GL 3.0 + GLSL 130
        backend_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
    }

    void imgui_init() {
        // 检查 ImGui 版本并创建上下文
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // 创建 IO 并启用键盘导航
        io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        // 设置 ImGui 样式
        ImGui::StyleColorsClassic();

        // 设置平台与渲染后端
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(backend_version);
    }

    void run() {
        while (!glfwWindowShouldClose(window)) {
            run_loop_start();

            ImGui::Begin("Hello world");

            ImGui::Text("Hello this is a text ⛏️");

            ImGui::End();

            run_loop_end();
        }
    }

    void run_loop_start() {
        // 轮询事件
        glfwPollEvents();

        // 开始 ImGui 帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void run_loop_end() {
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
private:
    ImGuiIO io;
    GLFWwindow* window;
};