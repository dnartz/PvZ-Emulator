#include "zombie.h"

namespace pvz_emulator::system {

using namespace::pvz_emulator::object;

void zombie_imp::update(object::zombie& z) {
    if (z.status == zombie_status::imp_flying) {
        z.d2y -= 0.05000000074505806;
        z.dy += z.d2y;

        z.x -= z.dx;

        auto new_y = zombie_init_y(scene.type, z, z.row);
        auto new_dy = z.dy + new_y - z.y;

        z.y = new_y;
        z.dy = new_dy;

        if (z.dy <= 0) {
            z.dy = 0;
            z.status = zombie_status::imp_landing;
            reanim.set(z, zombie_reanim_name::anim_land, reanim_type::once, 24);
        }
    } else if (z.status == zombie_status::imp_landing &&
        z.reanim.n_repeated > 0)
    {
        z.status = zombie_status::walking;
        reanim.update_status(z);
    }
}

void zombie_imp::init(zombie &z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::imp, row);

    reanim.set(z, zombie_reanim_name::anim_walk, reanim_type::repeat, 12);

    set_common_fields(z);
}

}