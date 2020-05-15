#include "plant.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_umbrella_leaf::update(plant& p) {
    if (p.status == plant_status::umbrella_leaf_block) {
        if (p.countdown.status == 0) {
            p.status = plant_status::umbrella_leaf_shrink;
        }
    }
    else if (p.status == plant_status::umbrella_leaf_shrink) {
        if (p.reanim.n_repeated > 0) {
            p.set_reanim(plant_reanim_name::anim_idle, reanim_type::repeat, 12);
            p.status = plant_status::idle;
        }
    }
}

}