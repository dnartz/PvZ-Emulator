#pragma once
#include "object/scene.h"

namespace pvz_emulator::system {

class endgame {
    scene::spawn_data& data;

public:
    endgame(scene& s) : data(s.spawn) {}

    bool update() {
        return data.countdown.endgame > 0 && --data.countdown.endgame == 0;
    }
};

}
