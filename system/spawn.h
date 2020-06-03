#pragma once

#include <array>

#include "object/scene.h"
#include "object/zombie.h"
#include "system/griditem_factory.h"
#include "system/zombie/zombie_factory.h"
#include "rng.h"

namespace pvz_emulator::system {

using namespace ::pvz_emulator::object;

class spawn {
private:
    void gen_spawn_list();
    void gen_spawn_flags();

public:
    spawn(object::scene& s);

    unsigned int get_current_hp();

    void reset();

    void update();

private:
    static const unsigned int MAX_WAVES;
    static const unsigned int MAX_ZOMBIES;
    static const unsigned int MAX_ZOMBIE_TYPES;

    static const unsigned int N_LURKERS;
    static const unsigned int N_HUGEWAVE_SQUAD;

    static const unsigned int NIGHT_MAX_GRAVES;

    static const unsigned int ZOMBIE_SPAWN_WEIGHT[33];

    unsigned int curr_spawn_count[33];
    unsigned int total_spawn_count[33];

    object::scene &scene;
    scene::spawn_data &data;
    system::zombie_factory zombie_factory;
    system::griditem_factory griditem_factory;

    system::rng rng;

    static bool is_wave_9_or_19(unsigned w) {
        return w == 9 || w == 19;
    }

    bool is_wave_9_or_19() {
        return data.wave == 9 || data.wave == 19;
    }

    constexpr unsigned int get_spawn_weight(zombie_type type);

    bool can_spawn(zombie_type type);
    bool can_spawn(zombie_type type, unsigned wave);

    zombie_type gen_lurking_zombie_type();

    void spawn_list_append(unsigned int wave, unsigned int i, zombie_type type) {
        data.spawn_list[wave][i] = type;
        curr_spawn_count[static_cast<int>(type)]++;
        total_spawn_count[static_cast<int>(type)]++;
    }

    void pool_lurking_spawn();
    void roof_lurking_spawn();
    void night_lurking_spawn();

    void night_grave_spawn();

    void lurking_squad_update();

    void hugewave_warn_update();
    void next_spawn_countdown_update();
};

};