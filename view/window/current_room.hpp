#pragma once

namespace view::window {
    enum class room_type {
        none,
        engi_mechgen,
        engi_powerstore
    };

    inline static room_type current_room = room_type::none;
}