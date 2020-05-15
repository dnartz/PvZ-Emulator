#include "zombie.h"
#include "system/damage.h"
#include "system/plant/plant_factory.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_jack_in_the_box::kill_plants(zombie& z, int x, int y) {
    for (auto& p : scene.plants) {
        rect rect;
        p.get_hit_box(rect);
        
        if (rect.is_intersect_with_circle(x, y, 90)) {
            plant_factory(scene).destroy(p);
        }
    }
}

void zombie_jack_in_the_box::update(zombie &z) {
    switch (z.status) {
    case zombie_status::jackbox_walking:
        if (z.countdown.action <= 0 && z.is_not_dying) {
            z.countdown.action = 110;
            z.status = zombie_status::jackbox_pop;
            reanim.set(z, zombie_reanim_name::anim_pop, reanim_type::once, 28);
        }
        break;

    case zombie_status::jackbox_pop: {
        int x = z.int_x + z.hit_box.offset_x / 2;
        int y = z.int_y + z.hit_box.offset_y / 2;

        if (z.countdown.action <= 0) {
            if (z.is_hypno) {
                damage(scene).take_instant_kill(
                    z.row,
                    x,
                    y,
                    1,
                    115,
                    true,
                    attack_flags::digging_digger |
                    attack_flags::dying_zombies |
                    attack_flags::animating_zombies |
                    attack_flags::lurking_snorkel |
                    attack_flags::flying_balloon |
                    attack_flags::ground |
                    0x8);
            }
            else {
                damage(scene).take_instant_kill(
                    z.row,
                    x,
                    y,
                    1,
                    115,
                    true,
                    attack_flags::hypno_zombies |
                    attack_flags::digging_digger |
                    attack_flags::dying_zombies |
                    attack_flags::animating_zombies |
                    attack_flags::lurking_snorkel |
                    attack_flags::flying_balloon |
                    attack_flags::ground |
                    0x8);

                kill_plants(z, x, y);
            }

            zombie_factory(scene).destroy(z);
        }
        break;
    }
    default:
        break;
    }
}

void zombie_jack_in_the_box::init(zombie& z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::jack_in_the_box, row);

    z.hp = 500;
    z.status = zombie_status::jackbox_walking;

    z.garlic_tick.a = 6;

    z.countdown.action = rng.randint(300) + 450;
    if (rng.randint(20) == 0) {
        z.countdown.action /= 3;
    }
    z.countdown.action = 2 * static_cast<unsigned int>(z.countdown.action / z.dx);

    z.attack_box.x = 20;
    z.attack_box.y = 0;
    z.attack_box.width = 50;
    z.attack_box.height = 115;

    set_common_fields(z);
}

}
