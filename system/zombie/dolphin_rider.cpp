#include "zombie.h"
#include "system/util.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_dolphin_rider::set_attrs_after_drop_dolphin(zombie& z, float dy) {
    z.dy = dy;

    z.status = zombie_status::dophin_walk_in_pool;

    z.attack_box.x = 30;
    z.attack_box.y = 0;
    z.attack_box.width = 30;
    z.attack_box.height = 115;
    
    z.hit_box.x = 20;
    z.hit_box.y = 0;
    z.hit_box.width = 42;
    z.hit_box.height = 115;

    reanim.update_status(z);
}

void zombie_dolphin_rider::update(zombie& z) {
    if (is_target_of_kelp(scene, z)) {
        return;
    }

    switch (z.status) {
    case zombie_status::dophin_walk_with_dophin:
        if (700 < z.int_x && z.int_x <= 720) {
            z.status = zombie_status::dophin_jump_in_pool;
            reanim.set(z, "anim_jumpinpool", reanim_type::once, 16);
        }
        return;

    case zombie_status::dophin_jump_in_pool:
        if (z.reanim.n_repeated > 0) {
            z.x -= 70;
            z.attack_box.x = -29;
            z.attack_box.width = 70;
            z.attack_box.height = 115;
            z.status = zombie_status::dophin_ride;
            z.is_in_water = true;

            reanim.set(z, "anim_ride", reanim_type::repeat, 12);
        }
        return;

    case zombie_status::dophin_ride:
        if (z.x > 10) {
            if (z.is_not_dying &&
                !is_target_of_kelp(scene, z) &&
                find_target(z, zombie_attack_type::jump))
            {
                z.dx = 0.5;
                z.status = zombie_status::dophin_jump;
                z.countdown.action = 120;
                
                reanim.set(z, "anim_dolphinjump", reanim_type::once, 10);
            }
        } else {
            z.dy -= 40;
            z.action = zombie_action::leaving_pool;
            z.status = zombie_status::dophin_walk_with_dophin;
            
            reanim.set(z, "anim_walkdolphin", reanim_type::repeat, 0);
            reanim.update_dx(z);
        }
        return;

    case zombie_status::dophin_jump:
        z.dy = 0;

        if (z.reanim.is_in_progress(0.30000001)) {
            auto target = find_target(z, zombie_attack_type::jump);

            if (target && target->type == plant_type::tallnut) {
                z.action = zombie_action::falling;

                z.x = static_cast<float>(target->x + 25);

                set_attrs_after_drop_dolphin(z, 30);
                return;
            }
        }

        if (z.reanim.is_in_progress(0.49000001)) {
            z.dx = 0;
            return;
        }

        if (z.reanim.n_repeated > 0) {
            z.x -= 94;
            set_attrs_after_drop_dolphin(z, 0);
        }

        return;

    case zombie_status::dophin_walk_in_pool:
        if (z.int_x <= 10 && !z.is_walk_right() ||
            z.int_x > 680 && z.is_walk_right()) 
        {
            z.action = zombie_action::leaving_pool;
            z.dy = -40;

            reanim.set(z, "anim_walk", reanim_type::repeat, 0);
            reanim.update_dx(z);
        }

    default:
        break;
    }
}

void zombie_dolphin_rider::init(zombie& z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::dolphin_rider, row);

    z.hp = 500;
    z.status = zombie_status::dophin_walk_with_dophin;
    z.garlic_tick.a = 6;

    reanim.set(z, "anim_walkdolphin", reanim_type::repeat, 0);
    reanim.update_dx(z);

    set_common_fields(z);
}

}
