#pragma once
#include "object/scene.h"
#include "system/rng.h"
#include "system/reanim.h"
#include "system/damage.h"
#include "system/plant/plant_factory.h"
#include "system/projectile/projectile_factory.h"

namespace pvz_emulator::system {

class plant_system {
private:
    object::scene& scene;
    system::plant_factory plant_factory;
    system::projectile_factory projectile_factory;
    system::damage damage;
    system::reanim reanim;
    system::rng rng;
    system::plant_subsystems subsystems;

    void get_pea_offset(const object::plant& p, int &x, int &y);

    void update_launch_countdown(object::plant& p);
    void update_countdown_and_status(object::plant& p);
    void update_attack(object::plant& p);

public:
    void launch(
        object::plant& p,
        object::zombie *zombie,
        unsigned int row,
        bool is_alt_attack);

    void update();

    plant_system(object::scene& s) :
        scene(s),
        plant_factory(s),
        projectile_factory(s),
        damage(s),
        reanim(s),
        rng(s),
        subsystems(s) {}
};

}
