#pragma once
#include "object/scene.h"
#include "system/debuff.h"
#include "system/damage.h"
#include "system/zombie/zombie.h"
#include "system/projectile/projectile_factory.h"

namespace pvz_emulator::system {

class projectile_system {
private:
    object::scene& scene;
    system::projectile_factory projectile_factory;
    system::damage damage;
    system::debuff debuff;
    system::zombie_base zombie_base;

    bool is_in_torchwood(object::projectile& proj);

    object::zombie* find_zombie_target(object::projectile& proj);
    object::plant* find_plant_target(object::projectile& proj);

    void attack_target_zombie(object::projectile& proj, object::zombie* z);

    bool is_covered_by_suppter(
        const object::projectile& proj,
        const object::zombie& z);

    void suppter_attack(object::projectile& proj, object::zombie* main_target);

    void parabola_do_attack(object::projectile& proj, object::zombie* z);

    void do_parabola_motion(object::projectile& proj);

    void others_do_attack(object::projectile& proj);
    void roof_set_disappear(object::projectile& proj);

    int get_star_row_by_x_and_y(int x, int y) {
        return std::max(0, get_row_by_x_and_y(scene.type, std::max(40, x), y));
    }

    void do_other_motion(object::projectile& proj);
public:
    void update();
    projectile_system(object::scene &s) :
        scene(s),
        projectile_factory(s),
        damage(s),
        debuff(s),
        zombie_base(s) {}
};

}
