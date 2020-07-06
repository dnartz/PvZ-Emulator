#include <cmath>
#include "plant.h"
#include "system/damage.h"
#include "system/griditem_factory.h"
#include "system/reanim.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_doomshroom::update(plant & p) {
    if (p.is_sleeping || p.status == plant_status::work) {
        return;
    }

    p.status = plant_status::work;
    p.countdown.effect = 100;
    p.set_reanim_frame(plant_reanim_name::anim_explode);
    p.reanim.fps = 23;
    p.reanim.type = reanim_type::once;
}

void plant_sunshroom::init(plant& p, unsigned int row, unsigned int col) {
    mushroom_base::init(p, plant_type::sunshroom, row, col);
    p.x += rng.randint(10) - 5;
    p.y += rng.randint(10) - 5;
    p.status = plant_status::sunshroom_small;
    p.countdown.status = 12000;
}

void plant_sunshroom::update(plant& p) {
    if (p.status == plant_status::sunshroom_small) {
        if (p.countdown.status == 0) {
            p.set_reanim(plant_reanim_name::anim_grow, reanim_type::once, 12);
            p.status = plant_status::sunshroom_grow;
        }

        produce_sun(p);
    } else if (p.status != plant_status::sunshroom_grow) {
        produce_sun(p);
    } else if (p.reanim.n_repeated > 0) {
        p.set_reanim(plant_reanim_name::anim_bigidle, reanim_type::repeat, rng.randfloat(12, 15));
        p.status = plant_status::sunshroom_big;
    }
}

void plant_iceshroom::update(plant & p) {
    if (!p.is_sleeping && p.status != plant_status::work) {
        p.status = plant_status::work;
        p.countdown.effect = 100;
    }
}

bool plant_magnetshroom::attack_zombie(plant& p) {
    zombie* target = nullptr;
    double dist = 0;

    for (auto& z : scene.zombies) {
        rect zr;
        z.get_hit_box(zr);

        auto row_diff = abs(static_cast<int>(p.row) - static_cast<int>(z.row));

        if (z.is_hypno ||
            !z.is_not_dying ||
            z.action != zombie_action::none ||
            z.status == zombie_status::rising_from_ground ||
            z.has_death_status() ||
            zr.x > 800 ||
            row_diff > 2)
        {
            continue;
        }

        if (z.status == zombie_status::digger_dig ||
            z.status == zombie_status::digger_dizzy ||
            z.status == zombie_status::digger_walk_right ||
            z.type == zombie_type::pogo)
        {
            if (!z.has_item_or_walk_left) {
                continue;
            }
        } else if (z.accessory_1.type != zombie_accessories_type_1::bucket &&
            z.accessory_1.type != zombie_accessories_type_1::football_cap &&
            z.accessory_2.type != zombie_accessories_type_2::screen_door &&
            z.accessory_2.type != zombie_accessories_type_2::ladder &&
            z.type != zombie_type::jack_in_the_box)
        {
            continue;
        }

        if (zr.is_overlap_with_circle(p.x, p.y, z.is_eating ? 320 : 270)) {
            auto d = sqrt(pow(p.x - z.x, 2) + pow(p.y - z.y, 2)) + row_diff * 80;
            if (target == nullptr || dist > d) {
                target = &z;
                dist = d;
            }
        }
    }

    if (target == nullptr) {
        return false;
    }

    p.status = plant_status::magnetshroom_working;
    p.countdown.status = 1500;
    p.set_reanim(plant_reanim_name::anim_shooting, reanim_type::once, 12);

    if (target->accessory_1.type == zombie_accessories_type_1::bucket ||
        target->accessory_1.type == zombie_accessories_type_1::football_cap)
    {
        target->accessory_1.type = zombie_accessories_type_1::none;
        target->accessory_1.hp = 0;
    } else if (target->accessory_2.type == zombie_accessories_type_2::screen_door ||
        target->accessory_2.type == zombie_accessories_type_2::ladder) {
        damage(scene).destroy_accessory_2(*target);
        target->status = zombie_status::walking;
        if (!target->is_eating) {
            reanim(scene).update_status(*target);
        }
    } else if (target->type == zombie_type::pogo) {
        zombie_pogo(scene).remove_stick(*target);
    } else if (target->status == zombie_status::jackbox_walking) {
        reanim(scene).update_dx(*target);
        target->status = zombie_status::walking;
    } else if (target->type == zombie_type::digger) {
        if (target->status == zombie_status::digger_dig) {
            target->reanim.prev_fps = 0;
            target->status = zombie_status::digger_lost_dig;
            target->countdown.action = 200;

            reanim reanim(scene);
            reanim.set_fps(*target, 0);
            reanim.update_fps(*target);
        }

        target->has_item_or_walk_left = false;
    }

    return true;
}

