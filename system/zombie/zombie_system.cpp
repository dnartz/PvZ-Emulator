#include <cassert>
#include <cstring>
#include <algorithm>
#include "zombie.h"
#include "system/zombie/zombie_system.h"
#include "system/zombie/zombie_factory.h"
#include "object/zombie.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

zombie* zombie_system::find_hypno_enemy(object::zombie& z) {
    if (z.status == zombie_status::digger_dig) {
        return nullptr;
    }

    rect zr;
    z.get_attack_box(zr);

    for (auto & enemy : scene.zombies) {
        if (z.is_hypno == enemy.is_hypno ||
            enemy.row != z.row ||
            enemy.status == zombie_status::balloon_flying ||
            enemy.status == zombie_status::balloon_falling ||
            enemy.status == zombie_status::digger_dig ||
            enemy.status == zombie_status::bungee_target_drop ||
            enemy.status == zombie_status::bungee_body_drop ||
            enemy.status == zombie_status::bungee_raise ||
            enemy.action == zombie_action::fall_from_sky ||
            enemy.has_death_status() ||
            !enemy.is_not_dying)
        {
            continue;
        }

        rect er;
        enemy.get_hit_box(er);

        auto d = zr.get_overlap_len(er);
        if (d >= 20 || d >= 0 && enemy.is_eating) {
            return &enemy;
        }
    }

    return nullptr;
}

void zombie_system::update_eating_plants(zombie& z, plant &p) {
    if (z.status == zombie_status::dancing_moonwalk) {
        z.countdown.action = 1;
        return;
    }

    if (z.has_eaten_garlic) {
        return;
    }

    if (z.type != zombie_type::digger) {
        for (const auto& item : scene.griditems) {
            if (item.row == p.row &&
                item.col == p.col &&
                item.type == griditem_type::ladder)
            {
                damage.unset_is_eating(z);

                if (z.action == zombie_action::none && z.ladder_col != p.col) {
                    z.action = zombie_action::climbing_ladder;
                    z.ladder_col = p.col;
                }

                return;
            }
        }
    }

    damage.set_is_eating(z);
    if (p.type != plant_type::cherry_bomb &&
        p.type != plant_type::jalapeno &&
        p.type != plant_type::doomshroom &&
        p.type != plant_type::iceshroom &&
        p.type != plant_type::hypnoshroom &&
        p.status != plant_status::squash_jump_up &&
        p.status != plant_status::flower_pot_placed &&
        p.status != plant_status::lily_pad_placed &&
        p.status != plant_status::squash_look ||
        p.is_sleeping)
    {
        if (p.type != plant_type::potato_mine || p.status == plant_status::idle) {
            if ((p.type != plant_type::iceshroom || p.is_sleeping) &&
                p.type != plant_type::blover)
            {
                if (z.countdown.slow <= 0 || z.time_since_spawn % 2 == 0) {
                    auto prev_hp = p.hp;
                    p.hp -= 4;

                    if (scene.is_iz && p.type == plant_type::sunflower && 
                        (prev_hp + 24) / 25 != (p.hp + 24) / 25) {
                        scene.sun.sun = std::min(9990u, scene.sun.sun + 25u);
                    }

                    p.countdown.eaten = 50;

                    if (p.hp <= 0) {
                        plant_factory.destroy(p);
                    }
                }
            } else {
                damage.activate_plant(p);
            }
        }
    }
}

void zombie_system::update_blocked_by_tallnut_or_fall_from_ladder(zombie &z) {
    z.dy -= z.status == zombie_status::pole_valuting_running ? 2 : 1;

    if (z.dy <= 0) {
        z.dy = 0;
        z.action = zombie_action::none;
    }
}

