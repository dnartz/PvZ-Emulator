#pragma once
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "common.h"
#include "zombie_reanim_data.h"

namespace pvz_emulator::object {

enum class zombie_type {
    none = -1,
    zombie = 0x0,
    flag = 0x1,
    conehead = 0x2,
    pole_vaulting = 0x3,
    buckethead = 0x4,
    newspaper = 0x5,
    screendoor = 0x6,
    football = 0x7,
    dancing = 0x8,
    backup_dancer = 0x9,
    ducky_tube = 0xa,
    snorkel = 0xb,
    zomboni = 0xc,
    dolphin_rider = 0xe,
    jack_in_the_box = 0xf,
    balloon = 0x10,
    digger = 0x11,
    pogo = 0x12,
    yeti = 0x13,
    bungee = 0x14,
    ladder = 0x15,
    catapult = 0x16,
    gargantuar = 0x17,
    imp = 0x18,
    giga_gargantuar = 0x20,
};

enum class zombie_status {
    walking = 0x0,
    dying = 0x1,
    dying_from_instant_kill = 0x2,
    dying_from_lawnmower = 0x3,
    bungee_target_drop = 0x4,
    bungee_body_drop = 0x5,
    bungee_idle_after_drop = 0x6,
    bungee_grab = 0x7,
    bungee_raise = 0x8,
    bungee_idle = 0xa,
    pole_valuting_running = 0xb,
    pole_valuting_jumpping = 0xc,
    pole_vaulting_walking = 0xd,
    rising_from_ground = 0xe,
    jackbox_walking = 0xf,
    jackbox_pop = 0x10,
    pogo_with_stick = 0x14,
    pogo_idle_before_target = 0x15,
    pogo_jump_across = 0x1b,
    newspaper_walking = 0x1d,
    newspaper_destoryed = 0x1e,
    newspaper_running = 0x1f,
    digger_dig = 0x20,
    digger_drill = 0x21,
    digger_lost_dig = 0x22,
    digger_landing = 0x23,
    digger_dizzy = 0x24,
    digger_walk_right = 0x25,
    digger_walk_left = 0x26,
    digger_idle = 0x27,
    dancing_moonwalk = 0x28,
    dancing_point = 0x29,
    dancing_wait_summoning = 0x2a,
    dancing_summoning = 0x2b,
    dancing_walking = 0x2c,
    dancing_armrise1 = 0x2d,
    dancing_armrise2 = 0x2e,
    dancing_armrise3 = 0x2f,
    dancing_armrise4 = 0x30,
    dancing_armrise5 = 0x31,
    backup_spawning = 0x32,
    dophin_walk_with_dophin = 0x33,
    dophin_jump_in_pool = 0x34,
    dophin_ride = 0x35,
    dophin_jump = 0x36,
    dophin_walk_in_pool = 0x37,
    dophin_walk_without_dophin = 0x38,
    snorkel_walking = 0x39,
    snorkel_jump_in_the_pool = 0x3a,
    snorkel_swim = 0x3b,
    snorkel_up_to_eat = 0x3c,
    snorkel_eat = 0x3d,
    snorkel_finied_eat = 0x3e,
    catapult_shoot = 0x43,
    catapult_idle = 0x44,
    balloon_flying = 0x49,
    balloon_falling = 0x4a,
    balloon_walking = 0x4b,
    imp_flying = 0x47,
    imp_landing = 0x48,
    gargantuar_throw = 0x45,
    gargantuar_smash = 0x46,
    ladder_walking = 0x4c,
    ladder_placing = 0x4d,
    yeti_escape = 0x5b,
};

enum class zombie_action {
    none = 0x0,
    entering_pool = 0x1,
    leaving_pool = 0x2,
    caught_by_kelp = 0x3,
    climbing_ladder = 0x6,
    falling = 0x7,
    fall_from_sky = 0x9
};

enum class zombie_accessories_type_1 {
    none = 0x0,
    roadcone = 0x1,
    bucket = 0x2,
    football_cap = 0x3,
    miner_hat = 0x4
};

enum class zombie_accessories_type_2 {
    none = 0x0,
    screen_door = 0x1,
    newspaper = 0x2,
    ladder = 0x3,
};

enum class zombie_hp_status {
    gte_two_third = 0,
    lt_two_third = 1,
    lt_one_third = 2
};

class scene;
class plant;

class zombie {
public:
    zombie_type type;
    zombie_status status;
    zombie_action action;

    int int_x;
    int int_y;

    float x;
    float y;
    float dx;
    float dy;
    float d2y;

    reanim reanim;

    const float* _ground;

    unsigned int row;
    int bungee_col;
    int ladder_col;

    struct {
        int x;
        int y;
        int width;
        int height;
        int offset_x;
        int offset_y;
    } hit_box;

    struct {
        int x;
        int y;
        int width;
        int height;
    } attack_box;

    struct {
        unsigned int butter;
        unsigned int freeze;
        unsigned int slow;
        int action;
        int dead;
    } countdown;

    unsigned int spawn_wave;

    unsigned int time_since_ate_garlic;
    unsigned int time_since_spawn;

    int hp;
    unsigned int max_hp;

    struct {
        zombie_accessories_type_1 type;
        unsigned int hp;
        unsigned int max_hp;
    } accessory_1;

    struct {
        zombie_accessories_type_2 type;
        unsigned int hp;
        unsigned int max_hp;
    } accessory_2;

    int master_id;
    int partners[4];

    struct {
        unsigned int a;
        unsigned int b;
        unsigned int c;
    } garlic_tick;

    plant* bungee_target;

    union {
        unsigned int n_basketballs;
        unsigned int summon_countdown;
    } catapult_or_jackson;

    bool is_eating;
    bool is_dead;
    bool is_blown;
    bool is_not_dying;
    bool is_hypno;
    bool has_item_or_walk_left;
    bool is_in_water;
    bool has_balloon;
    bool has_eaten_garlic;

    bool has_reanim(zombie_reanim_name name) const;

    enum zombie_hp_status get_hp_status() const {
        double r = hp / max_hp;

        if (r < 1.0 / 3.0) {
            return zombie_hp_status::lt_one_third;
        } else if (r < 2.0 / 3.0) {
            return zombie_hp_status::lt_two_third;
        } else {
            return zombie_hp_status::gte_two_third;
        }
    }

    bool is_flying_or_falling() const {
        return status == zombie_status::balloon_flying ||
            status == zombie_status::balloon_falling;
    }

    float get_delta_x_from_ground() const;

    bool is_walk_right() const;

    bool has_death_status() const {
        return status == zombie_status::dying ||
            status == zombie_status::dying_from_instant_kill ||
            status == zombie_status::dying_from_lawnmower;
    }

    bool has_pogo_status() const {
        return status >= zombie_status::pogo_with_stick &&
            static_cast<int>(status) <= 28;
    }

    bool can_be_slowed() const;

    bool can_be_freezed() const;

    bool is_freeable() const {
        return is_dead;
    }

    void get_attack_box(rect &rect) const;
    void get_hit_box(rect &rect) const;

    float get_height_bias() const;

    void init_reanim();

    void set_reanim_frame(zombie_reanim_name name);

    void to_json(scene& s, rapidjson::Writer<rapidjson::StringBuffer>& writer);

    static const char* status_to_string(zombie_status status);
    static const char* type_to_string(zombie_type type);

    static const char*
    accessories_type_to_string(zombie_accessories_type_1 type);

    static const char*
    accessories_type_to_string(zombie_accessories_type_2 type);
};

};
