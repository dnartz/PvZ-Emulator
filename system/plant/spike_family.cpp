#include "plant.h"
#include "system/damage.h"
#include "system/plant/plant_factory.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_spikeweed::attack(plant& p) {
    damage(scene).range_attack(p,
        zombie_damage_flags::spike | zombie_damage_flags::ignore_accessory_2);
    set_idle_reanim(p);
}

void plant_spikeweed::set_idle_reanim(plant& p) {
    if (p.reanim.n_repeated > 0) {
        p.set_reanim(
            plant_reanim_name::anim_idle,
            reanim_type::repeat,
            rng.randfloat(12, 15));
    }
}

void plant_spikeweed::update(plant& p) {
    if (p.status == plant_status::spike_attack) {
        if (p.countdown.status == 0) {
            p.status = plant_status::idle;
            set_idle_reanim(p);
            return;
        }

        int t;
        if (p.type == plant_type::spikerock) {
            if (p.countdown.status == 70) {
                attack(p);
                return;
            }

            t = 32;
        }
        else {
            t = 75;
        }

        if (p.countdown.status == t) {
            attack(p);
        }
        else {
            set_idle_reanim(p);
        }

        return;
    } else if (find_target(p, p.row, false)) {
        p.set_reanim(plant_reanim_name::anim_attack, reanim_type::once, 18);
        p.status = plant_status::spike_attack;
        p.countdown.status = 100;
    }
}

void plant_spikerock::reduce_life(plant &p) {
    if (p.status != plant_status::spike_attack) {
        p.status = plant_status::spike_attack;
        p.countdown.status = 100;
    }

    p.hp -= 50;

    if (p.hp <= 0) {
        plant_factory(scene).destroy(p);
    }
}

}
