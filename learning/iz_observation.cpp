#include <thread>
#include <algorithm>
#include <unordered_map>
#include "iz_observation.h"

namespace pvz_emulator::learning {

using namespace pvz_emulator::object;

const int eat_brain_thres = 25;
const int zombie_type_max = 3;
const int plant_type_max = 4;
const int sun_max = 1950;

const std::unordered_map<zombie_type, int> zombie_num_of_type = {
    {zombie_type::zombie, 1},
    {zombie_type::buckethead, 2},
    {zombie_type::football, 3},
};
const std::unordered_map<plant_type, int> plant_num_of_type = {
    {plant_type::sunflower, 1},
    {plant_type::pea_shooter, 2},
    {plant_type::squash, 3},
    {plant_type::snow_pea, 4},
};

float get_zombie_num(const zombie_type& type)
{
    auto it = zombie_num_of_type.find(type);
    if (it == zombie_num_of_type.end()) {
        return 0.0f;
    } else {
        return static_cast<float>(it->second) / zombie_type_max;
    }
}

float get_plant_num(const plant_type& type)
{
    auto it = plant_num_of_type.find(type);
    if (it == plant_num_of_type.end()) {
        return 0.0f;
    } else {
        return static_cast<float>(it->second) / plant_type_max;
    }
}

const unsigned int iz_observation::zombie_size = 6;
const unsigned int iz_observation::plant_size = 4;
const unsigned int iz_observation::meta_size = 6;

iz_observation::iz_observation(
    scene_type type,
    unsigned int num_zombies,
    unsigned int num_plants)
    : num_zombies(num_zombies)
    , num_plants(num_plants)
    , single_size(
          num_zombies * zombie_size + num_plants * plant_size + meta_size)
{
}
std::tuple<std::vector<float>, int, int> iz_observation::create(world& w)
{
    auto& scene = w.scene;
    std::vector<float> ob;
    ob.resize(single_size, 0);

    int i = 0;
    int zombie_count = 0;
    for (auto& z : scene.zombies) {
        if (z.x < eat_brain_thres) {
            scene.brains[z.row] = false;
            continue;
        }
        if (z.is_dead || !z.is_not_dying) {
            continue;
        }
        zombie_count++;

        ob[i++] = (get_zombie_num(z.type));
        ob[i++] = z.x / 650.0f;
        ob[i++] = static_cast<float>(z.row) / scene.rows;
        ob[i++] = static_cast<float>(z.hp) / z.max_hp;
        ob[i++] = static_cast<float>(z.accessory_1.hp) / std::max(1u, z.accessory_1.max_hp);
        ob[i++] = static_cast<float>(z.countdown.slow) / 2000.0f;
    }

    i = num_zombies * zombie_size;
    int plant_count = 0;
    for (auto& p : scene.plants) {
        if (p.status == plant_status::squash_crushed) {
            continue;
        }
        plant_count++;

        ob[i++] = (get_plant_num(p.type));
        ob[i++] = static_cast<float>(p.hp) / p.max_hp;
        ob[i++] = static_cast<float>(p.row) / scene.rows;
        ob[i++] = static_cast<float>(p.col) / 9.0f;
    }

    i = num_zombies * zombie_size + num_plants * plant_size;
    ob[i++] = static_cast<float>(scene.sun.sun) / sun_max;
    for (const auto& b : scene.brains) {
        ob[i++] = b ? 1.0f : 0.0f;
    }

    return {ob, zombie_count, plant_count};
}

}
