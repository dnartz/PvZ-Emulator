#include "plant.h"
#include "system/damage.h"
#include "system/plant/plant_factory.h"
#include "system/zombie/zombie_factory.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_tangle_kelp::update(plant & p) {
    if (p.status == plant_status::tangle_kelp_grab) {
        if (p.countdown.status == 50 && p.target != -1) {
            if (auto z = scene.zombies.get(p.target)) {
                z->action = zombie_action::caught_by_kelp;
                damage(scene).unset_is_eating(*z);
            }
        } else if (p.countdown.status == 0) {
            plant_factory(scene).destroy(p);

            if (p.target != -1) {
                if (auto z = scene.zombies.get(p.target)) {
                    zombie_factory(scene).destroy(*z);
                }
            }
        }
    } else if (auto target = find_target(p, p.row, false)) {
        p.status = plant_status::tangle_kelp_grab;
        p.countdown.status = 100;
        p.target = scene.zombies.get_index(*target);
    }
}

}