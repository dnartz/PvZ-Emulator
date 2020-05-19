#include <cassert>
#include "zombie.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_catapult::init(zombie& z, unsigned int row) {
    z._ground = nullptr;

    zombie_base::init(z, zombie_type::catapult, row);

    z.hp = 850;
    z.x = static_cast<float>(rng.randint(10) + 825);

    reanim.set(z, zombie_reanim_name::anim_walk, reanim_type::repeat, 5.5);

    z.catapult_or_jackson.n_basketballs = 20;

    z.hit_box.x = 0;
    z.hit_box.y = -13;
    z.hit_box.width = 153;
    z.hit_box.height = 140;

    z.attack_box.x = 10;
    z.attack_box.y = -13;
    z.attack_box.width = 133;
    z.attack_box.height = 140;

    set_common_fields(z);
}

plant *zombie_catapult::find_target(zombie& z) {
    plant* target = nullptr;

    for (auto& p : scene.plants) {
        if (p.row != z.row ||
            p.is_squash_attacking() ||
            p.is_smashed ||
            p.edible == plant_edible_status::invisible_and_not_edible ||
            p.type == plant_type::spikeweed ||
            p.type == plant_type::spikerock ||
            (target && target->col <= p.col))
        {
            continue;
        }

        auto& status = scene.plant_map[p.row][p.col];

        if (status.coffee_bean) {
            target = status.coffee_bean;
        } else if (status.content) {
            target = status.content;
        } else if (status.pumpkin) {
            target = status.pumpkin;
        } else {
            target = status.base;
        }
    }

    return target;
}

void zombie_catapult::update(zombie &z) {
    switch (z.status) {
    case zombie_status::walking:
        if (z.x <= 650 &&
            find_target(z) &&
            z.catapult_or_jackson.n_basketballs > 0)
        {
            z.status = zombie_status::catapult_shoot;
            z.countdown.action = 300;
            reanim.set(z, zombie_reanim_name::anim_shoot, reanim_type::once, 24);
        }
        return;

    case zombie_status::catapult_idle:
        if (z.countdown.action) {
            return;
        }

        if (find_target(z)) {
            z.status = zombie_status::catapult_shoot;
            z.countdown.action = 300;
            reanim.set(z, zombie_reanim_name::anim_shoot, reanim_type::once, 24);
            return;
        }

        reanim.set(z, zombie_reanim_name::anim_walk, reanim_type::repeat, 6);
        z.status = zombie_status::walking;
        return;

    case zombie_status::catapult_shoot:
        if (z.reanim.is_in_progress(0.54500002)) {
            auto target = find_target(z);
            projectile_factory.create(projectile_type::basketball, z, target);
        }
        
        if (z.reanim.n_repeated > 0) {
            if (z.catapult_or_jackson.n_basketballs > 0) {
                --z.catapult_or_jackson.n_basketballs;
                reanim.set(z, zombie_reanim_name::anim_idle, reanim_type::once, 12);
                z.status = zombie_status::catapult_idle;
            } else {
                reanim.set(z, zombie_reanim_name::anim_walk, reanim_type::repeat, 6);
                z.status = zombie_status::walking;
            }
        }

        break;

    default:
        assert(false);
        break;
    }
}

}