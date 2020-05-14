#include "zombie.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_football::init(object::zombie &z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::football, row);

    z.hit_box.x = 50;
    z.hit_box.y = 0;
    z.hit_box.width = 57;
    z.hit_box.height = 115;

    z.accessory_1.type = zombie_accessories_type_1::football_cap;
    z.accessory_1.hp = 1400;

    z.garlic_tick.a = 6;

    set_common_fields(z);
}

}