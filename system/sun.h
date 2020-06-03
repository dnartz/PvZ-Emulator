#pragma once
#include <algorithm>
#include "object/scene.h"
#include "rng.h"

namespace pvz_emulator::system {

class sun {
    static const unsigned int MAX_SUN;

    object::scene& scene;
    object::scene::sun_data& data;
    system::rng rng;

    unsigned int gen_nature_sun_countdown();
public:
    void add_sun(unsigned int sun) {
        scene.sun.sun = std::min(MAX_SUN, scene.sun.sun + sun);
    }

    sun(object::scene &s): scene(s), data(s.sun), rng(s) {
        data.natural_sun_countdown = gen_nature_sun_countdown();
    }

    void update();
};

}