void zombie_system::update_dead_from_plant(zombie& z) {
    if (z.action == zombie_action::falling) {
        update_blocked_by_tallnut_or_fall_from_ladder(z);
    }

    if (z.type == zombie_type::zomboni && z.countdown.action > 0 ||
        z.type == zombie_type::catapult) 
    {
        if (--z.countdown.action == 0) {
            zombie_factory.destroy(z);
        }

        return;
    }

    if (z.countdown.dead == -1 && z.reanim.n_repeated > 0) {
        z.countdown.dead = z.is_in_water ? 10 : 100;
    }
}

void zombie_system::update_x(zombie& z) {
    if (is_not_movable(scene, z)) {
        return;
    }

    double dx;

    if (z.status >= zombie_status::pogo_with_stick &&
        z.status <= zombie_status::pogo_jump_across ||
        z.status == zombie_status::dophin_ride ||
        z.status == zombie_status::balloon_flying ||
        z.status == zombie_status::snorkel_swim ||
        z.type == zombie_type::catapult)
    {
        dx = is_slowed(scene, z) ?
            z.dx * 0.4000000059604645 :
            z.dx;
    } else if (z.type == zombie_type::zomboni ||
        z.status == zombie_status::digger_dig ||
        z.status == zombie_status::dophin_jump ||
        z.status == zombie_status::pole_valuting_jumpping ||
        z.status == zombie_status::snorkel_jump_in_the_pool)
    {
        dx = z.dx;
    } else if (z.has_reanim(zombie_reanim_name::_ground)) {
        dx = z.get_dx_from_ground();
    } else {
        dx = is_slowed(scene, z) ?
            z.dx * 0.4000000059604645 :
            z.dx;
    }

    if (z.is_walk_right() || z.status == zombie_status::dancing_moonwalk) {
        z.x += static_cast<float>(dx);
    } else {
        z.x -= static_cast<float>(dx);
    }
}

void zombie_system::update_lurking_dy(zombie &z) {
    double dy;
    double ratio = (50.0 - z.countdown.action) / 50.0;

    if (ratio <= 0) {
        dy = z.is_in_water ? -150 : -200;
    } else if (ratio > 1) {
        dy = z.is_in_water ? -40 : 0;
    } else {
        if (z.is_in_water) {
            dy = 110 * ratio - 150;
        } else {
            dy = 200 * ratio - 200;
        }
    }

    z.dy = static_cast<float>(round(dy));

    if (z.countdown.action == 0) {
        z.status = zombie_status::walking;
    }
}

void zombie_system::update_near_death(zombie& z) {
    if (!z.has_death_status() &&
        (z.type == zombie_type::zomboni ||
            z.type == zombie_type::catapult) &&
            z.hp < 200 ||
            !z.is_not_dying)
    {
        unsigned int d = 1;

        if (z.type == zombie_type::yeti) {
            d = 10;
        }

        if (z.max_hp >= 500) {
            d = 3;
        }

        if (rng.randint(5) == 0) {
            damage.take(
                z,
                d,
                zombie_damage_flags::ignore_accessory_2 |
                zombie_damage_flags::no_flash);
        }
    }
}

bool zombie_system::not_effected_by_future(const object::zombie& z) {
    return !scene.is_future_enabled && (
        z.type == zombie_type::zombie ||
        z.type == zombie_type::conehead ||
        z.type == zombie_type::buckethead ||
        z.type == zombie_type::flag ||
        z.type == zombie_type::screendoor ||
        z.type == zombie_type::ducky_tube ||
        z.type == zombie_type::dancing ||
        z.type == zombie_type::backup_dancer ||
        z.type == zombie_type::newspaper ||
        z.type == zombie_type::pole_vaulting);
}

