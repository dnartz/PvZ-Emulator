#include <algorithm>

#include "debuff.h"
#include "system/zombie/zombie.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void debuff::set_slowed(object::zombie& z, unsigned int countdown) {
    if (z.can_be_slowed()) {
        z.countdown.slow = std::max(z.countdown.slow, countdown);
    }

    reanim.update_fps(z);
}

void debuff::set_butter(object::zombie& z) {
    if (z.is_not_dying &&
        z.can_be_freezed() &&
        z.type != zombie_type::zomboni &&
        !is_target_of_kelp(scene, z) &&
        !z.is_flying_or_falling())
    {
        z.countdown.butter = 400;

        if (z.master_id != -1) {
            if (auto master = scene.zombies.get(z.master_id)) {
                master->master_id = -1;
            }

            z.master_id = -1;
        }

        if (z.type == zombie_type::pogo) {
            z.dy = 0;
        }

        reanim.update_fps(z);
    }
}

void debuff::remove_by_fire_pea(zombie& z) {
    if (z.countdown.freeze > 0) {
        remove_freeze(z);
    }

    if (z.countdown.slow > 0) {
        remove_slow(z);
    }
}

}
