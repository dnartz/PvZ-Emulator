#include <cmath>
#include <cassert>
#include "zombie.h"
#include "system/util.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_digger::update_dy(zombie& z) {
    if (z.countdown.action <= 40) {
        z.dy = static_cast<float>(20 - pow(30.0 - z.countdown.action, 2) / 45.0);
    } else {
        z.dy = static_cast<float>((-7.0 * pow(z.countdown.action, 2) +
            560.0 * z.countdown.action 
            - 3100) / 405.0);
    }

    z.dy = round(z.dy);
}

void zombie_digger::update(zombie& z) {
    switch (z.status) {
    case zombie_status::digger_dig:
        if (z.x >= 10) {
            return;
        }

        z.dy = -120.0f;
        z.status = zombie_status::digger_drill;
        z.countdown.action = 130;
        reanim.set(z, zombie_reanim_name::anim_drill, reanim_type::repeat, 20);
        return;

    case zombie_status::digger_drill:
        update_dy(z);

        if (z.countdown.action == 30) {
            reanim.set(z, zombie_reanim_name::anim_landing, reanim_type::once, 12);
        }
        else if (z.countdown.action == 0) {
            z.dy = 0;
            z.status = zombie_status::digger_dizzy;
            reanim.set(z, zombie_reanim_name::anim_dizzy, reanim_type::repeat, 12);
        }

        return;

    case zombie_status::digger_lost_dig:
        if (z.countdown.action == 0) {
            z.dy = -120.0f;
            z.status = zombie_status::digger_landing;
            z.countdown.action = 130;
            reanim.set(z, zombie_reanim_name::anim_landing, reanim_type::once, 0);
        }
        return;

    case zombie_status::digger_landing:
        update_dy(z);

        if (z.countdown.action == 30) {
            reanim.set(z, zombie_reanim_name::anim_landing, reanim_type::once, 12);
        } else if (z.countdown.action == 0) {
            z.dy = 0.0f;
            z.status = zombie_status::digger_walk_left;
            reanim.update_status(z);
        }

        return;

    case zombie_status::digger_dizzy:
        if (z.reanim.n_repeated > 1) {
            z.status = zombie_status::digger_walk_right;
            reanim.update_status(z);
        }

    default:
        break;
    }
}

void zombie_digger::init(object::zombie &z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::digger, row);

    z.hit_box.x = 50;
    z.hit_box.y = 0;
    z.hit_box.width = 28;
    z.hit_box.height = 115;

    z.status = zombie_status::digger_dig;

    reanim.set(z, zombie_reanim_name::anim_dig, reanim_type::once, 12);
    reanim.update_dx(z);

    z.accessory_1.type = zombie_accessories_type_1::miner_hat;
    z.accessory_1.hp = 100;
    z.has_item_or_walk_left = true;

    set_common_fields(z);
}

}