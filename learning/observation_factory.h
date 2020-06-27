#pragma once
#include <vector>
#include <functional>
#include "world.h"

namespace pvz_emulator::learning {

class observation_factory {
private:
    template<class T, size_t S>
    float *fill_ob_vector(
        pvz_emulator::object::obj_list<T, S>& list,
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

    void create(world& w, const world::action_masks& masks, float *ob);

    void create(
        world& world,
        const world::action_masks& action_masks,
        std::vector<float> &ob);

    void create(
        std::vector<world *> &worlds,
        const world::batch_action_masks& action_masks,
        std::vector<float> &ob);
};

}
