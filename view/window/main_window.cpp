

#include "current_room.hpp"
#include "window_info.hpp"
#include "engi/mechgen.cpp"

namespace view::window {
    inline window::info main_window() {
        switch (window::current_room) {
            case room_type::engi_mechgen:
                return engi::mechgen::draw();
            case room_type::none:
                [[fallthrough]];
            default:
                return {};
        }
    }
}