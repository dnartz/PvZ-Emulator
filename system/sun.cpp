#include <algorithm>
#include "sun.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

const unsigned int sun::MAX_SUN = 9990;

unsigned int sun::gen_nature_sun_countdown() {
    int c = data.natural_sun_generated * 10 + 425;
    return std::min(c, 950) + rng.randint(275);
}

void sun::update() {
    if (scene.type != scene_type::pool &&
        scene.type != scene_type::day &&
        scene.type != scene_type::roof)
    {
        return;
    }

    if (--data.natural_sun_countdown == 0) {
        data.sun = std::min(MAX_SUN, data.sun + 25);
        data.natural_sun_generated++;
        data.natural_sun_countdown = gen_nature_sun_countdown();
    }
}

}