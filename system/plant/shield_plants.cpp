#include "plant.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_tallnut::init(plant& p, unsigned int row, unsigned int col) {
    plant_base::init(p, plant_type::tallnut, row, col);
    p.attack_box.height = 80;
}

void plant_pumpkin::init(plant& p, unsigned int row, unsigned int col) {
    plant_base::init(p, plant_type::pumpkin, row, col);
    p.attack_box.width = 120;
}

}