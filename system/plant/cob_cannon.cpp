#include "plant.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_cob_cannon::update(plant& p) {
    switch (p.status) {
    case plant_status::cob_cannon_unaramed_idle:
        if (p.countdown.status == 0) {
            p.status = plant_status::cob_cannon_charge;
            p.set_reanim("anim_charge", reanim_type::once, 12);
        }
        break;

    case plant_status::cob_cannon_charge:
        if (p.reanim.n_repeated > 0) {
            p.status = plant_status::cob_cannon_aramed_idle;
            p.set_reanim("anim_idle", reanim_type::repeat, 12);
        }
        break;

    default:
        break;
    }
}

bool plant_cob_cannon::launch(plant& p, int x, int y) {
    if (p.status != plant_status::cob_cannon_aramed_idle) {
        return false;
    }

    p.status = plant_status::cob_cannon_launch;
    p.countdown.launch = 206;
    p.set_reanim("anim_shooting", reanim_type::once, 12);

    p.cannon.x = static_cast<int>(x - 47.0);
    p.cannon.y = y;

    return true;
}

void plant_cob_cannon::init(plant& p, int row, int col, unsigned int countdown) {
    plant_base::init(p, plant_type::cob_cannon, row, col);

    p.status = plant_status::cob_cannon_unaramed_idle;
    p.countdown.status = 500;
    p.set_reanim_frame("anim_unarmed_idle");
}

}