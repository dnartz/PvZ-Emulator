#pragma once
#include "object/scene.h"
#include "zombie.h"
#include "system/damage.h"
#include "system/debuff.h"
#include "system/plant/plant_factory.h"
#include "system/zombie/zombie_factory.h"

namespace pvz_emulator::system {

class zombie_system {
    object::scene& scene;
    rng rng;
    damage damage;
    debuff debuff;
    plant_factory plant_factory;
    zombie_factory zombie_factory;

    reanim reanim;

    zombie_subsystems subsystems;

    void update_falling(object::zombie& z);
    void update_action_in_pool(object::zombie& z);
    void update_climb_ladder(object::zombie& z);

    void crush_plant(object::zombie& z);
    object::zombie* find_hypno_enemy(object::zombie& z);

    void update_eating_plants(object::zombie& z, object::plant &p);

    void update_status(object::zombie& z);
    void update_pos(object::zombie& z);
    void update_eating(object::zombie& z);
    void update_water_status(object::zombie& z);
    bool update_entering_home(object::zombie& z);

    void update_blocked_by_tallnut_or_fall_from_ladder(object::zombie &z);
    void update_dead_from_plant(object::zombie& z);
    void update_lurking_dy(object::zombie& z);
    void update_near_death(object::zombie& z);

    bool not_effected_by_future(const object::zombie& z);
    void set_hypno_effect(object::zombie& z);
    void set_garlic_row_switch(object::zombie& z);
    void set_garlic_and_hypno_status(object::zombie& z);
    void update_garlic_and_hypno_effect(object::zombie& z);

    void update_x(object::zombie& z);

public:
    zombie_system(object::scene& s) :
        scene(s),
        rng(s),
        damage(s),
        debuff(s),
        plant_factory(s),
        zombie_factory(s),
        reanim(s),
        subsystems(s) {}

    bool update();
};

}
