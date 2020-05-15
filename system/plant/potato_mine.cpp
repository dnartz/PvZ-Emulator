#include "plant.h"
#include "system/damage.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_potato_mine::update(plant& p) {
    switch (p.status) {
    case plant_status::potato_sprout_out:
        if (p.reanim.n_repeated > 0) {
            p.set_reanim(plant_reanim_name::anim_armed, reanim_type::repeat, rng.randfloat(12, 15));
            p.status = plant_status::potato_armed;
        }
        break;

    case plant_status::potato_armed:
        if (find_target(p, p.row, false)) {
            damage(scene).activate_plant(p);
        }
        break;

    case plant_status::idle:
        if (p.countdown.status == 0) {
            p.set_reanim(plant_reanim_name::anim_rise, reanim_type::once, 18);
            p.status = plant_status::potato_sprout_out;
        }

    default:
        break;
    }
}

void plant_potato_mine::init(plant & p, unsigned int row, unsigned int col) {
    plant_base::init(p, plant_type::potato_mine, row, col);
    p.reanim.fps = 12;
    p.countdown.status = 1500;
}

}