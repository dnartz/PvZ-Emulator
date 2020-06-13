#include <thread>
#include "observation_factory.h"

namespace pvz_emulator::learning {

using namespace pvz_emulator::object;

const unsigned int observation_factory::zombie_size = 14;
const unsigned int observation_factory::plant_size = 9;
const unsigned int observation_factory::projectiles_size = 4;
const unsigned int observation_factory::griditems_size = 3;
const unsigned int observation_factory::meta_size = 7;
const unsigned int observation_factory::spawn_flags_size = 33;

observation_factory::observation_factory(
        scene_type type,
        unsigned int num_zombies,
        unsigned int num_plants,
        unsigned int num_projectiles,
        unsigned int num_griditems) :
    num_zombies(num_zombies),
    num_plants(num_plants),
    num_griditems(num_griditems),
    num_projectiles(num_projectiles),
    num_ice_paths(type == scene_type::night ? 0 :
        type == scene_type::pool || type == scene_type::fog ? 6 : 5),
    single_size(
        num_zombies * zombie_size +
        num_plants * plant_size +
        num_projectiles * projectiles_size +
        num_griditems * griditems_size +
        30 + // Cards
        2 * num_ice_paths +
        spawn_flags_size +
        meta_size)
{}

void observation_factory::create(
    std::vector<world *> &worlds,
    const world::batch_action_masks& action_masks,
    std::vector<float> &ob)
{
    ob.clear();
    ob.resize(worlds.size() * (single_size + action_masks[0].size()), 0);

    std::atomic<decltype(worlds.size())> i = 0;
    std::vector<std::thread> threads;

    for (unsigned int j = 0; j < std::thread::hardware_concurrency(); j++) {
        threads.emplace_back([&]() {
            for (auto k = i.fetch_add(1); k < worlds.size(); k = i.fetch_add(1)) {
                make_observation(*worlds[k], action_masks[k], &ob[single_size * k]);
            }
        });
    }

    for (auto& t: threads) {
        t.join();
    }
}

void observation_factory::make_observation(
    world &w,
    const world::action_masks& masks,
    float *base)
{
    auto& scene = w.scene;

    base = fill_ob_vector<zombie>(
        scene.zombies,
        base,
        zombie_size,
        num_zombies,
        [](float *a, const zombie& z) {
            a[0] = static_cast<float>(z.type);
            a[1] = static_cast<float>(z.status);
            a[2] = static_cast<float>(z.action);
            a[3] = z.x;
            a[4] = z.y;
            a[5] = static_cast<float>(z.row);
            a[6] = static_cast<float>(z.bungee_col);
            a[7] = static_cast<float>(z.hp);
            a[8] = static_cast<float>(z.accessory_1.hp);
            a[9] = static_cast<float>(z.accessory_2.hp);
            a[10] = static_cast<float>(z.countdown.freeze);
            a[11] = static_cast<float>(z.countdown.butter);
            a[12] = static_cast<float>(z.countdown.slow);
            a[13] = static_cast<float>(z.countdown.action);
        });

    base = fill_ob_vector<plant>(
        scene.plants,
        base,
        plant_size,
        num_plants,
        [](float *a, const plant& p) {
            a[0] = static_cast<float>(p.type);
            a[1] = static_cast<float>(p.status);
            a[2] = static_cast<float>(p.hp);
            a[3] = static_cast<float>(p.row);
            a[4] = static_cast<float>(p.col);
            a[5] = static_cast<float>(p.countdown.status);
            a[6] = static_cast<float>(p.countdown.effect);
            a[7] = static_cast<float>(p.countdown.awake);
            a[8] = static_cast<float>(p.countdown.launch);
        });

    base = fill_ob_vector<projectile>(
        scene.projectiles,
        base,
        projectiles_size,
        num_projectiles,
        [](float *a, const projectile& proj) {
            a[0] = static_cast<float>(proj.type);
            a[1] = static_cast<float>(proj.row);
            a[2] = proj.x;
            a[3] = proj.y;
        });

    base = fill_ob_vector<griditem>(
        scene.griditems,
        base,
        griditems_size,
        num_griditems,
        [](float *a, const griditem& item) {
            a[0] = static_cast<float>(item.type);
            a[1] = static_cast<float>(item.row);
            a[2] = static_cast<float>(item.col);
        });

    for (unsigned int i = 0; i < 10; i++) {
        base[3 * i + 0] = static_cast<float>(scene.cards[i].type);
        base[3 * i + 1] = static_cast<float>(scene.cards[i].imitater_type);
        base[3 * i + 2] = static_cast<float>(scene.cards[i].cold_down);
    }

    base += 30;

    for (unsigned i = 0; i < num_ice_paths; i++) {
        base[2 * i + 0] = scene.ice_path.x[i];
        base[2 * i + 0] = scene.ice_path.countdown[i];
    }

    base += num_ice_paths * 2;

    base[0] = static_cast<float>(scene.spawn.wave);
    base[1] = static_cast<float>(scene.spawn.countdown.next_wave);
    base[2] = static_cast<float>(scene.spawn.countdown.lurking_squad);
    base[3] = static_cast<float>(scene.spawn.countdown.hugewave_fade);
    base[4] = static_cast<float>(scene.spawn.countdown.endgame);
    base[5] = static_cast<float>(scene.spawn.countdown.pool);
    base[6] = static_cast<float>(scene.sun.sun);

    base += 7;
    std::copy(masks.cbegin(), masks.cend(), base);
}

}
