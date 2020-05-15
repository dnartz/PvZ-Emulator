#include "plant.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_cactus::update(plant& p) {
    if (p.countdown.launch > 0) {
        return;
    }

    switch (p.status) {
    case plant_status::cactus_grow_tall:
        if (p.reanim.n_repeated > 0) {
            p.status = plant_status::cactus_tall_idle;
            p.set_reanim(plant_reanim_name::anim_idlehigh, reanim_type::repeat, 0);
            p.countdown.generate = 1;
        }
        break;

    case plant_status::cactus_tall_idle:
        if (!find_target(p, p.row, false)) {
            p.status = plant_status::cactus_get_short;
            p.set_reanim(plant_reanim_name::anim_lower, reanim_type::once, 12);
        }
        break;

    case plant_status::cactus_get_short:
        if (p.reanim.n_repeated > 0) {
            p.status = plant_status::cactus_short_idle;
            p.set_reanim(plant_reanim_name::anim_idle, reanim_type::repeat, 0);
        }
        break;

    default:
        if (find_target(p, p.row, false)) {
            p.status = plant_status::cactus_grow_tall;
            p.set_reanim(plant_reanim_name::anim_rise, reanim_type::once, 12);
        }
    }
}

void plant_cactus::init(plant & p, unsigned int row, unsigned int col) {
    plant_base::init(p, plant_type::cactus, row, col);
    p.status = plant_status::cactus_short_idle;
}

}