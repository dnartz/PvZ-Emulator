#include <cmath>
#include <algorithm>
#include "plant.h"
#include "system/sun.h"
#include "system/damage.h"
#include "system/plant/plant_system.h"
#include "system/util.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_base::init(
    plant& p,
    plant_type type,
    int row,
    int col,
    plant_type imitater_target)
{
    p.type = type;
    p.imitater_target = imitater_target;

    p.row = row;
    p.col = col;

    p.x = 80 * col + 40;
    p.y = get_y_by_row_and_col(scene.type, row, col);

    p.cannon.x = -1;
    p.cannon.y = -1;

    switch (type) {
    case plant_type::wallnut:
    case plant_type::pumpkin:
        p.hp = 4000;
        break;

    case plant_type::tallnut:
        p.hp = 8000;
        break;

    case plant_type::garlic:
        p.hp = 400;
        break;

    case plant_type::spikerock:
        p.hp = 450;
        break;

    default:
        p.hp = 300;
    }
    p.max_hp = p.hp;

    p.status = plant_status::idle;

    p.is_dead = false;
    p.is_smashed = false;
    p.is_sleeping = false;
    p.can_attack = plant::CAN_ATTACK_TABLE[static_cast<size_t>(p.type)];

    p.threepeater_time_since_first_shot = 0;

    p.split_pea_attack_flags.front = false;
    p.split_pea_attack_flags.back = false;

    p.edible = plant_edible_status::visible_and_edible;

    memset(&p.countdown, 0, sizeof(p.countdown));
    p.countdown.blover_disappear = 200;

    p.max_boot_delay =
        plant::EFFECT_INTERVAL_TABLE[static_cast<size_t>(p.type)];

    p.direction = plant_direction::right;

    p.attack_box.width = 80;
    p.attack_box.height = 80;

    p.reanim.type = reanim_type::repeat;
    p.reanim.fps = rng.randfloat(10, 15);

    p.target = -1;
    
    memset(&p.reanim, 0, sizeof(p.reanim));

    p.init_reanim();

    p.reanim.type = reanim_type::repeat;
    p.reanim.fps = rng.randfloat(5, 15);

    if (p.has_reanim(plant_reanim_name::anim_idle)) {
        p.set_reanim_frame(plant_reanim_name::anim_idle);
    }

    if (p.max_boot_delay <= 0) {
        p.countdown.generate = 0;
    } else if (p.type == plant_type::sunflower ||
        p.type == plant_type::twin_sunflower ||
        p.type == plant_type::sunshroom)
    {
        p.countdown.generate = rng.randint(p.max_boot_delay / 2 - 299) + 300;
    } else {
        p.countdown.generate = rng.randint(p.max_boot_delay + 1);
    }
}

zombie* plant_base::find_target(plant& p, unsigned int row, bool is_alt_attack) {
    auto flags = p.get_attack_flags(is_alt_attack);

    rect pr;
    p.get_attack_box(pr, is_alt_attack);

    double weight = 0;
    zombie* result = nullptr;

    unsigned int l, u;
    if (p.type == plant_type::gloomshroom) {
        l = row > 0 ? row - 1 : 0;
        u = row <= scene.rows - 1 ? row + 1 : scene.rows - 1;
    } else if (p.type == plant_type::cattail) {
        l = 0;
        u = scene.rows - 1;
    } else {
        l = u = row;
    }

    for (; l <= u; l++) {
        for (auto& z : scene.zombie_map[l]) {
            if ((!z->is_not_dying || is_target_of_kelp(scene, *z)) &&
                (p.type == plant_type::potato_mine ||
                 p.type == plant_type::chomper ||
                 p.type == plant_type::tangle_kelp))
            {
                continue;
            }

            if (!damage(scene).can_be_attacked(*z, flags)) {
                continue;
            }

            int overlap = 0;

            if (p.type == plant_type::chomper) {
                if (z->status == zombie_status::digger_walk_right) {
                    pr.x += 20;
                    pr.width -= 20;
                } else if (z->status == zombie_status::pogo_with_stick ||
                   z->type == zombie_type::bungee && z->bungee_col == p.col)
                {
                    continue;
                }

                if (z->is_eating || p.status == plant_status::chomper_bite_begin) {
                    overlap = 60;
                }
            } else if (p.type == plant_type::potato_mine) {
                if (z->type == zombie_type::pogo && z->has_item_or_walk_left ||
                    z->status == zombie_status::pole_valuting_jumpping ||
                    z->status == zombie_status::pole_valuting_running)
                {
                    continue;
                }

                if (z->type != zombie_type::pole_vaulting) {
                    pr.x += 40;
                    pr.width -= 40;
                }

                if (z->type == zombie_type::bungee && z->bungee_col != p.col) {
                    continue;
                }

                if (z->is_eating) {
                    overlap = 30;
                }
            }

            if (z->status != zombie_status::pole_valuting_jumpping &&
                (p.type != plant_type::tangle_kelp || z->is_in_water))
            {
                rect zr;
                z->get_hit_box(zr);

                if (pr.get_overlap_len(zr) >= -overlap) {
                    double w = -zr.x;

                    if (p.type == plant_type::cattail) {
                        auto x1 = zr.x + zr.width / 2;
                        auto y1 = zr.y + zr.height / 2;
                        auto x2 = p.x + 40;
                        auto y2 = p.y + 40;

                        auto d = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
                        w = -d;

                        if (z->status == zombie_status::balloon_flying ||
                            z->status == zombie_status::balloon_falling)
                        {
                            w = 10000 - d;
                        }
                    }

                    if (result == nullptr || w > weight) {
                        weight = w;
                        result = z;
                    }
                }
            }
        }
    }

    return result;
}

