#pragma once
#include <random>
#include <cassert>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "object/obj_list.h"
#include "object/griditem.h"
#include "object/zombie.h"
#include "object/plant.h"
#include "object/projectile.h"

namespace pvz_emulator::object {

enum class scene_type {
    day = 0x0,
    night = 0x1,
    pool = 0x2,
    fog = 0x3,
    roof = 0x4,
    moon_night = 0x5,
};

class scene {
public:
    scene_type type;

    std::mt19937 rng{std::random_device()()};

    unsigned int zombie_dancing_clock;

    unsigned int rows;

    typename obj_list<object::zombie, 1024> zombies{};
    typename obj_list<object::plant, 512> plants{};
    typename obj_list<object::griditem, 128> griditems{};
    typename obj_list<object::projectile, 1024> projectiles{};

    struct spawn_data {
        object::zombie_type spawn_list[20][50];

        unsigned int total_flags = 1000;
        unsigned int wave;

        struct {
            unsigned int initial;
            unsigned int threshold;
        } hp;

        struct {
            unsigned int next_wave = 600;
            unsigned int next_wave_initial = 600;
            unsigned int lurking_squad = 0;
            unsigned int hugewave_fade = 0;
            unsigned int endgame = 0;
            unsigned int pool = 0;
        } countdown;

        struct row_random_tuple {
            float b;
            float c;
            float d;
        } row_random[6] = { 0 };

        bool spawn_flags[33] = { 0 };
        bool is_hugewave_shown = false;
    } spawn;

    struct sun_data {
        unsigned int sun = 9990;
        unsigned int natural_sun_generated = 0;
        unsigned int natural_sun_countdown = 0;
    } sun;

    struct ice_path_data {
        unsigned int countdown[6] = { 0 };
        int x[6] = { 800 , 800, 800, 800, 800, 800};
    } ice_path;

    struct card_data {
        plant_type type;
        plant_type imitater_type;
        unsigned int cold_down;

        card_data() :
            type(plant_type::none),
            imitater_type(plant_type::none),
            cold_down(0) {}
    } cards[10];

    bool is_game_over;

    bool is_zombie_dance;
    bool is_future_enabled;

    bool stop_spawn;
    bool enable_split_pea_bug;

    scene(scene_type t) : type(t),
        zombie_dancing_clock(rng() % 10000),
        rows(get_max_row()),
        is_zombie_dance(false),
        is_future_enabled(false),
        is_game_over(false),
        stop_spawn(false),
        enable_split_pea_bug(true) {}

    unsigned int get_n_zombies_alive_and_not_hypno() {
        unsigned int n = 0;

        for (auto& z : zombies) {
            if (z.is_not_dying && !z.has_death_status() && !z.is_hypno) {
                n++;
            }
        }

        return n;
    }

    bool is_water_grid(int row, int col) {
        if (type != scene_type::fog && type != scene_type::pool) {
            return false;
        }

        return (row == 2 || row == 3) && (col >= 0 && col <= 8);
    }

    plant* find_flower_pot(int row, int col) {
        for (auto& p : plants) {
            if (p.row == row &&
                p.col == col &&
                !p.is_squash_attacking() &&
                !p.is_smashed && 
                p.edible != plant_edible_status::invisible_and_not_edible &&
                p.type == plant_type::flower_pot)
            {
                return &p;
            }
        }
        
        return nullptr;
    }

    unsigned int get_max_row() {
        return type == scene_type::pool || type == scene_type::fog ?  6 : 5;
    }

    void to_json(rapidjson::Writer<rapidjson::StringBuffer>& writer);

    void reset();

    void reset(scene_type type) {
        this->type = type;
        rows = get_max_row();

        reset();
    }
};

}

