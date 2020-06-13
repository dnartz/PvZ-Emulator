#pragma once
#include <vector>
#include <functional>
#include "world.h"

namespace pvz_emulator::learning {

class observation_factory {
private:
    template<class T>
    float *fill_ob_vector(
        pvz_emulator::object::obj_list<T>& list,
        float *base,
        unsigned int size,
        unsigned int max,
        std::function<void (float *base, const T& t)> cb)
    {
        auto n = list.size() < max ? list.size() : max;
        auto it = list.begin();

        for (unsigned i = 0; i < n; i++) {
            cb(base + size * i, *it);
        }

        return base + size * max;
    }

    void make_observation(world& w, float *base);

public:
    unsigned int num_zombies;
    unsigned int num_plants;
    unsigned int num_projectiles;
    unsigned int num_griditems;
    unsigned int num_ice_paths;
    unsigned int single_size;

    static const unsigned int zombie_size;
    static const unsigned int plant_size;
    static const unsigned int projectiles_size;
    static const unsigned int griditems_size;
    static const unsigned int meta_size;
    static const unsigned int spawn_flags_size;

    observation_factory(
        pvz_emulator::object::scene_type type,
        unsigned int num_zombies,
        unsigned int num_plants,
        unsigned int num_projectiles,
        unsigned int num_griditems);

    void create(std::vector<world *>& worlds, std::vector<float>& ob);
};

}
