#include <cassert>
#include "plant.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void pea_base::init(plant & p, plant_type type, unsigned int row, unsigned int col) {
    plant_base::init(p, type, row, col);
    p.reanim.fps = rng.randfloat(15, 20);
}

void plant_threepeater::set_launch_countdown(plant & p) {
    if (find_target(p, p.row, false) ||
        p.row - 1 >= 0 && find_target(p, p.row - 1, false) ||
        p.row + 1 < scene.get_max_row() && find_target(p, p.row + 1, false))
    {
        p.countdown.launch = 35;
    } else {
        p.threepeater_time_since_first_shot = 0;
    }
}

}