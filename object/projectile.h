#pragma once
#ifdef PVZEMU_BUILD_DEBUGGER
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#endif // PVZEMU_BUILD_DEBUGGER

namespace pvz_emulator::object {

enum class projectile_type {
    none = -1,
    pea = 0x0,
    snow_pea = 0x1,
    cabbage = 0x2,
    melon = 0x3,
    puff = 0x4,
    wintermelon = 0x5,
    fire_pea = 0x6,
    star = 0x7,
    cactus = 0x8,
    basketball = 0x9,
    kernel = 0xA,
    cob_cannon = 0xB,
    butter = 0xC
};

enum class projectile_motion_type {
    straight = 0,
    parabola = 1,
    switch_way = 2,
    puff = 5,
    left_straight = 6,
    starfruit = 7,
    cattail = 9
};

class scene;
class zombie;

class projectile {
public:
    projectile_type type;
    projectile_motion_type motion_type;

    int int_x;
    int int_y;

    int row;

    float y;
    float x;
    float shadow_y;

    float dx;

    float dy1;
    float dy2;

    float ddy;
    float dddy;

    struct {
        int width;
        int height;
    } attack_box;

    unsigned int flags;

    unsigned int time_since_created;
    unsigned int countdown;

    int last_torchwood_col;

    float cannon_x;
    int cannon_row;

    int target;

    bool is_visible;
    bool is_disappeared;

    const static unsigned int DAMAGE[14];

    bool is_freeable() {
        return is_disappeared;
    }

    void get_attack_box(rect& r) const;

    unsigned int get_flags_with_zombie(const zombie& z) const;

#ifdef PVZEMU_BUILD_DEBUGGER
    void to_json(
        object::scene& scene,
        rapidjson::Writer<rapidjson::StringBuffer>& writer);

    static const char* type_to_string(projectile_type type);
    static const char* motion_type_to_string(projectile_motion_type type);
#endif // PVZEMU_BUILD_DEBUGGER
};

}
