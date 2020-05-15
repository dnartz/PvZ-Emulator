#pragma once
#include <array>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "common.h"
#include "plant_reanim_data.h"

namespace pvz_emulator::object {

enum class plant_type {
    none = -1,
    pea_shooter = 0x0,
    sunflower = 0x1,
    cherry_bomb = 0x2,
    wallnut = 0x3,
    potato_mine = 0x4,
    snow_pea = 0x5,
    chomper = 0x6,
    repeater = 0x7,
    puffshroom = 0x8,
    sunshroom = 0x9,
    fumeshroom = 0xA,
    grave_buster = 0xB,
    hypnoshroom = 0xC,
    scaredyshroom = 0xD,
    iceshroom = 0xE,
    doomshroom = 0xF,
    lily_pad = 0x10,
    squash = 0x11,
    threepeater = 0x12,
    tangle_kelp = 0x13,
    jalapeno = 0x14,
    spikeweed = 0x15,
    torchwood = 0x16,
    tallnut = 0x17,
    seashroom = 0x18,
    plantern = 0x19,
    cactus = 0x1a,
    blover = 0x1b,
    split_pea = 0x1c,
    starfruit = 0x1d,
    pumpkin = 0x1e,
    magnetshroom = 0x1f,
    cabbagepult = 0x20,
    flower_pot = 0x21,
    kernelpult = 0x22,
    coffee_bean = 0x23,
    garlic = 0x24,
    umbrella_leaf = 0x25,
    marigold = 0x26,
    melonpult = 0x27,
    gatling_pea = 0x28,
    twin_sunflower = 0x29,
    gloomshroom = 0x2A,
    cattail = 0x2B,
    winter_melon = 0x2C,
    gold_magnet = 0x2D,
    spikerock = 0x2E,
    cob_cannon = 0x2F,
    imitater = 0x30
};

enum class plant_status {
    idle = 0x0,
    wait = 0x1,
    work = 0x2,
    squash_look = 0x3,
    squash_jump_up = 0x4,
    squash_stop_in_the_air = 0x5,
    squash_jump_down = 0x6,
    squash_crushed = 0x7,
    grave_buster_land = 0x8,
    grave_buster_idle = 0x9,
    chomper_bite_begin = 0xA,
    chomper_bite_success = 0xB,
    chomper_bite_fail = 0xC,
    chomper_chew = 0xD,
    chomper_swallow = 0xE,
    potato_sprout_out = 0xF,
    potato_armed = 0x10,
    spike_attack = 0x12,
    scaredyshroom_scared = 0x14,
    scaredyshroom_scared_idle = 0x15,
    scaredyshroom_grow = 0x16,
    sunshroom_small = 0x17,
    sunshroom_grow = 0x18,
    sunshroom_big = 0x19,
    magnetshroom_working = 0x1A,
    magnetshroom_inactive_idle = 0x1B,
    cactus_short_idle = 0x1E,
    cactus_grow_tall = 0x1F,
    cactus_tall_idle = 0x20,
    cactus_get_short = 0x21,
    tangle_kelp_grab = 0x22,
    cob_cannon_unaramed_idle = 0x23,
    cob_cannon_charge = 0x24,
    cob_cannon_launch = 0x25,
    cob_cannon_aramed_idle = 0x26,
    kernelpult_launch_butter = 0x27,
    umbrella_leaf_block = 0x28,
    umbrella_leaf_shrink = 0x29,
    imitater_explode = 0x2A,
    flower_pot_placed = 0x2F,
    lily_pad_placed = 0x30
};

enum class attack_flags {
    ground = 0x1,
    flying_balloon = 0x2,
    lurking_snorkel = 0x4,
    animating_zombies = 0x10,
    dying_zombies = 0x20,
    digging_digger = 0x40,
    hypno_zombies = 0x80,
};

template <typename T>
unsigned int operator&(T a, attack_flags f) {
    return static_cast<unsigned int>(a) & static_cast<unsigned int>(f);
}

template <typename T>
unsigned int operator|(T a, attack_flags f) {
    return static_cast<unsigned int>(a) | static_cast<unsigned int>(f);
}

enum class plant_edible_status {
    visible_and_edible = 0,
    invisible_and_edible = 1,
    invisible_and_not_edible = 2
};

enum class plant_direction {
    left = 1,
    right = -1
};

class scene;
class zombie;

class plant {
public:
    plant_type type;
    plant_status status;

    int x;
    int y;

    struct {
        int x;
        int y;
    } cannon;

    int hp;
    int max_hp;

    struct {
        int width;
        int height;
    } attack_box;

    unsigned int row;
    unsigned int col;

    unsigned int max_boot_delay;

    plant_direction direction;

    zombie* target;

    plant_type imitater_target;

    struct {
        int status;
        int generate;
        int launch;
        int eaten;
        int awake;
        int effect;
        int dead;
        int blover_disappear;
    } countdown;

    reanim reanim;

    plant_edible_status edible;

    unsigned int threepeater_time_since_first_shot;

    struct {
        bool front;
        bool back;
    } split_pea_attack_flags;

    bool is_dead;
    bool is_smashed;
    bool is_sleeping;
    bool can_attack;

    static const std::array<unsigned int, 49> EFFECT_INTERVAL_TABLE;
    static const std::array<bool, 49> CAN_ATTACK_TABLE;
    static const std::array<unsigned int, 48> COST_TABLE;
    static const std::array<unsigned int, 48> CD_TABLE;

    bool is_squash_attacking() {
        return type == plant_type::squash && (
            status == plant_status::squash_stop_in_the_air ||
            status == plant_status::squash_jump_down ||
            status == plant_status::squash_crushed);
    }
         
    bool is_sun_plant() {
        return type == plant_type::sunflower ||
            type == plant_type::twin_sunflower ||
            type == plant_type::sunshroom;
    }

    bool is_freeable() {
        return is_dead;
    }

    void get_hit_box(rect &rect);
    void get_attack_box(rect& rect, bool is_alt_attack = false);

    unsigned int get_attack_flags(bool is_alt_attack = false);

    void set_sleep(bool is_sleep);

    void init_reanim();

    void set_reanim_frame(plant_reanim_name name);
    void set_reanim(plant_reanim_name name, reanim_type type, float fps);
    bool has_reanim(plant_reanim_name name);

    void to_json(scene& scene, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    static const char* type_to_string(plant_type type);
    static const char* status_to_string(plant_status status);
};

};
