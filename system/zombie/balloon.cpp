#include "system/zombie/zombie.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_balloon::update(zombie& z) {
    if (z.status == zombie_status::balloon_falling &&
        z.reanim.n_repeated > 0)
    {
        z.status = zombie_status::balloon_walking;
        reanim.update_status(z);
    }
}

void zombie_balloon::init(zombie &z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::balloon, row);

    z.dy = 25;
    z.status = zombie_status::balloon_flying;

    reanim.set(z, zombie_reanim_name::anim_idle, reanim_type::repeat, z.reanim.fps);
    
    z.hit_box.x = 36;
    z.hit_box.y = 30;
    z.hit_box.width = 42;
    z.hit_box.height = 115;

    z.attack_box.x = 20;
    z.attack_box.y = 30;
    z.attack_box.width = 50;
    z.attack_box.height = 115;

    z.has_balloon = true;

    set_common_fields(z);
}

}
