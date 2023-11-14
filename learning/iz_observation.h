#pragma once
#include <vector>
#include <tuple>
#include "world.h"

namespace pvz_emulator::learning {

class iz_observation {
public:
    unsigned int num_zombies;
    unsigned int num_plants;
    unsigned int single_size;

    static const unsigned int zombie_size;
    static const unsigned int plant_size;
    static const unsigned int meta_size;

    iz_observation(
        pvz_emulator::object::scene_type type,
        unsigned int num_zombies,
        unsigned int num_plants);

    std::tuple<std::vector<float>, int, int> create(world& w);
};

}
