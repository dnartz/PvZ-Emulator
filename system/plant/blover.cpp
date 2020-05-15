#include "plant.h"
#include "system/damage.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_blover::update(object::plant& p) {
    if (p.reanim.n_repeated > 0 && p.reanim.type != reanim_type::repeat) {
        p.set_reanim_frame(plant_reanim_name::anim_loop);
        p.reanim.type = reanim_type::repeat;
    }

    if (p.status != plant_status::work && p.countdown.effect == 0) {
        damage(scene).activate_plant(p);
    }
}

void plant_blover::init(plant & p, unsigned int row, unsigned int col) {
    plant_base::init(p, plant_type::blover, row, col);

    p.countdown.effect = 50;
    p.set_reanim_frame(plant_reanim_name::anim_idle);
    p.reanim.fps = 10;
}

}