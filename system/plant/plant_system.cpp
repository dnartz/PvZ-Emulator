#include <cmath>
#include <cassert>
#include "plant_system.h"
#include "system/damage.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void plant_system::get_pea_offset(const plant& p, int& x, int& y) {
    reanim_frame_status rfs;
    p.reanim.get_frame_status(rfs);

    float nx, cx;
    float ny, cy;

    switch (p.type) {
    case plant_type::pea_shooter:
        cx = plant_pea_shooter::PEA_OFFSETS[rfs.frame].first;
        cy = plant_pea_shooter::PEA_OFFSETS[rfs.frame].second;
        nx = plant_pea_shooter::PEA_OFFSETS[rfs.next_frame].first;
        ny = plant_pea_shooter::PEA_OFFSETS[rfs.next_frame].second;
        break;

    case plant_type::snow_pea:
        cx = plant_snow_pea::PEA_OFFSETS[rfs.frame].first;
        cy = plant_snow_pea::PEA_OFFSETS[rfs.frame].second;
        nx = plant_snow_pea::PEA_OFFSETS[rfs.next_frame].first;
        ny = plant_snow_pea::PEA_OFFSETS[rfs.next_frame].second;
        break;

    case plant_type::repeater:
        cx = plant_repeater::PEA_OFFSETS[rfs.frame].first;
        cy = plant_repeater::PEA_OFFSETS[rfs.frame].second;
        nx = plant_repeater::PEA_OFFSETS[rfs.next_frame].first;
        ny = plant_repeater::PEA_OFFSETS[rfs.next_frame].second;
        break;

    case plant_type::split_pea:
        cx = plant_split_pea::PEA_OFFSETS[rfs.frame].first;
        cy = plant_split_pea::PEA_OFFSETS[rfs.frame].second;
        nx = plant_split_pea::PEA_OFFSETS[rfs.next_frame].first;
        ny = plant_split_pea::PEA_OFFSETS[rfs.next_frame].second;
        break;

    case plant_type::gatling_pea:
        cx = plant_gatling_pea::PEA_OFFSETS[rfs.frame].first;
        cy = plant_gatling_pea::PEA_OFFSETS[rfs.frame].second;
        nx = plant_gatling_pea::PEA_OFFSETS[rfs.next_frame].first;
        ny = plant_gatling_pea::PEA_OFFSETS[rfs.next_frame].second;
        break;

    default:
        assert(false);
        return;
    }

    x = static_cast<int>((nx - cx) * rfs.frame_progress + cx);
    y = static_cast<int>((ny - cy) * rfs.frame_progress + cy);
}

