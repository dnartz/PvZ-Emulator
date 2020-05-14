#include "zombie.h"
#include "system/util.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_pole_vaulting::update(zombie &z) {
    if (z.status != zombie_status::pole_valuting_running ||
        !z.is_not_dying ||
        z.action != zombie_action::none)
    {
        if (z.status != zombie_status::pole_valuting_jumpping) {
            return;
        }

        bool has_jumped = false;

        if (z.reanim.progress > 0.60000002 && z.reanim.progress <= 0.69999999) {
            auto plant = find_target(z, zombie_attack_type::jump);
            
            if (plant && plant->type == plant_type::tallnut) {
                has_jumped = true;

                z.x = static_cast<float>(plant->x);
                z.y -= 30;
                z.action = zombie_action::falling;
            }
        } 
        
        if (z.reanim.n_repeated > 0) {
            has_jumped = true;
            z.x -= 150;
        }

        if (has_jumped) {
            z.int_x = static_cast<int>(z.x);
            z.attack_box.x = 50;
            z.attack_box.y = 0;
            z.attack_box.width = 20;
            z.attack_box.height = 115;
            z.status = zombie_status::pole_vaulting_walking;
            reanim.update_status(z);
        } else {
            auto t = z.x;
            z.x -= z.reanim.progress * 150;
            z.y = zombie_init_y(scene.type, z, z.row);
            z.x = t;
        }
    } else {
        auto plant = find_target(z, zombie_attack_type::jump);

        if (plant == nullptr) {
            return;
        }

        for (auto& item : scene.griditems) {
            if (item.col == plant->col &&
                item.row == plant->row &&
                item.type == griditem_type::ladder)
            {
                if (z.x < get_x_by_col(plant->col) + 40 &&
                    z.action == zombie_action::none &&
                    z.ladder_col != plant->col)
                {
                    z.ladder_col = plant->col;
                    z.action = zombie_action::climbing_ladder;
                }
                return;
            }
        }

        z.status = zombie_status::pole_valuting_jumpping;
        reanim.set(z, "anim_jump", reanim_type::once, 24);
        z.has_item_or_walk_left = false;
        z.dx = static_cast<double>(z.int_x - plant->x - 80) /
            (z.reanim.n_frames / z.reanim.fps * 100);
    }
}

void zombie_pole_vaulting::init(zombie &z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::pole_vaulting, row);

    z.x = static_cast<float>(rng.randint(10) + 870);

    z.garlic_tick.a = 6;

    z.hp = 500;
    z.status = zombie_status::pole_valuting_running;
    z.has_item_or_walk_left = true;

    z.attack_box.x = -29;
    z.attack_box.y = 0;
    z.attack_box.width = 70;
    z.attack_box.height = 115;

    reanim.set(z, "anim_run", reanim_type::repeat, 0);
    reanim.update_dx(z);

    set_common_fields(z);
}

}