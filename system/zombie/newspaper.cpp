#include "system/zombie/zombie.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_newspaper::update(zombie& z) {
    if (z.status == zombie_status::newspaper_destoryed &&
        z.reanim.n_repeated > 0)
    {
        z.status = zombie_status::newspaper_running;
        reanim.update_status(z);
    }
}

void zombie_newspaper::init(zombie& z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::newspaper, row);

    z.status = zombie_status::newspaper_walking;

    z.attack_box.x = 20;
    z.attack_box.y = 0;
    z.attack_box.width = 50;
    z.attack_box.height = 115;

    z.accessory_2.type = zombie_accessories_type_2::newspaper;
    z.accessory_2.hp = 150;

    set_common_fields(z);
}

}
