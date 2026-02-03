#pragma once

// STL
#include <cstdio>
#include <string>

// ImGui
#include "imgui.h"

// Project
#include "imgui_internal.h"
#include "model/model.hpp"
#include "color.hpp"
#include "item/other.hpp"
#include "model/task_manager.hpp"

using namespace std::chrono_literals;
using cstring = const char*;

enum class main_window_t {
    none,
    spawn_collapsed,
    spawn_storage,
    engi_mech,
    engi_powerstore,
};

class view {
public:
    // 默认窗口设置
    constexpr inline static ImGuiWindowFlags default_window_config{
        ImGuiWindowFlags_NoMove            |
        ImGuiWindowFlags_NoCollapse        |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_AlwaysAutoResize
    };

    view(model& model_)
        : model_{model_} {}

    void draw() {
        ImGui::SetNextWindowPos(ImVec2{});
        const auto status_window = view::player_status();
        ImGui::SetNextWindowPos({
            status_window->Pos.x,
            status_window->Size.y
        });
        view::side_menu();
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

        static auto current_window = main_window_t::none;
        ImGui::Text("休眠舱");
        ImGui::Indent();

        if (ImGui::Selectable("储物间")) {
            current_window = main_window_t::spawn_storage;
        }
        if (ImGui::Selectable("坍塌的房间")) {
            current_window = main_window_t::spawn_collapsed;
        }
        ImGui::Unindent();

        ImGui::Text("工程部");
        ImGui::Indent();
        if (ImGui::Selectable("机械发电室")) {
            current_window = main_window_t::engi_mech;
        }

        if (ImGui::Selectable("电力存储室")) {
            current_window = main_window_t::engi_powerstore;
        }

        ImGui::End();

        // 绘制主窗口
        switch (current_window) {
            case main_window_t::engi_mech:
                return mechgen();
            case main_window_t::engi_powerstore:
                return powerstore();
            case main_window_t::spawn_storage:
                return storage();
            case main_window_t::spawn_collapsed:
                return collapsed();
            case main_window_t::none:
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
            ImGui::SetItemTooltip("%s", item->get_description().c_str());
        }

        ImGui::End();
        return current_window;
    }

    ImGuiWindow* collapsed() {
        static auto ice_task_ptr = [this] {
            auto ptr = std::make_shared<task>([this] {
                auto& player = model_.player;
                player.hunger.force_minus(10);
                model_.inventory.add_item(std::make_shared<ice_block>(1));
            }, 60s);

            task_manager::instance().add_task(ptr);
            return ptr;
        }();

        static auto metal_task_ptr = [this] {
            auto ptr = std::make_shared<task>([this] {
                auto& player = model_.player;
                player.hunger.force_minus(25);
                model_.inventory.add_item(std::make_shared<metal_scrap>(1));
            }, 60s);

            task_manager::instance().add_task(ptr);
            return ptr;
        }();

        ImGui::Begin("已经坍塌的房间", nullptr, default_window_config);
        auto window = ImGui::GetCurrentWindow();

        static bool ice_is_checked = false;
        if (ImGui::Checkbox("采集冻结的冰块", &ice_is_checked)) {
            ice_is_checked ? ice_task_ptr->start() : ice_task_ptr->stop();
        }

        ImGui::SameLine();
        ImGui::ProgressBar(ice_task_ptr->progress(), ImVec2{});

        static bool metal_is_checked = false;
        if (ImGui::Checkbox("搜寻金属残骸堆", &metal_is_checked)) {
            metal_is_checked ? metal_task_ptr->start() : metal_task_ptr->stop();
        }

        ImGui::SameLine();
        ImGui::ProgressBar(metal_task_ptr->progress(), ImVec2{});

        ImGui::End();
        return window;
    }
private:
    model& model_;
};