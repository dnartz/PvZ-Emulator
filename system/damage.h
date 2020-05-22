#pragma once
#include "object/scene.h"
#include "object/zombie.h"
#include "system/rng.h"
#include "system/debuff.h"
#include "system/zombie/zombie_factory.h"
#include "system/plant/plant_factory.h"
#include "system/plant/plant.h"
#include "system/griditem_factory.h"
#include "system/reanim.h"

namespace pvz_emulator::system {

enum class zombie_damage_flags {
    ignore_accessory_2 = 0x1,
    not_reduce = 0x2,
    slow_effect = 0x4,
    no_flash = 0x8,
    disable_ballon_pop = 0x10,
    spike = 0x20
};

template <typename T>
unsigned int operator&(T a, zombie_damage_flags f) {
    return static_cast<unsigned int>(a) & static_cast<unsigned int>(f);
}

template <typename T>
unsigned int operator|(T a, zombie_damage_flags f) {
    return static_cast<unsigned int>(a) | static_cast<unsigned int>(f);
}

class damage {
    object::scene& scene;
    rng rng;
    debuff debuff;
    reanim reanim;
    plant_factory plant_factory;
    zombie_factory zombie_factory;
    griditem_factory griditem_factory;
    plant_spikerock spikerock;

    void set_death_state(object::zombie& z, unsigned int flags);

    void take_body(object::zombie& z, unsigned int damage, unsigned int flags);

    void take_ash_attack(object::zombie &z);
public:
    bool can_be_attacked(const object::zombie& z, unsigned char flags);

    void set_is_eating(object::zombie& z);
    void unset_is_eating(object::zombie& z);

    void destroy_accessory_2(object::zombie& z);

    void set_smashed(object::plant& p);

    void activate_blover();
    void activate_plant(object::plant &p);

    void range_attack(object::plant& p, unsigned int flags);

    void take_instant_kill(
        int row,
        int x,
        int y,
        int radius,
        int grid_radius,
        bool is_ash_attack,
        unsigned char flags);

    void take(object::zombie &z, unsigned int damage, unsigned int flags);

    damage(object::scene &s) :
        scene(s),
        rng(s),
        debuff(s),
        reanim(s),
        plant_factory(s),
        zombie_factory(s),
        griditem_factory(s),
        spikerock(s) {}
};

}
