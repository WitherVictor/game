// glfw
#include <GLFW/glfw3.h>

// ImGui backends
#include <filesystem>
#include <fstream>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// nlohmann json
#include <iostream>
#include <nlohmann/json.hpp>

#include <memory>
#include <ranges>
#include <stdexcept>
#include <thread>

#include "item/attribute/hydration.hpp"
#include "model/task_manager.hpp"
#include "view/view.hpp"

#include "item/factory.hpp"

#include "item/attribute/factory.hpp"
#include "item/attribute/nutrition.hpp"
#include "item/attribute/stackable.hpp"

// 全局 glfw 错误处理回调函数
static void glfw_error_callback(int error_code, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error_code, description);
}

// 手动注册物品属性
void register_attribute_factory() {
    auto& factory = attribute_factory::instance();

    factory.register_creator("nutrition", [] {
        return std::make_unique<attribute_nutrition>();
    });

    factory.register_creator("hydration", [] {
        return std::make_unique<attribute_hydration>();
    });

    factory.register_creator("stackable", [] {
        return std::make_unique<attribute_stackable>();
    });
}

void register_item_factory() {
    // 这名字太长了有点
    namespace fs = std::filesystem;
    using json = nlohmann::json;

    // 物品数据存放路径
    const fs::path data_path = fs::current_path().parent_path() / "item" / "data";

    // 筛选 item/data 目录下的所有物品的 json 数据文件
    auto json_files = fs::recursive_directory_iterator(data_path)
                        | std::views::filter([] (const auto& entry) { 
                            return fs::is_regular_file(entry.status()) && entry.path().extension() == ".json"; })
                        | std::views::transform([] (const auto& entry) { return entry.path(); })
                        | std::ranges::to<std::vector<fs::path>>();

    // 注册物品信息到工厂内
    auto& item_factory = item_factory::instance();
    for (const auto& path : json_files) {
        std::ifstream ifs{path};
        json item_data = json::parse(ifs);

        item_factory.register_creator(item_data.at("id").get<std::string>(), [item_data] {
            // 设置物品 ID，名字与描述文本
            auto new_item_ptr = std::make_unique<item>();
            new_item_ptr->id = item_data.at("id").get<std::string>();
            new_item_ptr->name = item_data.at("name").get<std::string>();
            new_item_ptr->description = item_data.at("description").get<std::string>();
            new_item_ptr->stackable = item_data.at("stackable").get<bool>();

            // 若存在属性，则为物品读取属性
            if (item_data.contains("attributes")) {
                auto attribute_json = item_data.at("attributes");
                new_item_ptr->attributes.reserve(attribute_json.size());

                // 遍历数据为物品注册属性
                auto& attribute_factory = attribute_factory::instance();
                for (const auto& attribute_data : attribute_json) {
                    // 从 JSON 获取属性名并构造为属性对象
                    auto attribute_type = attribute_data.at("type").get<std::string>();
                    auto new_attribute = attribute_factory.create(attribute_type);

                    // 初始化属性数据并放入物品内
                    new_attribute->from_json(attribute_data);
                    new_item_ptr->attributes.push_back(std::move(new_attribute));
                }
            }

            // 返回构造完成的新物品
            return new_item_ptr;
        });
    }
}

int main() {
    constexpr const char* title = "Escape from Charlie Station";
    constexpr std::size_t window_width = 1280;
    constexpr std::size_t window_height = 800;

    // 设置全局回调函数
    glfwSetErrorCallback(glfw_error_callback);

    // 如果 glfw 初始化失败，则直接抛出异常
    if (!glfwInit())
        throw std::runtime_error{"GLFW initialization failed!"};

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    auto backend_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    auto backend_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    auto backend_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    auto backend_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // 初始化窗口
    auto main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    auto window = glfwCreateWindow(
        (int)(window_width * main_scale),
        (int)(window_height * main_scale),
        title,
        nullptr,
        nullptr
    );
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

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
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(backend_version);

    // 启动更新线程
    std::jthread{[](std::stop_token st) {
        // 每帧的时间
        using namespace std::chrono_literals;
        constexpr auto frame_rate = 16ms;

        // 记录下一针的时间点
        auto next_frame = std::chrono::steady_clock::now();

        while (!st.stop_requested()) {
            task_manager::instance().update_all(frame_rate);

            // 推进一帧并等待直到下一帧
            next_frame += frame_rate;
            std::this_thread::sleep_until(next_frame);
        }
    }}.detach();

    // 按顺序注册物品属性和物品
    register_attribute_factory();
    register_item_factory();

    view view_obj;

    // 运行窗口循环
    while (!glfwWindowShouldClose(window)) {
        // 轮询事件
        glfwPollEvents();

        // 开始 ImGui 帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 绘制游戏界面
        view_obj.draw();

        // 绘制 ImGui 界面
        ImGui::Render();

        // glfw 绘制窗口
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 清理资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}