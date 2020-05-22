#pragma once
#include <array>
#include <random>
#include <cassert>

#ifdef PVZEMU_BUILD_DEBUGGER
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#endif // PVZEMU_BUILD_DEBUGGER

#include "object/obj_list.h"
#include "object/griditem.h"
#include "object/zombie.h"
#include "object/plant.h"
#include "object/projectile.h"

namespace pvz_emulator::object {

struct grid_plant_status {
    object::plant* pumpkin;
    object::plant* base;
    object::plant* content;
    object::plant* coffee_bean;

    grid_plant_status():
        pumpkin(nullptr),
        base(nullptr),
        content(nullptr),
        coffee_bean(nullptr) {}
};

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

    std::mt19937 rng;

    unsigned int zombie_dancing_clock;

    unsigned int rows;

    obj_list<object::zombie, 1024> zombies;
    obj_list<object::plant, 512> plants;
    obj_list<object::griditem, 128> griditems;
    obj_list<object::projectile, 1024> projectiles;

    grid_plant_status plant_map[6][9];

    struct spawn_data {
        object::zombie_type spawn_list[20][50];

        unsigned int total_flags;
        unsigned int wave;

        struct {
            unsigned int initial;
            unsigned int threshold;
        } hp;

        struct {
            unsigned int next_wave;
            unsigned int next_wave_initial;
            unsigned int lurking_squad;
            unsigned int hugewave_fade;
            unsigned int endgame;
            unsigned int pool;
        } countdown;

        struct row_random_tuple {
            float b;
            float c;
            float d;
        } row_random[6];

        bool spawn_flags[33];
        bool is_hugewave_shown = false;

        spawn_data() {
            memset(this, 0, sizeof(*this));

            total_flags = 1000;
            countdown.next_wave = 600;
            countdown.next_wave_initial = 600;
        }
    } spawn;

    struct sun_data {
        unsigned int sun;
        unsigned int natural_sun_generated;
        unsigned int natural_sun_countdown;

        sun_data() :
            sun(9990),
            natural_sun_generated(0),
            natural_sun_countdown(0) {}
    } sun;

    struct ice_path_data {
        unsigned int countdown[6];
        int x[6];

        ice_path_data() {
            memset(countdown, 0, sizeof(countdown));
            for (auto& p : x) {
                p = 800;
            }
        }
    } ice_path;

    struct card_data {
        plant_type type;
        plant_type imitater_type;
        unsigned int cold_down;

        card_data() :
            type(plant_type::none),
            imitater_type(plant_type::none),
            cold_down(0) {}
    };
    
    std::array<card_data, 10> cards;

    bool is_game_over;

    bool is_zombie_dance;
    bool is_future_enabled;

    bool stop_spawn;
    bool enable_split_pea_bug;

    scene(scene_type t) : type(t),
        rng(std::random_device()()),
        zombie_dancing_clock(rng() % 10000),
        rows(get_max_row()),
        is_game_over(false),
        is_zombie_dance(false),
        is_future_enabled(false),
        stop_spawn(false),
        enable_split_pea_bug(true) {}

    scene(const scene& s);

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

#ifdef PVZEMU_BUILD_DEBUGGER
    void to_json(rapidjson::Writer<rapidjson::StringBuffer>& writer);
#endif // PVZEMU_BUILD_DEBUGGER

    void reset();

    void reset(scene_type type) {
        this->type = type;
        rows = get_max_row();

        reset();
    }
};

}