void plant_magnetshroom::attack_ladder(plant& p) {
    griditem* target = nullptr;
    double dist = 0;

    for (auto& item : scene.griditems) {
        if (item.type == griditem_type::ladder) {
            auto d1 = abs(static_cast<int>(item.col) - static_cast<int>(item.col));
            auto d2 = abs(static_cast<int>(item.row) - static_cast<int>(item.row));

            if (std::max(d1, d2) <= 2) {
                auto d = d1 * 0.05000000074505806 + std::max(d1, d2);

                if (target == nullptr || dist > d) {
                    target = &item;
                    dist = d;
                }
            }
        }
    }

    if (target) {
        p.status = plant_status::magnetshroom_working;
        p.countdown.status = 1500;
        p.set_reanim(plant_reanim_name::anim_shooting, reanim_type::once, 12);

        griditem_factory(scene).destroy(*target);
    }
}

void plant_magnetshroom::update(plant & p) {
    switch (p.status) {
    case plant_status::magnetshroom_inactive_idle:
        if (p.countdown.status == 0) {
            p.status = plant_status::wait;
            p.set_reanim(plant_reanim_name::anim_idle, reanim_type::repeat, rng.randfloat(10, 15));
        }
        return;

    case plant_status::magnetshroom_working:
        if (p.reanim.n_repeated > 0) {
            p.set_reanim(plant_reanim_name::anim_nonactive_idle2, reanim_type::repeat, 2);
            p.status = plant_status::magnetshroom_inactive_idle;
        }
        return;

    default:
        break;
    }

    if (attack_zombie(p)) {
        return;
    }

    attack_ladder(p);
}

void plant_scaredyshroom::update(plant & p) {
    if (p.countdown.launch > 0) {
        return;
    }

    bool found_scared = false;
    for (auto& z : scene.zombies) {
        auto diff = abs(static_cast<int>(p.row) - static_cast<int>(z.row));

        if (!z.is_hypno &&
            !z.is_dead &&
            !z.has_death_status() &&
            diff <= 1)
        {
            rect zr;
            z.get_hit_box(zr);

            if (zr.is_overlap_with_circle(p.x, p.y + 20, 120)) {
                found_scared = true;
                break;
            }
        }
    }

    switch (p.status) {
    case plant_status::wait:
        if (found_scared) {
            p.status = plant_status::scaredyshroom_scared;
            p.set_reanim(plant_reanim_name::anim_scared, reanim_type::once, 10);
        }
        break;

    case plant_status::scaredyshroom_scared:
        if (p.reanim.n_repeated > 0) {
            p.status = plant_status::scaredyshroom_scared_idle;
            p.set_reanim(plant_reanim_name::anim_scaredidle, reanim_type::repeat, 0);
        }
        break;

    case plant_status::scaredyshroom_scared_idle:
        if (!found_scared) {
            p.status = plant_status::scaredyshroom_grow;
            p.set_reanim(plant_reanim_name::anim_grow, reanim_type::once, rng.randfloat(7, 12));
        }
        break;

    case plant_status::scaredyshroom_grow:
        if (p.reanim.n_repeated > 0) {
            p.status = plant_status::wait;
            p.set_reanim(plant_reanim_name::anim_idle, reanim_type::repeat, rng.randfloat(10, 15));
        }
        break;

    default:
        break;
    }

    if (p.status != plant_status::wait) {
        p.countdown.generate = p.max_boot_delay;
    }
}

}