#pragma once

// STL
#include <cstdio>
#include <stdexcept>
#include <string>

// ImGui
#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"

// Project
#include "imgui_internal.h"
#include "model/model.hpp"
#include "color.hpp"

using namespace std::chrono_literals;
using cstring = const char*;

enum class main_window_type {
    none,
    spawn_storage,
    engi_mech,
    engi_powerstore,
};

// 全局 glfw 错误处理回调函数
static void glfw_error_callback(int error_code, cstring description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error_code, description);
}

class view {
public:
    // 定义标题，窗口宽度与长度
    constexpr inline static cstring title = "Escape from Charlie Station";
    constexpr inline static std::size_t window_width = 1280;
    constexpr inline static std::size_t window_height = 800;
    constexpr inline static ImGuiWindowFlags default_window_config{
        ImGuiWindowFlags_NoMove            |
        ImGuiWindowFlags_NoCollapse        |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_AlwaysAutoResize
    };

    // 定义后端版本字符串
    inline static cstring backend_version = nullptr;

    view(model& model_)
        : model_{model_}
    {
        window_init();
        check_backend_version();
        imgui_init();
    }

    ~view() {
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
        window_ = glfwCreateWindow(
            (int)(window_width * main_scale),
            (int)(window_height * main_scale),
            title,
            nullptr,
            nullptr
        );
        glfwMakeContextCurrent(window_);
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
        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        // 添加中文字符集
        io.Fonts->AddFontFromFileTTF(
            "C:\\Windows\\Fonts\\msyh.ttc",
            18.0f,
            nullptr,
            io.Fonts->GetGlyphRangesChineseFull()
        );

        // 设置 ImGui 样式
        ImGui::StyleColorsClassic();

        // 设置平台与渲染后端
        ImGui_ImplGlfw_InitForOpenGL(window_, true);
        ImGui_ImplOpenGL3_Init(backend_version);
    }

    void run() {
        while (!glfwWindowShouldClose(window_)) {
            run_loop_start();

            ImGui::SetNextWindowPos(ImVec2{});
            const auto status_window = view::player_status();
            ImGui::SetNextWindowPos({
                status_window->Pos.x,
                status_window->Size.y
            });
            view::side_menu();

            run_loop_end();
        }
    }

    // 绘制人物状态栏
    ImGuiWindow* player_status() {
        ImGui::Begin("人物状态", nullptr, default_window_config);

        auto window = ImGui::GetCurrentWindow();

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, Color::Red);
        ImGui::Text("%s", "生命");
        ImGui::SameLine();
        const auto health_values = model_.player.health.values();
        ImGui::ProgressBar(
            health_values.ratio,
            ImVec2{},
            std::to_string(health_values.now).c_str());
        ImGui::PopStyleColor();

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, Color::Brown);
        ImGui::Text("%s", "饥饿");
        ImGui::SameLine();
        const auto hunger_values = model_.player.hunger.values();
        ImGui::ProgressBar(hunger_values.ratio, ImVec2{});
        ImGui::PopStyleColor();

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, Color::LightBlue);
        ImGui::Text("%s", "口渴");
        ImGui::SameLine();
        const auto thirst_values = model_.player.thirst.values();
        ImGui::ProgressBar(thirst_values.ratio, ImVec2{}, std::to_string(thirst_values.now).c_str());
        ImGui::PopStyleColor();

        ImGui::End();

        return window;
    }

    // 绘制人物侧边栏
    ImGuiWindow* side_menu() {
        ImGui::Begin("空间站", nullptr, default_window_config);

        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();
        ImGui::SetNextWindowPos({
            pos.x + size.x,
            pos.y,
        });

        static auto current_window = main_window_type::none;
        ImGui::Text("休眠舱");
        ImGui::Indent();

        if (ImGui::Selectable("储物间")) {
            current_window = main_window_type::spawn_storage;
        }
        ImGui::Unindent();

        ImGui::Text("工程部");
        ImGui::Indent();
        if (ImGui::Selectable("机械发电室")) {
            current_window = main_window_type::engi_mech;
        }

        if (ImGui::Selectable("电力存储室")) {
            current_window = main_window_type::engi_powerstore;
        }

        ImGui::End();

        // 绘制主窗口
        switch (current_window) {
            case main_window_type::engi_mech:
                return mechgen();
            case main_window_type::engi_powerstore:
                return powerstore();
            case main_window_type::spawn_storage:
                return storage();
            case main_window_type::none:
                [[fallthrough]];
            default:
                return nullptr;
        }
    }

    ImGuiWindow* mechgen() {
        static auto task_ptr = [this]() {
            auto ptr = std::make_shared<task>([this] {
                if (model_.player.hunger.try_minus()) {
                    model_.electricity.power.force_add();
                }
            }, 1s);

            task_manager::instance().add_task(ptr);
            return ptr;
        }();

        ImGui::Begin("机械发电室", nullptr, default_window_config);
        auto window_ptr = ImGui::GetCurrentWindow();;

        static bool is_checked = false;
        if (ImGui::Checkbox("人力发电机", &is_checked)) {
            task_ptr->reverse_condition();
        }

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, Color::ElectricityYellow);
        ImGui::ProgressBar(task_ptr->progress(), ImVec2{});
        ImGui::PopStyleColor();

        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
        ImGui::SetItemTooltip("%s", "每秒获得 1 点电力，消耗饥饿值。");

        ImGui::End();

        return window_ptr;
    }

    ImGuiWindow* powerstore() {
        ImGui::Begin("电力存储室", nullptr, default_window_config);
        auto window_ptr = ImGui::GetCurrentWindow();

        ImGui::Text("%s", "电力");
        ImGui::SameLine();
        const auto electricity = model_.electricity.power.values();
        ImGui::ProgressBar(
            electricity.ratio,
            ImVec2{},
            std::format("{} KJ", electricity.now).c_str());

        ImGui::End();

        return window_ptr;
    }

    ImGuiWindow* storage() {
        ImGui::Begin("储物间", nullptr, default_window_config);
        auto current_window = ImGui::GetCurrentWindow();

        auto items = model_.inventory.get_items();
        constexpr std::size_t line_length = 8;

        for (auto& item : items) {
            if (item->get_amount() == 0)
                continue;

            auto label = std::format("{} x{}", item->get_name(), item->get_amount());
            if (ImGui::Selectable(label.c_str())) {
                item->use();
            }
        }

        ImGui::End();
        return current_window;
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
        glfwGetFramebufferSize(window_, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);
    }
private:
    GLFWwindow* window_;

    model& model_;
};