void plant_system::launch(plant& p,
    zombie* target,
    unsigned int row,
    bool is_alt_attack)
{
    projectile_type type = projectile_type::none;

    switch (p.type) {
    case plant_type::fumeshroom:
        damage.range_attack(p, 0x0 | zombie_damage_flags::not_reduce);
        return;

    case plant_type::gloomshroom:
        damage.range_attack(p, 0x0 | zombie_damage_flags::not_reduce);
        return;

    case plant_type::starfruit:
        subsystems.starfruit.attack(p);
        return;

    case plant_type::pea_shooter:
    case plant_type::repeater:
    case plant_type::threepeater:
    case plant_type::split_pea:
    case plant_type::gatling_pea:
        type = projectile_type::pea;
        break;

    case plant_type::snow_pea:
        type = projectile_type::snow_pea;
        break;

    case plant_type::cabbagepult:
        type = projectile_type::cabbage;
        break;

    case plant_type::kernelpult:
        if (is_alt_attack) {
            type = projectile_type::butter;
        } else {
            type = projectile_type::kernel;
        }
        break;

    case plant_type::melonpult:
        type = projectile_type::melon;
        break;

    case plant_type::winter_melon:
        type = projectile_type::wintermelon;
        break;

    case plant_type::puffshroom:
    case plant_type::scaredyshroom:
    case plant_type::seashroom:
        type = projectile_type::puff;
        break;

    case plant_type::cactus:
    case plant_type::cattail:
        type = projectile_type::cactus;
        break;

    case plant_type::cob_cannon:
        type = projectile_type::cob_cannon;
        break;

    default:
        break;
    }

    int x, y;
    switch (p.type) {
    case plant_type::puffshroom:
        x = p.x + 40;
        y = p.y + 40;
        break;

    case plant_type::seashroom:
        x = p.x + 45;
        y = p.y + 63;
        break;

    case plant_type::cabbagepult:
        x = p.x + 5;
        y = p.y - 12;
        break;

    case plant_type::melonpult:
    case plant_type::winter_melon:
        x = p.x + 25;
        y = p.y - 46;
        break;

    case plant_type::cattail:
        x = p.x + 20;
        y = p.y - 3;
        break;

    case plant_type::kernelpult:
        if (is_alt_attack) {
            x = p.x + 12;
            y = p.y - 56;
        } else {
            x = p.x + 19;
            y = p.y - 37;
        }
        break;

    case plant_type::pea_shooter:
    case plant_type::snow_pea:
    case plant_type::repeater:
        get_pea_offset(p, x, y);
        x += p.x + 24;
        y += p.y - 33;
        break;

    case plant_type::gatling_pea:
        get_pea_offset(p, x, y);
        x += p.x + 34;
        y += p.y - 33;
        break;

    case plant_type::split_pea:
        get_pea_offset(p, x, y);

        if (is_alt_attack) {
            x += p.x - 64;
        } else {
            x += p.x + 24;
        }

        y += p.y - 33;
        break;

    case plant_type::threepeater:
        x = p.x + 45;
        y = p.y + 10;
        break;

    case plant_type::scaredyshroom:
        x = p.x + 29;
        y = p.y + 21;
        break;

    case plant_type::cactus:
        if (is_alt_attack) {
            x = p.x + 70;
            y = p.y + 23;
        } else {
            x = p.x + 93;
            y = p.y - 50;
        }
        break;

    case plant_type::cob_cannon:
        x = p.x - 44;
        y = p.y - 184;
        break;

    default:
        x = p.x + 10;
        y = p.y + 5;
        break;
    }

    for (auto& pot : scene.plants) {
        if (pot.row == p.row &&
            pot.col == p.col &&
            pot.type == plant_type::flower_pot &&
            !pot.is_smashed &&
            pot.edible != plant_edible_status::invisible_and_not_edible &&
            !pot.is_dead)
        {
            y -= 5;
        }
    }

    auto& proj = projectile_factory.create(type, row, x, y);
    proj.flags = p.get_attack_flags(is_alt_attack);

    double dist_x;
    double dist_y;

    switch (p.type) {
    case plant_type::cabbagepult:
    case plant_type::kernelpult:
    case plant_type::melonpult:
    case plant_type::winter_melon:
        if (target == nullptr) {
            dist_x = 700.0 - x;
            dist_y = 0;
        } else {
            rect zr;
            target->get_hit_box(zr);

            dist_x = zombie_base(scene).predict_after(*target, 50) - x - 30;
            dist_y = static_cast<double>(zr.y) - y;

            if (target->status == zombie_status::dophin_ride) {
                dist_x -= 60;
            } else if (target->type == zombie_type::pogo &&
                target->has_item_or_walk_left)
            {
                dist_x -= 60;
            } else if (target->status == zombie_status::snorkel_swim) {
                dist_x -= 40;
            }
        }

        proj.motion_type = projectile_motion_type::parabola;
        proj.dx = static_cast<float>(std::max(40.0, dist_x) / 120.0);
        proj.dy2 = 0;
        proj.ddy = static_cast<float>(dist_y / 120.0 - 7.0);
        proj.dddy = 0.115f;
        break;

    case plant_type::threepeater:
        if (row > p.row) {
            proj.motion_type = projectile_motion_type::switch_way;
            proj.dy2 = 3;
            proj.shadow_y -= 80;
        } else if (row < p.row) {
            proj.motion_type = projectile_motion_type::switch_way;
            proj.dy2 = -3;
            proj.shadow_y += 80;
        }
        break;

    case plant_type::puffshroom:
    case plant_type::seashroom:
        proj.motion_type = projectile_motion_type::puff;
        break;

    case plant_type::cattail:
        proj.dx = 2;
        proj.motion_type = projectile_motion_type::cattail;
        proj.target = scene.zombies.get_index(*target);
        break;

    case plant_type::cob_cannon:
        proj.flags = p.get_attack_flags(false);
        proj.dx = 0.001f;
        proj.motion_type = projectile_motion_type::parabola;
        proj.dy2 = 0;
        proj.ddy = -8;
        proj.dddy = 0;
        proj.cannon_x = static_cast<float>(p.cannon.x - 40);
        proj.cannon_row = get_row_by_x_and_y(scene.type, p.cannon.x, p.cannon.y);
        break;

    case plant_type::split_pea:
        if (is_alt_attack) {
            proj.motion_type = projectile_motion_type::left_straight;
        }

    default:
        break;
    }
}

