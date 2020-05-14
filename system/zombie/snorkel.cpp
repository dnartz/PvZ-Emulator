#include <cassert>
#include "zombie.h"
#include "system/damage.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_snorkel::update(zombie& z) {
    switch (z.status) {
    case zombie_status::snorkel_walking:
        if (!z.is_walk_right()) {
            if (z.int_x > 700 && z.int_x <= 720) {
                z.dx = 0.2f;
                z.status = zombie_status::snorkel_jump_in_the_pool;

                reanim.set(z, "anim_jumpinpool", reanim_type::once, 16);
            }
            return;
        }

    case zombie_status::snorkel_jump_in_the_pool: {
        float t = static_cast<float>(
            static_cast<int>(z.reanim.progress * 1000) / 1000.f);

        if (t > 0) {
            if (t < 1) {
                z.dy = t * 10;
            } else {
                z.dy = 10;
            }
        } else {
            z.dy = 0;
        }

        if (z.reanim.n_repeated > 0) {
            z.status = zombie_status::snorkel_swim;
            z.is_in_water = true;
            reanim.set(z, "anim_swim", reanim_type::repeat, 12);
        }

        return;
    }

    case zombie_status::snorkel_swim:
        if (!z.is_not_dying) {
            damage(scene).take(
                z,
                1800,
                zombie_damage_flags::ignore_accessory_2 |
                zombie_damage_flags::no_flash);

            return;
        }

        if (z.int_x <= 25 && !z.is_walk_right()) {
            z.dy = -90.0f;
            z.x -= 15.0f;
            z.status = zombie_status::snorkel_walking;
            z.action = zombie_action::leaving_pool;
            reanim.update_status(z);

            return;
        }

        if (z.int_x > 640 && z.is_walk_right()) {
            z.dy = -90.0f;
            z.x += 15.0f;
            z.status = zombie_status::snorkel_walking;
            z.action = zombie_action::leaving_pool;
            reanim.update_status(z);

            return;
        }

        if (z.is_eating) {
            z.status = zombie_status::snorkel_up_to_eat;
            reanim.set(z, "anim_uptoeat", reanim_type::once, 24);
        }

        return;

    case zombie_status::snorkel_up_to_eat:
        if (z.is_eating) {
            if (z.reanim.n_repeated > 0) {
                z.status = zombie_status::snorkel_eat;
                reanim.set(z, "anim_eat", reanim_type::repeat, 0);
            }
        } else {
            z.status = zombie_status::snorkel_finied_eat;
            reanim.set(z, "anim_uptoeat", reanim_type::once, -24);
        }
        return;

    case zombie_status::snorkel_eat:
        if (!z.is_eating) {
            z.status = zombie_status::snorkel_finied_eat;
            reanim.set(z, "anim_uptoeat", reanim_type::once, -24);
        }
        return;

    case zombie_status::snorkel_finied_eat:
        if (z.reanim.n_repeated > 0) {
            z.status = zombie_status::snorkel_swim;
            reanim.set(z, "anim_swim", reanim_type::once, 0);
            reanim.update_dx(z);
        }
        return;

    default:
        assert(false);
        break;
    }
}

void zombie_snorkel::init(zombie& z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::snorkel, row);

    z.hit_box.x = 12;
    z.hit_box.y = 0;
    z.hit_box.width = 62;
    z.hit_box.height = 115;

    z.attack_box.x = -5;
    z.attack_box.y = 0;
    z.attack_box.width = 55;
    z.attack_box.height = 115;

    z.status = zombie_status::snorkel_walking;

    set_common_fields(z);
}

}
