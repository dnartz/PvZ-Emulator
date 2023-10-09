#include "zombie.h"
#include "system/util.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_pogo::remove_stick(zombie& z) {
    if (!z.has_item_or_walk_left) {
        return;
    }

    z.action = zombie_action::falling;
    z.status = zombie_status::walking;

    reanim.update_status(z);

    z.hit_box.x = 36;
    z.hit_box.y = 17;
    z.hit_box.width = 42;
    z.hit_box.height = 115;

    z.attack_box.x = 20;
    z.attack_box.y = 17;
    z.attack_box.width = 50;
    z.attack_box.height = 115;

    z.accessory_2.type = zombie_accessories_type_2::none;
    z.accessory_2.hp = 0;

    z.has_item_or_walk_left = false;
}

void zombie_pogo::update(object::zombie& z) {
    if (z.is_dead ||
        z.has_death_status() ||
        z.countdown.freeze > 0 ||
        z.countdown.butter > 0 ||
        z.status < zombie_status::pogo_with_stick ||
        static_cast<int>(z.status) > 28 ||
        static_cast<int>(z.action) == 8)
    {
        return;
    }

    int b = 40;

    if (z.status < zombie_status::pogo_idle_before_target ||
        static_cast<int>(z.status) > 26)
    {
        if (z.status == zombie_status::pogo_jump_across) {
            b = 90;
        } else if (static_cast<int>(z.status) == 28) {
            b = 170;
        }
    } else {
        b = (static_cast<int>(z.status) -
            static_cast<int>(zombie_status::pogo_idle_before_target)) * 20 + 50;
    }

    float dy = 1 - fabs(1.0f - 2.0f * ((80.0f - z.countdown.action) / 80.0f));
    dy = 2 * dy - dy * dy;
    dy = b * dy + 9;

    z.dy = dy;

    if (z.countdown.action == 7) {
        z.reanim.progress = 0;
        z.reanim.type = reanim_type::once;
    }

    if (z.status == zombie_status::pogo_jump_across &&
        z.countdown.action == 70)
    {
        auto target = find_target(z, zombie_attack_type::jump);

        if (target && target->type == plant_type::tallnut) {
            remove_stick(z);
            return;
        }
    }

    if (z.countdown.action == 0) {
        if (auto target = find_target(z, zombie_attack_type::jump)) {
            if (z.status == zombie_status::pogo_idle_before_target) {
                z.status = zombie_status::pogo_jump_across;
                z.countdown.action = 80;
                z.dx = (z.int_x - target->x + 60) / 80.0;
            } else {
                z.status = zombie_status::pogo_idle_before_target;
                z.dx = 0;
                z.countdown.action = 80;
            }
        } else {
            z.status = zombie_status::pogo_with_stick;
            reanim.update_dx(z);
            z.countdown.action = 80;
        }
    }
}

void zombie_pogo::init(zombie &z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::pogo, row);

    z.hp = 500;

    z.status = zombie_status::pogo_with_stick;
    z.countdown.action = rng.randint(80) + 1;

    z.attack_box.x = 10;
    z.attack_box.y = 0;
    z.attack_box.width = 30;
    z.attack_box.height = 115;

    z.has_item_or_walk_left = true;

    reanim.set(z, zombie_reanim_name::anim_pogo, reanim_type::once, 40);
    z.reanim.progress = 1;

    set_common_fields(z);
}

}
