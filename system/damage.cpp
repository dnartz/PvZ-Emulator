#include <cmath>
#include <cassert>
#include "damage.h"
#include "system/zombie/zombie.h"
#include "system/griditem_factory.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

bool damage::can_be_attacked(const zombie& z, unsigned char flags) {
    if (!(flags & attack_flags::dying_zombies) &&
        (z.is_dead || z.has_death_status()))
    {
        return false;
    }

    if (flags & attack_flags::hypno_zombies) {
        if (!z.is_hypno) {
            return false;
        }
    } else if (z.is_hypno) {
        return false;
    }

    if (z.type == zombie_type::bungee &&
        z.status != zombie_status::bungee_idle_after_drop &&
        z.status != zombie_status::bungee_grab)
    {
        return false;
    }

    if (z.action == zombie_action::fall_from_sky) {
        return false;
    }

    if (z.status == zombie_status::pole_valuting_jumpping ||
        z.status == zombie_status::imp_flying ||
        z.status == zombie_status::digger_drill ||
        z.status == zombie_status::digger_lost_dig ||
        z.status == zombie_status::digger_landing ||
        z.status == zombie_status::dophin_jump_in_pool ||
        z.status == zombie_status::dophin_jump ||
        z.status == zombie_status::snorkel_jump_in_the_pool ||
        z.status == zombie_status::balloon_falling ||
        z.status == zombie_status::rising_from_ground ||
        z.status == zombie_status::backup_spawning)
    {
        return (flags >> 4) & 1;
    }

    rect rect;
    z.get_hit_box(rect);

    if (rect.x > 800) {
        return false;
    }

    bool is_lurking_snorkel = z.type == zombie_type::snorkel &&
        !z.is_eating &&
        z.is_in_water;

    if (flags & attack_flags::lurking_snorkel && is_lurking_snorkel) {
        return true;
    }

    if (flags & attack_flags::digging_digger &&
        z.status == zombie_status::digger_dig)
    {
        return true;
    }

    if (flags & attack_flags::flying_balloon &&
        z.is_flying_or_falling())
    {
        return true;
    }

    if (flags & attack_flags::ground && (
        !z.is_flying_or_falling() &&
        !is_lurking_snorkel &&
        z.status != zombie_status::digger_dig))
    {
        return true;
    }

    return false;
}

void damage::set_is_eating(object::zombie& z) {
    if (z.is_eating) {
        return;
    }

    z.is_eating = true;

    if (z.status == zombie_status::digger_dig) {
        return;
    }

    if (z.status == zombie_status::ladder_walking) {
        reanim.set(z, "anim_laddereat", reanim_type::repeat, 0);
    } else if (z.status == zombie_status::newspaper_running) {
        reanim.set(z, "anim_eat_nopaper", reanim_type::repeat, 0);
    } else if (z.status != zombie_status::pole_valuting_running) {
        reanim.set(z, "anim_eat", reanim_type::repeat, 0);
    }
}

void damage::unset_is_eating(zombie& z) {
    if (z.is_eating) {
        z.is_eating = false;

        if (z.status != zombie_status::digger_dig) {
            if (z.type != zombie_type::snorkel) {
                reanim.update_status(z);
            }

            reanim.update_fps(z);
        }
    }
}

void damage::set_death_state(zombie& z, unsigned int flags) {
    if (z.has_death_status()) {
        return;
    }

    if (z.has_reanim("anim_death") && (
        z.type != zombie_type::dolphin_rider ||
        z.status != zombie_status::dophin_jump_in_pool) &&
        z.status != zombie_status::snorkel_jump_in_the_pool &&
        z.status != zombie_status::snorkel_walking)
    {
        z.countdown.freeze = z.countdown.butter = 0;
        z.has_eaten_garlic = z.time_since_ate_garlic = 0;

        if (flags & zombie_damage_flags::disable_ballon_pop &&
            z.type != zombie_type::gargantuar &&
            z.type != zombie_type::giga_gargantuar)
        {
            zombie_factory.destroy(z);
            return;
        }

        if (z.type == zombie_type::pogo) {
            z.dy = 0;
        }

        unset_is_eating(z);

        if (z.accessory_2.type != zombie_accessories_type_2::none) {
            destroy_accessory_2(z);
        }

        z.dx = 0;
        z.status = zombie_status::dying;

        if (z.action == zombie_action::climbing_ladder) {
            z.action = zombie_action::falling;
        }

        float fps;
        switch (z.type) {
        case zombie_type::football:
            fps = 24;
            break;

        case zombie_type::gargantuar:
        case zombie_type::giga_gargantuar:
        case zombie_type::snorkel:
        case zombie_type::yeti:
            fps = 14;
            break;

        case zombie_type::digger:
            fps = 18;
            break;

        default:
            fps = rng.randfloat(24, 30);
            break;
        }

        const char* name = "anim_death";

        if (z.is_in_water && z.has_reanim("anim_waterdeath")) {
            name = "anim_waterdeath";
        } else {
            auto n = rng.randint(100);

            if (n == 99 &&
                z.has_reanim("anim_superlongdeath") &&
                z.countdown.slow == 0 &&
                scene.get_n_zombies_alive_and_not_hypno() <= 5)
            {
                name = "anim_superlongdeath";
                fps = 14;
            } else if (n > 50 && z.has_reanim("anim_death2")) {
                name = "anim_death2";
            }
        }

        reanim.set(z, name, reanim_type::once, fps);
    }

    zombie_factory.destroy(z);
}

