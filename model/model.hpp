#ifndef MODEL_MODEL_HPP
#define MODEL_MODEL_HPP

#include "electricity.hpp"
#include "player.hpp"
class model {
public:
    player& get_player() {
        return player_;
    }

    electricity& get_electricity() {
        return electricity_;
    }
private:
    electricity electricity_;

    player player_;
};

#endif // End of MODEL_MODEL_HPP