void plant_system::update_launch_countdown(plant& p) {
    if (p.is_squash_attacking() ||
        p.is_smashed ||
        p.edible == plant_edible_status::invisible_and_not_edible ||
        p.is_dead ||
        p.countdown.launch == 0)
    {
        return;
    }

    p.countdown.launch--;

    switch (p.type) {
    case plant_type::gloomshroom:
        if (p.countdown.launch == 126 ||
            p.countdown.launch == 98 ||
            p.countdown.launch == 70 ||
            p.countdown.launch == 42)
        {
            launch(p, nullptr, p.row, false);
        }
        break;

    case plant_type::gatling_pea:
        if (p.countdown.launch == 18 ||
            p.countdown.launch == 35 ||
            p.countdown.launch == 51 ||
            p.countdown.launch == 68)
        {
            launch(p, nullptr, p.row, false);
        }
        break;

    case plant_type::cattail:
        if (p.countdown.launch == 19) {
            if (auto target = subsystems.cattail.find_target(p, p.row, false)) {
                launch(p, target, p.row, false);
            }
        }
        break;

    default:
        if (p.countdown.launch == 1) {
            switch (p.type) {
            case plant_type::threepeater:
                if (p.row > 0) {
                    launch(p, nullptr, p.row - 1, false);
                }

                launch(p, nullptr, p.row, false);

                if (p.row < scene.get_max_row() - 1) {
                    launch(p, nullptr, p.row + 1, false);
                }

                p.threepeater_time_since_first_shot++;
                break;

            case plant_type::split_pea:
                if (p.split_pea_attack_flags.front) {
                    launch(p, nullptr, p.row, false);
                    p.split_pea_attack_flags.front = false;
                }

                if (p.split_pea_attack_flags.back) {
                    launch(p, nullptr, p.row, true);
                    p.split_pea_attack_flags.back = false;
                }
                break;

            case plant_type::cactus:
                if (p.status == plant_status::cactus_short_idle) {
                    launch(p, nullptr, p.row, true);
                } else {
                    launch(p, nullptr, p.row, false);
                }
                break;

            case plant_type::cabbagepult:
            case plant_type::kernelpult:
            case plant_type::melonpult:
            case plant_type::winter_melon: {
                bool alt = p.status == plant_status::kernelpult_launch_butter;
                auto target = subsystems.base.find_target(p, p.row, alt);

                launch(p, target, p.row, alt);
                break;
            }

            default:
                launch(p, nullptr, p.row, false);
            }

            return;
        }
    }

    if (p.countdown.launch) {
        return;
    }

    if (p.type == plant_type::threepeater) {
        if (p.threepeater_time_since_first_shot <= 30) {
            p.countdown.launch = 1;
        }

        return;
    } 
    
    if (p.status == plant_status::cactus_tall_idle) {
        if (p.reanim.n_repeated > 0) {
            p.set_reanim(plant_reanim_name::anim_idlehigh, reanim_type::once, 0);
            p.reanim.fps = 12;
            return;
        }
    } else if (p.type == plant_type::cob_cannon) {
        if (p.reanim.n_repeated > 0) {
            p.status = plant_status::cob_cannon_unaramed_idle;
            p.countdown.status = 3000;
            p.set_reanim(plant_reanim_name::anim_unarmed_idle, reanim_type::repeat, 12);
            return;
        }
    } else if (p.reanim.n_repeated > 0) {
        p.set_reanim(plant_reanim_name::anim_idle, reanim_type::repeat, 12);
        return;
    }

    p.countdown.launch = 1;
}