void zombie_system::set_garlic_row_switch(zombie& z) {
    ++z.time_since_ate_garlic;

    if (z.time_since_ate_garlic > 20 &&
        z.time_since_ate_garlic < 170 &&
        !not_effected_by_future(z))
    {
        damage.unset_is_eating(z);
        z.time_since_ate_garlic = 170;
    }

    if (z.time_since_ate_garlic > 270) {
        z.has_eaten_garlic = false;
        z.time_since_ate_garlic = 0;
        return;
    }

    if (z.time_since_ate_garlic == 70) {
        damage.unset_is_eating(z);
    }

    if (z.time_since_ate_garlic == 170) {
        reanim.update_status(z);

        switch (scene.type) {
        case scene_type::fog:
        case scene_type::pool: {
            static const unsigned int ROW_LOOKUP[] = {
                1, 0, 3, 2, 5, 4
            };

            assert(z.row >= 0 && z.row <= 5);

            z.row = ROW_LOOKUP[z.row % 6];
            break;
        }

        default:
            assert(z.row >= 0 && z.row <= 4);

            switch (z.row) {
            case 0:
                z.row = 1;
                break;

            case 4:
                z.row = 3;
                break;

            default:
                if (rng.randint(2)) {
                    --z.row;
                } else {
                    ++z.row;
                }

                break;
            }
        }
    }
}

void zombie_system::set_hypno_effect(zombie& z) {
    z.is_hypno = true;

    if (z.type == zombie_type::dancing) {
        for (auto& p : z.partners) {
            p = -1;
        }
        return;
    }

    if (z.type == zombie_type::backup_dancer) {
        if (auto m = scene.zombies.get(z.master_id)) {
            for (int i = 0; i < 4; i++) {
                if (m->partners[i] == scene.zombies.get_index(z)) {
                    m->partners[i] = -1;
                    break;
                }
            }
        }

        z.master_id = -1;
    } else if (auto m = scene.zombies.get(z.master_id)) {
        z.master_id = m->master_id = -1;
    }
}

void zombie_system::set_garlic_and_hypno_status(zombie& z) {
    if (z.status == zombie_status::snorkel_up_to_eat) {
        return;
    }

    auto p = subsystems.zombie.find_target(z, zombie_attack_type::smash_or_eat);

    if (p == nullptr) {
        return;
    }

    if (p->type == plant_type::hypnoshroom && !p->is_sleeping) {
        plant_factory.destroy(*p);
        set_hypno_effect(z);
        z.dx = 0.17f;
        z.garlic_tick.a = 18;
    } else if (p->type == plant_type::garlic) {
        z.has_eaten_garlic = true;
        z.time_since_ate_garlic = 0;
    }

    reanim.update_fps(z);
}

void zombie_system::update_garlic_and_hypno_effect(zombie& z) {
    if (z.status == zombie_status::jackbox_pop ||
        z.status == zombie_status::newspaper_destoryed ||
        z.status == zombie_status::digger_drill ||
        z.status == zombie_status::digger_lost_dig ||
        z.status == zombie_status::digger_landing ||
        z.status == zombie_status::digger_dizzy ||
        z.countdown.freeze > 0 ||
        z.countdown.butter > 0)
    {
        return;
    }

    ++z.garlic_tick.c;
    if (z.has_eaten_garlic) {
        set_garlic_row_switch(z);
    }
    
    if (z.is_eating && z.is_not_dying) {
        int ratio = z.countdown.slow > 0 ? 12 : 6;

        if (z.garlic_tick.c >= ratio * z.garlic_tick.b) {
            z.garlic_tick.c = ratio;
        }

        double p1 = 0.14;
        double p2 = 0.68000001;

        if (z.type == zombie_type::pole_vaulting) {
            p1 = 0.38;
            p2 = 0.80000001;
        } else if (z.type == zombie_type::jack_in_the_box) {
            p1 = p2 = 0.52999997;
        } else if (z.type == zombie_type::imp) {
            p1 = 0.33000001;
            p2 = 0.79000002;
        }

        if (z.reanim.is_in_progress(p1) || z.reanim.is_in_progress(p2)) {
            set_garlic_and_hypno_status(z);
        }

        return;
    } else if (z.garlic_tick.c >= z.garlic_tick.a * z.garlic_tick.b) {
        z.garlic_tick.c = 0;
    }
}

