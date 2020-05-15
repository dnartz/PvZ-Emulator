#include "plant.h"
#include "system/plant/plant_factory.h"
#include "system/griditem_factory.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_grave_buster::update(plant& p) {
    if (p.status == plant_status::grave_buster_land) {
        if (p.reanim.n_repeated > 0) {
            p.set_reanim(plant_reanim_name::anim_idle, reanim_type::repeat, 12);
            p.countdown.status = 400;
            p.status = plant_status::grave_buster_idle;
        }
    } else if (p.status == plant_status::grave_buster_idle &&
        p.countdown.status == 0)
    {
        for (auto& item : scene.griditems) {
            if (item.col == p.col &&
                item.row == p.row &&
                item.type == griditem_type::grave)
            {
                griditem_factory(scene).destroy(item);
                break;
            }
        }
        
        plant_factory(scene).destroy(p);
    }
}

void plant_grave_buster::init(plant & p, unsigned int row, unsigned int col) {
    plant_base::init(p, plant_type::grave_buster, row, col);
    p.set_reanim_frame(plant_reanim_name::anim_land);
    p.reanim.type = reanim_type::once;
    p.status = plant_status::grave_buster_land;
}

}