void damage::destroy_accessory_2(zombie& z) {
    if (z.accessory_2.type == zombie_accessories_type_2::none) {
        return;
    }

    z.accessory_2.hp = z.accessory_2.max_hp = 0;

    switch (z.accessory_2.type) {
    case zombie_accessories_type_2::newspaper:
        unset_is_eating(z);

        if (z.has_eaten_garlic) {
            z.has_eaten_garlic = false;
            z.time_since_ate_garlic = 0;
        }

        z.status = zombie_status::newspaper_destoryed;
        reanim.set(z, "anim_gasp", reanim_type::once, 8);

        break;

    case zombie_accessories_type_2::ladder:
        z.status = zombie_status::walking;
        if (z.is_eating) {
            reanim.set(z, "anim_eat", reanim_type::repeat, 0);
        } else {
            reanim.update_status(z);
        }
        break;

    default:
        break;
    }

    z.accessory_2.type = zombie_accessories_type_2::none;
}

void damage::take_body(zombie& z, unsigned int damage, unsigned int flags) {
    if (flags & zombie_damage_flags::slow_effect) {
        debuff.set_slowed(z, 1000);
    }

    z.hp -= damage;

    switch (z.type) {
    case zombie_type::zomboni:
        if (flags & zombie_damage_flags::spike) {
            z.status = zombie_status::dying;
            z.dx = 0;
            z.countdown.action = 280;

            if (rng.randint(4) || z.x >= 600) {
                reanim.set(z, "anim_wheelie1", reanim_type::once, 12);
            } else {
                reanim.set(z, "anim_wheelie2", reanim_type::once, 10);
            }
        } else if (z.hp < 0) {
            zombie_factory.destroy(z);
        }
        break;

    case zombie_type::catapult:
        if (flags & zombie_damage_flags::spike) {
            z.status = zombie_status::dying;
            z.dx = 0;
            z.countdown.action = 280;
            reanim.set(z, "anim_bounce", reanim_type::once, 12);
        } else if (z.hp < 0) {
            zombie_factory.destroy(z);
        }
        break;
    }

    if (z.hp <= 0) {
        z.hp = 0;
        set_death_state(z, flags);
    }
}

void damage::take_ash_attack(zombie& z) {
    if (z.status == zombie_status::dying_from_instant_kill) {
        return;
    }

    if (z.hp >= 1800) {
        take(z, 1800,
            zombie_damage_flags::disable_ballon_pop |
            zombie_damage_flags::not_reduce);
        return;
    }

    if (z.countdown.freeze > 0) {
        debuff.remove_freeze(z);
    }

    if (z.countdown.butter > 0) {
        z.countdown.butter = 0;
    }

    if (z.status != zombie_status::dying &&
        z.status != zombie_status::dying_from_lawnmower &&
        z.status != zombie_status::pole_valuting_jumpping &&
        z.status != zombie_status::imp_flying &&
        z.status != zombie_status::rising_from_ground &&
        z.status != zombie_status::backup_spawning &&
        z.status != zombie_status::dophin_jump_in_pool &&
        z.status != zombie_status::dophin_jump &&
        z.status != zombie_status::dophin_ride &&
        z.status != zombie_status::snorkel_jump_in_the_pool &&
        z.status != zombie_status::digger_dig &&
        z.status != zombie_status::digger_lost_dig &&
        z.status != zombie_status::digger_drill &&
        z.status != zombie_status::digger_landing &&
        !z.is_in_water)
    {
        if (z.type == zombie_type::bungee ||
            z.type == zombie_type::yeti ||
            z.is_flying_or_falling() ||
            !z.is_not_dying)
        {
            z.reanim.prev_fps = 0;
            z.reanim.fps = 0;
            z.status = zombie_status::dying_from_instant_kill;
            z.countdown.action = 300;

            return;
        }
    }

    zombie_factory.destroy(z);
}