void zombie_system::update_climb_ladder(zombie& z) {
    int col = std::max(0, get_col_by_x(static_cast<int>(z.dy * 0.5 + z.int_x + 5)));

    for (auto& item : scene.griditems) {
        if (item.col != col ||
            item.row != z.row ||
            item.type != griditem_type::ladder)
        {
            continue;
        }

        z.dy += 0.800000011920929;

        if (z.dx < 0.5) {
            z.x -= 0.5;
        }

        if (90 <= z.dy) {
            z.action = zombie_action::falling;
        }

        return;
    }

    z.action = zombie_action::falling;
}

void zombie_system::update_action_in_pool(zombie& z) {
    switch (z.action) {
    case zombie_action::entering_pool:
        z.dy -= 1;
        if (-40 >= z.dy) {
            z.dy = -40;
            z.action = zombie_action::none;
            reanim.update_status(z);
        }
        break;

    case zombie_action::leaving_pool:
        z.dy += 1;

        if (z.type == zombie_type::snorkel) {
            z.dy += 1;
        }

        if (z.dy >= 0) {
            z.dy = 0;
            z.action = zombie_action::none;
            z.is_in_water = 0;
        }
        break;

    case zombie_action::caught_by_kelp:
        z.dy -= 1;
        break;

    default:
        break;
    }
}

void zombie_system::update_falling(zombie& z) {
    z.dy -= 1;

    if (z.status == zombie_status::pole_valuting_running) {
        z.dy -= 1;
    }

    if (z.dy <= 0) {
        z.dy = 0;
        z.action = zombie_action::none;
    }
}

void zombie_system::update_status(zombie& z) {
    if (z.action == zombie_action::climbing_ladder) {
        update_climb_ladder(z);
    }

    if (z.action == zombie_action::leaving_pool ||
        z.action == zombie_action::entering_pool ||
        z.is_in_water)
    {
        update_action_in_pool(z);
    }

    if (z.action == zombie_action::falling) {
        update_falling(z);
    }

    switch (z.type) {
    case zombie_type::pole_vaulting:
        subsystems.pole_vaulting.update(z);
        break;

    case zombie_type::catapult:
        subsystems.catapult.update(z);
        break;

    case zombie_type::dolphin_rider:
        subsystems.dolphin_rider.update(z);
        break;

    case zombie_type::snorkel:
        subsystems.snorkel.update(z);
        break;

    case zombie_type::balloon:
        subsystems.balloon.update(z);
        break;

    case zombie_type::newspaper:
        subsystems.newspaper.update(z);
        break;

    case zombie_type::digger:
        subsystems.digger.update(z);
        break;

    case zombie_type::jack_in_the_box:
        subsystems.jack_in_the_box.update(z);
        break;

    case zombie_type::gargantuar:
    case zombie_type::giga_gargantuar:
        subsystems.gargantuar.update(z);
        break;

    case zombie_type::zomboni:
        subsystems.zomboni.update(z);
        break;

    case zombie_type::ladder:
        subsystems.ladder.update(z);
        break;

    case zombie_type::yeti:
        subsystems.yeti.update(z);
        break;

    case zombie_type::dancing:
        subsystems.dancing.update(z);
        break;

    case zombie_type::backup_dancer:
        subsystems.backup_dancer.update(z);
        break;

    case zombie_type::imp:
        subsystems.imp.update(z);
        break;

    default:
        break;
    }
}

void zombie_system::crush_plant(object::zombie& z) {
    rect zr;
    z.get_attack_box(zr);

    for (auto& p : scene.plants) {
        if (p.row != z.row) {
            continue;
        }

        rect pr;
        p.get_hit_box(pr);

        if (zr.get_overlap_len(pr) >= 20 && subsystems.catapult.can_attack_plant(
            z, p, zombie_attack_type::crush) &&
            p.type != plant_type::spikeweed &&
            p.type != plant_type::spikerock)
        {
            damage.set_smashed(p);
            return;
        }
    }
}

