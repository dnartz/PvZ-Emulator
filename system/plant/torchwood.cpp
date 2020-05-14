#include "plant.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_torchwood::update(plant & p) {
    rect pr;
    p.get_attack_box(pr);

    for (auto& proj : scene.projectiles) {
        if (proj.type != projectile_type::pea &&
            proj.type != projectile_type::snow_pea ||
            proj.row != p.row)
        {
            continue;
        }

        rect pjr;
        proj.get_attack_box(pjr);

        if (pr.get_overlap_len(pjr) >= 10 && proj.last_torchwood_col != p.col) {
            if (proj.type == projectile_type::pea) {
                proj.type = projectile_type::fire_pea;
            } else {
                proj.type = projectile_type::pea;
            }

            proj.last_torchwood_col = p.col;
        }
    }
}

}