void damage::set_smashed(object::plant& p) {
    if (p.is_squash_attacking() || p.is_dead || p.is_smashed) {
        return;
    }

    if (p.is_sleeping ||
        p.type != plant_type::cherry_bomb &&
        p.type != plant_type::jalapeno &&
        p.type != plant_type::doomshroom &&
        p.type != plant_type::iceshroom &&
        (p.type != plant_type::potato_mine ||
            p.status == plant_status::idle))
    {
        if (p.type != plant_type::squash || p.status == plant_status::idle) {
            p.is_smashed = true;
            p.countdown.dead = 500;

            for (auto& item : scene.griditems) {
                if (item.col == p.col &&
                    item.row == p.row &&
                    item.type == griditem_type::ladder)
                {
                    griditem_factory.destroy(item);
                    break;
                }
            }
        }
    } else {
        activate_plant(p);
    }
}

void damage::activate_blover() {
    for (auto& z : scene.zombies) {
        if (!z.has_death_status() &&
            z.status == zombie_status::balloon_flying)
        {
            z.is_blown = true;
        }
    }
}

void damage::activate_plant(object::plant& p) {
    auto flags = p.get_attack_flags();

    int x = p.attack_box.width / 2 + p.x;
    int y = p.attack_box.height / 2 + p.y;

    switch (p.type) {
    case plant_type::blover:
        if (p.status != plant_status::work) {
            p.status = plant_status::work;
            activate_blover();
        }
        return;

    case plant_type::cherry_bomb:
        take_instant_kill(p.row, x, y, 115, 1, true, p.get_attack_flags());
        plant_factory.destroy(p);
        return;

    case plant_type::doomshroom:
        take_instant_kill(p.row, x, y, 250, 3, true, p.get_attack_flags());

        for (auto& other : scene.plants) {
            if (other.row == p.row && other.col == p.col) {
                plant_factory.destroy(other);
            }
        }

        griditem_factory.create(griditem_type::crater, p.row, p.col);

        plant_factory.destroy(p);
        return;

    case plant_type::jalapeno:
        for (auto& z : scene.zombies) {
            if (z.row != p.row || !can_be_attacked(z, flags)) {
                continue;
            }

            debuff.remove_freeze(z);

            if (z.countdown.slow > 0) {
                debuff.remove_slow(z);
            }

            take_ash_attack(z);
        }

        for (auto& item : scene.griditems) {
            if (item.row == p.row && item.type == griditem_type::ladder) {
                griditem_factory.destroy(item);
            }
        }

        plant_factory.destroy(p);
        return;

    case plant_type::umbrella_leaf:
        if (p.status != plant_status::umbrella_leaf_block &&
            p.status != plant_status::umbrella_leaf_shrink)
        {
            p.status = plant_status::umbrella_leaf_block;
            p.countdown.status = 5;
            p.set_reanim("anim_block", reanim_type::once, 22);
        }
        return;

    case plant_type::iceshroom:
        for (auto& z : scene.zombies) {
            bool has_freezed_or_slowed = z.countdown.slow > 0 ||
                z.countdown.freeze > 0;

            debuff.set_slowed(z, 2000);

            if (z.can_be_freezed()) {
                if (z.is_in_water) {
                    z.countdown.freeze = 300;
                } if (has_freezed_or_slowed) {
                    z.countdown.freeze = rng.randint(101) + 300;
                } else {
                    z.countdown.freeze = rng.randint(201) + 400;
                }
            }

            take(z, 20, static_cast<unsigned int>(
                zombie_damage_flags::ignore_accessory_2));

            reanim.update_fps(z);
        }

        scene.spawn.countdown.pool = 300;

        plant_factory.destroy(p);
        return;

    case plant_type::potato_mine:
        take_instant_kill(p.row, x, y, 60, 0, 0, p.get_attack_flags());
        plant_factory.destroy(p);
        return;

    case plant_type::coffee_bean: {
        auto gs = get_grid_plant_status(scene, p.row, p.col);

        if (gs.content && gs.content->is_sleeping) {
            gs.content->countdown.awake = 100;
        }

        p.status = plant_status::work;
        p.set_reanim("anim_crumble", reanim_type::once, 22);

        return;
    }

    default:
        break;
    }
}