void zombie_system::update_pos(zombie& z) {
    if (z.type == zombie_type::bungee ||
        z.status == zombie_status::rising_from_ground)            
    {
        return;
    }

    update_x(z);

    if (z.type == zombie_type::zomboni || z.type == zombie_type::catapult) {
        crush_plant(z);
    }

    float prev_x = z.x;

    if (z.is_blown) {
        z.x += 10;
    }

    if (!z.is_blown || prev_x <= 850) {
        if (z.action == zombie_action::none) {
            auto init_y = zombie_init_y(scene.type, z, z.row);

            if (z.y > init_y) {
                if (z.y - init_y <= 1) {
                    z.y = init_y;
                } else {
                    z.y -= 1;
                }
            } else if (z.y < init_y) {
                if (init_y - z.y <= 1) {
                    z.y = init_y;
                } else {
                    z.y += 1;
                }
            }
        }
    } else {
        zombie_factory.destroy(z);
    }
}

void zombie_system::update_eating(object::zombie& z) {
    if (z.type != zombie_type::bungee &&
        z.type != zombie_type::gargantuar &&
        z.type != zombie_type::giga_gargantuar &&
        z.type != zombie_type::zomboni &&
        z.type != zombie_type::catapult &&
        !z.has_pogo_status() &&
        z.status != zombie_status::pole_valuting_jumpping &&
        z.status != zombie_status::newspaper_destoryed &&
        z.status != zombie_status::digger_drill &&
        z.status != zombie_status::digger_lost_dig &&
        z.status != zombie_status::digger_landing &&
        z.status != zombie_status::digger_dizzy &&
        static_cast<int>(z.status) != 14 &&
        z.status != zombie_status::imp_flying &&
        z.status != zombie_status::imp_landing &&
        z.status != zombie_status::backup_spawning &&
        z.status != zombie_status::dancing_point &&
        z.status != zombie_status::dancing_wait_summoning &&
        z.status != zombie_status::dancing_summoning &&
        z.status != zombie_status::dophin_walk_with_dophin &&
        z.status != zombie_status::dophin_walk_without_dophin &&
        z.status != zombie_status::dophin_jump_in_pool &&
        z.status != zombie_status::dophin_ride &&
        z.status != zombie_status::dophin_jump &&
        z.status != zombie_status::snorkel_jump_in_the_pool &&
        z.status != zombie_status::snorkel_walking &&
        z.status != zombie_status::ladder_placing &&
        z.status != zombie_status::balloon_flying &&
        z.status != zombie_status::balloon_falling &&
        z.action != zombie_action::fall_from_sky &&
        z.action != zombie_action::climbing_ladder &&
        z.action != zombie_action::entering_pool &&
        z.action != zombie_action::leaving_pool &&
        z.action != zombie_action::falling &&
        !is_target_of_kelp(scene, z) &&
        z.is_not_dying)
    {
        int op = z.countdown.slow > 0 ? 8 : 4;
        if (z.time_since_spawn % op == 0) {
            if (auto enemy = find_hypno_enemy(z)) {
                damage.take(*enemy, 4,
                    zombie_damage_flags::no_flash |
                    zombie_damage_flags::ignore_accessory_2);

                damage.set_is_eating(z);
            } else {
                auto target = subsystems.zombie.find_target(
                    z, zombie_attack_type::smash_or_eat);

                if (z.is_hypno || target == nullptr) {
                    if (z.is_eating) {
                        damage.unset_is_eating(z);
                    }
                } else {
                    update_eating_plants(z, *target);
                }
            }
        }
    }
}