void plant_system::update_countdown_and_status(plant& p) {
    if (p.status == plant_status::work || p.is_smashed) {
        if (p.countdown.blover_disappear-- < 1) {
            plant_factory.destroy(p);
            return;
        }
    }

    if (p.countdown.awake > 0 && --p.countdown.awake == 0) {
        p.set_sleep(false);
    }

    if (p.is_sleeping ||
        p.is_smashed ||
        p.edible != plant_edible_status::visible_and_edible)
    {
        return;
    }

    update_launch_countdown(p);

    if (p.countdown.status > 0) {
        p.countdown.status--;
    }

    switch (p.type) {
    case plant_type::squash:
        subsystems.squash.update(p);
        break;

    case plant_type::doomshroom:
        subsystems.doomshroom.update(p);
        break;

    case plant_type::iceshroom:
        subsystems.iceshroom.update(p);
        break;

    case plant_type::chomper:
        subsystems.chomper.update(p);
        break;

    case plant_type::blover:
        subsystems.blover.update(p);
        break;

    case plant_type::flower_pot:
        if (p.status == plant_status::flower_pot_placed &&
            p.countdown.status == 0)
        {
            p.status = plant_status::idle;
        }
        break;

    case plant_type::lily_pad:
        if (p.status == plant_status::lily_pad_placed &&
            p.countdown.status == 0)
        {
            p.status = plant_status::idle;
        }
        break;

    case plant_type::imitater:
        subsystems.imitater.update(p);
        break;

    case plant_type::umbrella_leaf:
        subsystems.umbrella_leaf.update(p);
        break;

    case plant_type::cob_cannon:
        subsystems.cob_cannon.update(p);
        break;

    case plant_type::cactus:
        subsystems.cactus.update(p);
        break;

    case plant_type::magnetshroom:
        subsystems.magnetshroom.update(p);
        break;

    case plant_type::sunshroom:
        subsystems.sunshroom.update(p);

    case plant_type::twin_sunflower:
        subsystems.twin_sunflower.produce_sun(p);
        break;

    case plant_type::sunflower:
        subsystems.sunflower.produce_sun(p);
        break;
        
    case plant_type::grave_buster:
        subsystems.grave_buster.update(p);
        break;

    case plant_type::torchwood:
        subsystems.torchwood.update(p);
        break;

    case plant_type::potato_mine:
        subsystems.potato_mine.update(p);
        break;

    case plant_type::spikeweed:
        subsystems.spickweed.update(p);
        break;

    case plant_type::spikerock:
        subsystems.spikerock.update(p);
        break;

    case plant_type::tangle_kelp:
        subsystems.tangle_kelp.update(p);
        break;

    case plant_type::scaredyshroom:
        subsystems.scaredyshroom.update(p);
        break;

    default:
        break;
    }
}

void plant_system::update_attack(plant& p) {
    if (--p.countdown.generate <= 0) {
        p.countdown.generate = p.max_boot_delay - rng.randint(15);

        switch (p.type) {
        case plant_type::threepeater:
            subsystems.threepeater.set_launch_countdown(p);
            break;

        case plant_type::starfruit:
            subsystems.starfruit.set_launch_countdown(p);
            break;

        case plant_type::split_pea:
            subsystems.split_pea.set_launch_countdown(p, true);

            if (scene.enable_split_pea_bug && p.split_pea_attack_flags.back) {
                subsystems.split_pea.set_launch_countdown(p, false);
            }
            break;

        case plant_type::cactus:
            if (p.status == plant_status::cactus_tall_idle) {
                subsystems.cactus.set_launch_countdown(p, false);
            } else if (p.status == plant_status::cactus_short_idle) {
                subsystems.cactus.set_launch_countdown(p, true);
            }
            break;

        default:
            subsystems.base.set_launch_countdown(p, false);
        }
    }

    if (p.countdown.generate == 50 && p.type == plant_type::cattail) {
        subsystems.cactus.set_launch_countdown(p, false);
    } else if (p.countdown.generate == 25) {
        if (p.type == plant_type::repeater) {
            subsystems.repeater.set_launch_countdown(p, false);
        } else if (p.type == plant_type::split_pea) {
            subsystems.split_pea.set_launch_countdown(p, false);
            subsystems.split_pea.set_launch_countdown(p, true);
        }
    }
}

void plant_system::update() {
    for (auto& p : scene.plants) {
        update_countdown_and_status(p);

        if (p.can_attack) {
            update_attack(p);
        }

        if (p.countdown.effect > 0 && --p.countdown.effect == 0) {
            damage.activate_plant(p);
        }

        if (p.countdown.eaten > 0) {
            p.countdown.eaten--;
        }

        if (p.hp < 0) {
            plant_factory.destroy(p);
        }

        reanim.update_progress(p.reanim);
    }
}

}