void damage::range_attack(object::plant& p, unsigned int flags) {
    rect pr;
    p.get_attack_box(pr);

    auto pf = p.get_attack_flags(false);

    for (auto& z : scene.zombies) {
        auto diff = abs(static_cast<int>(z.row) - static_cast<int>(p.row));
        if (diff > 1 || !can_be_attacked(z, pf)) {
            continue;
        }

        rect zr;
        z.get_hit_box(zr);

        if ((p.type == plant_type::gloomshroom && diff <= 1 || diff == 0) &&
            pr.get_overlap_len(zr) > 0)
        {
            unsigned int d = 20;

            if ((z.type == zombie_type::zomboni ||
                z.type == zombie_type::catapult) &&
                flags & zombie_damage_flags::spike)
            {
                d = 1800;

                if (p.type == plant_type::spikerock) {
                    spikerock.reduce_life(p);
                } else {
                    plant_factory.destroy(p);
                }
            }

            take(z, d, flags);
        }
    }
}

void damage::take_instant_kill(
    int row,
    int x,
    int y,
    int radius,
    int grid_radius,
    bool is_ash_attack,
    unsigned char flags)
{
    for (auto& z : scene.zombies) {
        if (!can_be_attacked(z, flags)) {
            continue;
        }

        rect rect;
        z.get_hit_box(rect);

        if (abs(static_cast<int>(z.row) - row) <= grid_radius &&
            rect.is_intersect_with_circle(x, y, radius))
        {
            if (is_ash_attack) {
                take_ash_attack(z);
            } else {
                take(z,
                    1800,
                    zombie_damage_flags::disable_ballon_pop |
                    zombie_damage_flags::not_reduce);
            }
        }
    }

    auto col = std::max(0, get_col_by_x(x));

    int source_row = std::max(
        0,
        get_row_by_x_and_y(scene.type, std::max(40, x), y));

    for (auto& item : scene.griditems) {
        if (item.type != griditem_type::ladder) {
            continue;
        }

        if (abs(static_cast<int>(item.col) - col) <= grid_radius &&
            abs(static_cast<int>(item.row) - source_row) <= grid_radius)
        {
            griditem_factory.destroy(item);
        }
    }
}

void damage::take(object::zombie& z, unsigned int damage, unsigned int flags) {
    if (z.has_death_status() ||
        z.status == zombie_status::jackbox_pop ||
        z.is_dead)
    {
        return;
    }

    auto d = damage;

    if (z.status == zombie_status::balloon_flying ||
        z.status == zombie_status::balloon_falling &&
        z.has_balloon)
    {
        assert(d >= 20);

        if (z.has_balloon) {
            d -= 20;
            z.has_balloon = false;

            if (!(flags & zombie_damage_flags::disable_ballon_pop) &&
                z.status == zombie_status::balloon_flying)
            {
                z.status = zombie_status::balloon_falling;
                reanim.set(z, "anim_pop", reanim_type::once, 24);
            }

            if ((scene.type == scene_type::pool ||
                scene.type == scene_type::fog) &&
                (z.row == 2 || z.row == 3))
            {
                zombie_factory.destroy(z);
            } else {
                z.action = zombie_action::falling;
            }
        }
    }

    if (d > 0 && z.accessory_2.type != zombie_accessories_type_2::none &&
        !(flags & zombie_damage_flags::ignore_accessory_2))
    {
        auto taken = std::min(z.accessory_2.hp, d);

        if (!(flags & zombie_damage_flags::not_reduce)) {
            d -= taken;
        }

        z.accessory_2.hp -= taken;

        if (z.accessory_2.hp == 0) {
            destroy_accessory_2(z);
        }
    }

    if (d > 0 && z.accessory_1.type != zombie_accessories_type_1::none) {
        auto taken = std::min(z.accessory_1.hp, d);

        d -= taken;
        z.accessory_1.hp -= taken;

        if (z.accessory_1.hp == 0) {
            z.accessory_1.type = zombie_accessories_type_1::none;
        }
    }

    if (d > 0) {
        take_body(z, d, flags);
    }
}

}