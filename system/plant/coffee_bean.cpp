#include "plant.h"
#include "system/plant/plant_factory.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_coffee_bean::update(object::plant& p) {
    if (p.status == plant_status::work && p.reanim.n_repeated > 0) {
        plant_factory(scene).destroy(p);
    }
}

void plant_coffee_bean::init(plant & p, unsigned int row, unsigned int col) {
    plant_base::init(p, plant_type::coffee_bean, row, col);
    p.countdown.effect = 100;
}

}