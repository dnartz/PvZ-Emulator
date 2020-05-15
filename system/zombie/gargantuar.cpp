#include <cassert>
#include "zombie.h"
#include "system/damage.h"
#include "system/reanim.h"
#include "system/plant/plant.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_gargantuar::set_walk(zombie& z) {
    if (z.reanim.n_repeated > 0) {
        z.status = zombie_status::walking;
        reanim.update_status(z);
    }
}

void zombie_gargantuar::update(zombie& z) {
    switch (z.status) {
    case zombie_status::gargantuar_smash: {
        if (!z.reanim.is_in_progress(0.63999999)) {
            set_walk(z);
            return;
        }

        if (auto target = find_target(z, zombie_attack_type::smash_or_eat)) {
            if (target->type == plant_type::spikerock) {
                damage(scene).take(z, 20,
                    static_cast<unsigned int >(zombie_damage_flags::spike));

                plant_spikerock(scene).reduce_life(*target);

                if (target->hp > 0) {
                    set_walk(z);
                    return;
                }
            }

            for (auto& p : scene.plants) {
                if (p.type != plant_type::spikerock &&
                    p.row == target->row &&
                    p.col == target->col)
                {
                    damage(scene).set_smashed(p);
                }
            }
        }

        return;
    }

    case zombie_status::gargantuar_throw: {
        if (!z.reanim.is_in_progress(0.74000001)) {
            if (z.reanim.n_repeated > 0) {
                z.status = zombie_status::walking;
                reanim.update_status(z);
            }

            return;
        }

        double d2y = static_cast<double>(z.x) - 360;
        double min_d2y = 40;

        if (scene.type == scene_type::roof ||
            scene.type == scene_type::moon_night)
        {
            d2y -= 180;
        }

        if (min_d2y <= d2y) {
            if (d2y > 140) {
                d2y -= rng.randfloat(0, 100);
            }
        } else {
            d2y = min_d2y;
        }

        z.has_item_or_walk_left = false;
        auto& imp = zombie_factory(scene).create(zombie_type::imp);

        imp.row = z.row;
        imp.status = zombie_status::imp_flying;

        imp.x = z.x - 133.0f;
        imp.y = zombie_init_y(scene.type, z, z.row);

        imp.dx = 3.0f;
        imp.dy = 88.0f;
        imp.d2y = static_cast<float>(d2y / 3 * 0.5 * 0.05000000074505806);

        reanim.set(imp, zombie_reanim_name::anim_thrown, reanim_type::once, 18);
        reanim.update_progress(imp.reanim);

        set_walk(z);
        return;
    }

    default:
        if (z.countdown.freeze > 0 ||
            z.countdown.butter > 0 ||
            !z.is_not_dying)
        {
            return;
        }

        if (z.has_item_or_walk_left &&
            z.hp < static_cast<int>(z.max_hp) / 2 &&
            z.x > 400)
        {
            z.status = zombie_status::gargantuar_throw;
            reanim.set(z, zombie_reanim_name::anim_throw, reanim_type::once, 24);
            return;
        }

        if (find_target(z, zombie_attack_type::smash_or_eat)) {
            z.status = zombie_status::gargantuar_smash;
            reanim.set(z, zombie_reanim_name::anim_smash, reanim_type::once, 16);
        }
    }
}

void zombie_gargantuar::init(zombie& z, zombie_type type, unsigned int row) {
    assert(type == zombie_type::gargantuar ||
        type == zombie_type::giga_gargantuar);

    z._ground = _ground.data();

    zombie_base::init(z, type, row);

    z.hp = z.type == zombie_type::gargantuar ? 3000 : 6000;

    z.x = static_cast<float>(rng.randint(10) + 845);

    z.hit_box.x = -17;
    z.hit_box.y = -38;
    z.hit_box.width = 125;
    z.hit_box.height = 154;
    z.hit_box.offset_x = 180;
    z.hit_box.offset_y = 180;

    z.attack_box.x = -30;
    z.attack_box.y = -38;
    z.attack_box.width = 89;
    z.attack_box.height = 154;

    z.garlic_tick.a = 8;
    z.garlic_tick.b = 24;

    z.has_item_or_walk_left = true;

    set_common_fields(z);
}

}