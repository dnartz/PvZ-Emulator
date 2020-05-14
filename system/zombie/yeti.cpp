#include "zombie.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_yeti::update(object::zombie& z) {
    if (z.is_hypno ||
        !z.is_not_dying ||
        z.is_dead ||
        z.has_death_status() ||
        z.status != zombie_status::walking ||
        z.countdown.action)
    {
        return;
    }

    z.status = zombie_status::yeti_escape;
    z.has_item_or_walk_left = false;
    reanim.update_dx(z);
}

void zombie_yeti::init(zombie& z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::yeti, row);

    z.hp = 1350;
    z.countdown.action = rng.randint(501) + 1500;
    z.has_item_or_walk_left = true;

    z.attack_box.x = 20;
    z.attack_box.y = 0;
    z.attack_box.width = 50;
    z.attack_box.height = 115;

    set_common_fields(z);
}

}