void mushroom_base::init(
    plant& p,
    plant_type type,
    unsigned int row,
    unsigned int col)
{
    plant_base::init(p, type, row, col);

    if (scene.type == scene_type::pool ||
        scene.type == scene_type::day ||
        scene.type == scene_type::roof)
    {
        p.set_sleep(true);
    }
}
void plant_base::set_launch_countdown(object::plant & p, bool is_alt_attack) {
    auto target = find_target(p, p.row, is_alt_attack);
    if (target == nullptr) {
        return;
    }

    if (p.type == plant_type::split_pea && is_alt_attack) {
        p.countdown.launch = 26;
        p.split_pea_attack_flags.back = true;
        return;
    }

    if ((p.type == plant_type::pea_shooter ||
        p.type == plant_type::snow_pea ||
        p.type == plant_type::repeater ||
        p.type == plant_type::gatling_pea ||
        p.type == plant_type::split_pea ||
        p.type == plant_type::threepeater) &&
        p.has_reanim(plant_reanim_name::anim_shooting))
    {
        p.countdown.launch = 35;

        if (p.type == plant_type::repeater || p.type == plant_type::split_pea) {
            p.countdown.launch = 26;

            if (p.type == plant_type::split_pea) {
                p.split_pea_attack_flags.front = true;
            }
        } else if (p.type == plant_type::gatling_pea) {
            p.countdown.launch = 100;
        }

        return;
    }

    if (p.status == plant_status::cactus_tall_idle) {
        p.set_reanim(plant_reanim_name::anim_shootinghigh, reanim_type::once, 35);
        p.countdown.launch = 23;
        return;
    }

    if (p.type == plant_type::gloomshroom) {
        p.set_reanim(plant_reanim_name::anim_shooting, reanim_type::once, 14);
        p.countdown.launch = 200;
        return;
    }

    if (p.type == plant_type::cattail) {
        p.set_reanim(plant_reanim_name::anim_shooting, reanim_type::once, 30);
        p.countdown.launch = 50;
        return;
    }

    if (!p.has_reanim(plant_reanim_name::anim_shooting)) {
        plant_system(scene).launch(p, target, p.row, is_alt_attack);
        return;
    }

    p.set_reanim(plant_reanim_name::anim_shooting, reanim_type::once, 35);

    switch (p.type) {
    case plant_type::fumeshroom:
        p.countdown.launch = 50;
        return;

    case plant_type::puffshroom:
        p.countdown.launch = 29;
        return;

    case plant_type::scaredyshroom:
        p.countdown.launch = 25;
        return;

    case plant_type::cabbagepult:
        p.countdown.launch = 32;
        return;

    case plant_type::melonpult:
    case plant_type::winter_melon:
        p.countdown.launch = 36;
        return;

    case plant_type::kernelpult:
        if (rng.randint(4) == 0) {
            p.status = plant_status::kernelpult_launch_butter;
        }
        p.countdown.launch = 30;
        return;

    case plant_type::cactus:
        p.countdown.launch = 35;
        return;

    default:
        p.countdown.launch = 29;
    }
}

void sun_plant_base::produce_sun(object::plant & p) {
    if (scene.spawn.countdown.endgame > 0) {
        return;
    }

    if (--p.countdown.generate <= 0) {
        sun sun(scene);

        p.countdown.generate = p.max_boot_delay - 150 + rng.randint(151);

        if (p.type == plant_type::sunshroom) {
            if (p.status == plant_status::sunshroom_small) {
                sun.add_sun(15);
            } else {
                sun.add_sun(25);
            }
        } else if (p.type == plant_type::twin_sunflower) {
            sun.add_sun(50);
        } else if (p.type == plant_type::sunflower) {
            sun.add_sun(25);
        }
    }
}

}

