#include <tuple>
#include <cmath>
#include "zombie.h"
#include "system/util.h"
#include "system/damage.h"
#include "system/zombie/zombie_factory.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_bungee::select_target(zombie &z) {
    int n_sun_plants = 0;

    bool has_grave_or_bungee[6][9] = {{0}};

    for (auto& z : scene.zombies) {
        if (z.has_death_status() ||
            z.type != zombie_type::bungee ||
            z.bungee_col == -1)
        {
            continue;
        }

        has_grave_or_bungee[z.row][z.bungee_col] = true;

        auto& status = scene.plant_map[z.row][z.bungee_col];

        if (status.content && !status.content->is_sun_plant()) {
            n_sun_plants++;
        }
    }

    int n_total_sun_plants = 0;

    for (auto& p : scene.plants) {
        if (p.is_sun_plant()) {
            n_total_sun_plants++;
        }
    }

    bool steal_sun_plants = n_total_sun_plants - n_sun_plants > 1;

    for (auto& item : scene.griditems) {
        if (item.type == griditem_type::grave) {
            has_grave_or_bungee[item.row][item.col] = true;
        }
    }

    std::vector<unsigned int> weights;
    std::vector<std::pair<int, int>> targets;

    for (unsigned int i = 0; i < scene.rows; i++) {
        for (int j = 0; j < 9; j++) {
            if (has_grave_or_bungee[i][j]) {
                continue;
            }

            auto& status = scene.plant_map[i][j];

            int w = 1;

            plant* target = nullptr;
            if (status.coffee_bean) {
                target = status.coffee_bean;
            } else if (status.content) {
                target = status.content;
            } else if (status.pumpkin) {
                target = status.pumpkin;
            } else if (status.base) {
                target = status.base;
            }

            if (target &&
                target->type != plant_type::grave_buster &&
                target->type != plant_type::cob_cannon &&
                (steal_sun_plants || !target->is_sun_plant()))
            {
                w = 10000;
            }

            weights.push_back(w);
            targets.emplace_back(i, j);
        }
    }

    if (weights.empty()) {
        zombie_factory(scene).destroy(z);
        return;
    }

    auto& t = targets[rng.random_weighted_sample(weights)];

    z.row = std::get<0>(t);
    z.bungee_col = std::get<1>(t);

    z.x = static_cast<float>(80 * z.bungee_col + 40);
    z.y = zombie_init_y(scene.type, z, z.row);
}

plant* zombie_bungee::find_umbrella(object::zombie& z) {
    for (auto& p : scene.plants) {
        if (p.type == plant_type::umbrella_leaf &&
            !p.is_smashed &&
            p.edible != plant_edible_status::invisible_and_not_edible &&
            !p.is_dead &&
            abs(static_cast<int>(p.row) - static_cast<int>(z.row)) <= 1 &&
            abs(static_cast<int>(p.col) - z.bungee_col) <= 1)
        {
            return &p;
        }
    }

    return nullptr;
}

void zombie_bungee::update(zombie& z) {
    if (z.is_dead ||
        z.has_death_status() ||
        z.countdown.freeze > 0 ||
        z.countdown.butter > 0)
    {
        return;
    }

    switch (z.status) {
    case zombie_status::bungee_target_drop:
    case zombie_status::bungee_body_drop: {
        z.dy -= 8;

        if (z.status == zombie_status::bungee_target_drop && z.dy < 1500) {
            z.status = zombie_status::bungee_body_drop;
        }

        if (z.dy > 40) {
            break;
        }

        if (auto umberalla = find_umbrella(z)) {
            damage(scene).activate_plant(*umberalla);
            z.status = zombie_status::bungee_raise;
        } else if (z.dy <= 0) {
            z.dy = 0;
            
            if (auto roof_partner = scene.zombies.get(z.master_id)) {
                roof_partner->action = zombie_action::none;
                reanim.update_status(*roof_partner);
                z.master_id = -1;
                z.status = zombie_status::bungee_raise;
                reanim.set(z, zombie_reanim_name::anim_raise, reanim_type::once, 36);
            } else {
                z.status = zombie_status::bungee_idle_after_drop;
                z.countdown.action = 300;
                reanim.set(z, zombie_reanim_name::anim_idle, reanim_type::repeat, 24);
                z.reanim.progress = 0.5;
            }
        }

        break;
    }

    case zombie_status::bungee_idle_after_drop:
        if (z.countdown.action <= 0) {
            reanim.set(z, zombie_reanim_name::anim_grab, reanim_type::once, 24);

            auto& status = scene.plant_map[z.row][z.bungee_col];
            auto p = status.coffee_bean;
            if (p == nullptr) {
                p = status.content;
                if (p == nullptr) {
                    p = status.pumpkin;
                    if (p == nullptr) {
                        p = status.base;
                    }
                }
            }

            if (p &&
                !p->is_squash_attacking() &&
                !p->is_smashed &&
                p->edible != plant_edible_status::invisible_and_not_edible &&
                !p->is_dead && p->type != plant_type::cob_cannon)
            {
                z.bungee_target = scene.plants.get_index(*p);
                p->edible = plant_edible_status::invisible_and_edible;
            }

            z.status = zombie_status::bungee_grab;
        }
        break;

    case zombie_status::bungee_grab:
        if (z.reanim.n_repeated > 0) {
            reanim.set(z, zombie_reanim_name::anim_raise, reanim_type::once, 36);

            if (auto p = scene.plants.get(z.bungee_target)) {
                p->edible = plant_edible_status::invisible_and_not_edible;
                p->reanim.fps = 0.1f;

                if (p->type == plant_type::cattail) {
                    auto& status = scene.plant_map[p->row][p->col];
                    if (status.pumpkin) {
                        plant_factory(scene).create(
                            plant_type::lily_pad,
                            p->row,
                            p->col);
                    }
                }
            }

            z.status = zombie_status::bungee_raise;
        }
        break;

    case zombie_status::bungee_raise:
        z.dy += 8;
        if (z.dy >= 600) {
            zombie_factory(scene).destroy(z);
        }
        break;

    default:
        break;
    }

    z.int_x = static_cast<int>(z.x);
    z.int_y = static_cast<int>(z.y);
}

void zombie_bungee::init(zombie &z, unsigned int row) {
    z._ground = nullptr;

    zombie_base::init(z, zombie_type::bungee, row);

    z.hp = 450;
    z.garlic_tick.b = 4;
    z.dy = static_cast<float>(rng.randint(151) + 3000);
    z.dx = 0;

    select_target(z);

    z.status = zombie_status::bungee_target_drop;

    reanim.set(z, zombie_reanim_name::anim_drop, reanim_type::repeat, 24);

    z.hit_box.x = -20;
    z.hit_box.y = 22;
    z.hit_box.width = 110;
    z.hit_box.height = 94;
    
    z.attack_box.x = 0;
    z.attack_box.y = 0;
    z.attack_box.width = 0;
    z.attack_box.height = 0;

    set_common_fields(z);
}

}
