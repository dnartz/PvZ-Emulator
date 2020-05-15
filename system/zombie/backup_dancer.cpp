#include <cmath>
#include "zombie.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_backup_dancer::update(zombie& z) {
    if (z.is_eating) {
        return;
    }

    if (z.status == zombie_status::backup_spawning) {
        z.dy = static_cast<float>(round(z.countdown.action * (-4/3)));

        if (z.countdown.action) {
            return;
        }
    }

    auto next_status = get_status_by_clock(z);
    if (next_status != z.status) {
        z.status = next_status;

        switch (next_status) {
        case zombie_status::dancing_walking:
            reanim.set(z, zombie_reanim_name::anim_walk, reanim_type::repeat, 0);
            break;

        case zombie_status::dancing_armrise1:
            reanim.set(z, zombie_reanim_name::anim_armraise, reanim_type::repeat, 18);
            z.reanim.progress = 0.60000002f;
            break;

        default:
            reanim.set(z, zombie_reanim_name::anim_armraise, reanim_type::repeat, 18);
            break;
        }
    }
}

void zombie_backup_dancer::init(zombie& z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::backup_dancer, row);

    z.status = zombie_status::dancing_walking;

    reanim.set(z, zombie_reanim_name::anim_armraise, reanim_type::repeat, 12);

    set_common_fields(z);
}

}