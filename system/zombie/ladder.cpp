#include "zombie.h"
#include "system/damage.h"
#include "system/griditem_factory.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_ladder::update(object::zombie& z) {
    if (z.is_hypno || !z.is_not_dying || z.is_dead || z.has_death_status()) {
        return;
    }

    if (z.status != zombie_status::ladder_walking ||
        z.action != zombie_action::none)
    {
        if (z.status == zombie_status::ladder_placing &&
            z.reanim.n_repeated > 0)
        {
            if (auto target = find_target(z, zombie_attack_type::place_ladder))
            {
                griditem_factory(scene).create(
                    griditem_type::ladder,
                    target->row,
                    target->col);

                z.action = zombie_action::climbing_ladder;
                z.ladder_col = target->col;

                damage(scene).destroy_accessory_2(z);
            } else {
                z.status = zombie_status::ladder_walking;
                reanim.update_status(z);
            }
        }
    } else if (find_target(z, zombie_attack_type::place_ladder)) {
        damage(scene).unset_is_eating(z);
        z.status = zombie_status::ladder_placing;
        reanim.set(z, zombie_reanim_name::anim_placeladder, reanim_type::once, 24);
    }
}

void zombie_ladder::init(zombie &z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::ladder, row);

    z.hp = 500;
    z.accessory_2.type = zombie_accessories_type_2::ladder;
    z.accessory_2.hp = 500;

    z.attack_box.x = 10;
    z.attack_box.y = 0;
    z.attack_box.width = 50;
    z.attack_box.height = 115;

    z.status = zombie_status::ladder_walking;
    reanim.update_status(z);

    set_common_fields(z);
}

}