void zombie_system::update_water_status(zombie& z) {
    if (z.type != zombie_type::zombie &&
        z.type != zombie_type::conehead &&
        z.type != zombie_type::buckethead &&
        z.type != zombie_type::flag &&
        z.type != zombie_type::balloon ||
        z.status == zombie_status::balloon_flying ||
        z.status == zombie_status::balloon_falling ||
        z.type == zombie_type::dolphin_rider ||
        z.type == zombie_type::snorkel ||
        z.action == zombie_action::entering_pool ||
        z.action == zombie_action::leaving_pool)
    {
        return;
    }

    bool current_in_water;
    if (scene.is_water_grid(z.row, get_col_by_x(z.int_x + 75)) &&
        scene.is_water_grid(z.row, get_col_by_x(z.int_x + 45)))
    {
        current_in_water = z.int_x < 680;
    } else {
        current_in_water = false;
    }

    if (z.is_in_water) {
        if (!current_in_water) {
            z.action = zombie_action::leaving_pool;
            reanim.update_status(z);
        }
    } else if (current_in_water) {
        if (scene.spawn.countdown.pool <= 0) {
            z.action = zombie_action::entering_pool;
            z.is_in_water = true;
        } else {
            z.countdown.freeze = scene.spawn.countdown.pool;
            debuff.set_slowed(z, 2000);
        }
    }
}

bool zombie_system::update_entering_home(object::zombie& z) {
    if (z.is_walk_right() && z.x > 850) {
        zombie_factory.destroy(z);
        return false;
    }

    int threshold;

    switch (z.type) {
    case zombie_type::gargantuar:
    case zombie_type::giga_gargantuar:
    case zombie_type::pole_vaulting:
        threshold = -150;
        break;

    case zombie_type::catapult:
    case zombie_type::football:
    case zombie_type::zomboni:
        threshold = -175;
        break;

    case zombie_type::backup_dancer:
    case zombie_type::dancing:
    case zombie_type::snorkel:
        threshold = -130;
        break;

    default:
        threshold = -100;
        break;
    }

    if (z.int_x < threshold && z.is_not_dying) {
        return true;
    }

    if (z.int_x < threshold + 70 && !z.is_not_dying) {
        damage.take(z, 1800, zombie_damage_flags::ignore_accessory_2 |
            zombie_damage_flags::no_flash);
    }

    return false;
}

bool zombie_system::update() {
    for (auto& z : scene.zombies) {
        z.time_since_spawn++;

        switch (z.status) {
        case zombie_status::dying_from_instant_kill:
            if (z.countdown.action-- == 1) {
                zombie_factory.destroy(z);
            }
            break;

        case zombie_status::dying_from_lawnmower:
            z.countdown.butter = 0;
            z.is_not_dying = false;

            if (z.type == zombie_type::flag) {
                z.has_item_or_walk_left = false;
            }

            zombie_factory.destroy(z);
            break;

        case zombie_status::dying:
            update_dead_from_plant(z);
            update_x(z);
            break;
            
        default:
            if (z.countdown.action > 0 &&
                z.countdown.freeze == 0 &&
                z.countdown.butter == 0)
            {
                z.countdown.action--;
            }

            if (z.countdown.freeze > 0 && --z.countdown.freeze == 0) {
                reanim.update_fps(z);
            }

            if (z.countdown.slow > 0 && --z.countdown.slow == 0) {
                reanim.update_fps(z);
            }

            if (z.countdown.butter > 0 && --z.countdown.butter == 0) {
                reanim.update_fps(z);
            }

            if (z.status == zombie_status::rising_from_ground) {
                update_lurking_dy(z);
                continue;
            }

            if (z.countdown.freeze <= 0 && z.countdown.butter <= 0) {
                update_status(z);
                update_pos(z);
                update_eating(z);
                update_water_status(z);

                if (update_entering_home(z)) {
                    return true;
                }
            }

            update_near_death(z);

            if (z.type == zombie_type::bungee) {
                subsystems.bungee.update(z);
            } else if (z.type == zombie_type::pogo) {
                subsystems.pogo.update(z);
            }

            update_garlic_and_hypno_effect(z);
        }

        if (z.countdown.dead > 0 && --z.countdown.dead == 0) {
            zombie_factory.destroy(z);
        }

        z.int_x = static_cast<int>(z.x);
        z.int_y = static_cast<int>(z.y);

        reanim.update_progress(z.reanim);
    }

    return false;
}

}