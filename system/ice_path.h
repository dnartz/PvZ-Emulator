#pragma once
#include "object/scene.h"

namespace pvz_emulator::system {

class ice_path {
    object::scene& scene;
    object::scene::ice_path_data& data;

public:
    ice_path(object::scene& s) : scene(s), data(s.ice_path) {}

    void update() {
        for (auto i = 0u; i < scene.rows; i++) {
            if (data.countdown[i] > 0 && --data.countdown[i] == 0) {
                data.x[i] = 800;
            }
        }
    }
};

}
