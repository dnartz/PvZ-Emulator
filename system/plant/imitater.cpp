#include "plant.h"
#include "system/plant/plant_factory.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_imitater::update(plant& p) {
    if (p.status == plant_status::imitater_explode) {
        if (p.reanim.n_repeated > 0) {
            plant_factory plant_factory(scene);
            plant_factory.destroy(p);
            plant_factory.create(p.imitater_target, p.row, p.col);
        }
    } else if (p.countdown.status == 0) {
        p.status = plant_status::imitater_explode;
        p.set_reanim(plant_reanim_name::anim_explode, reanim_type::once, 26);
    }
}

void plant_imitater::init(
    plant & p,
    plant_type target,
    unsigned int row,
    unsigned int col)
{
    plant_base::init(p, plant_type::imitater, row, col, target);
    p.reanim.fps = rng.randfloat(25, 30);
    p.countdown.effect = 200;
}

}