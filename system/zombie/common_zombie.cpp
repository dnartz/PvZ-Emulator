#include <cstring>

#include "zombie.h"
#include "system/util.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void zombie_base::init(object::zombie &z, zombie_type type, unsigned int row) {
    z.type = type;

    z.status = zombie_status::walking;
    z.action = zombie_action::none;

    z.row = row;
    z.spawn_wave = scene.spawn.wave;

    z.hp = 270;

    z.x = static_cast<float>(rng.randint(40) + 780);
    if (z.spawn_wave == 9 || z.spawn_wave == 19) {
        z.x += 40;
    }

    z.y = zombie_init_y(scene.type, z, z.row);

    z.int_x = 0;
    z.int_y = 0;

    z.dx = 0;
    z.dy = 0;
    z.d2y = 0;

    z.hit_box.x = 36;
    z.hit_box.y = 0;
    z.hit_box.width = 42;
    z.hit_box.height = 115;
    z.hit_box.offset_x = 120;
    z.hit_box.offset_y = 120;

    z.attack_box.x = 50;
    z.attack_box.y = 0;
    z.attack_box.width = 20;
    z.attack_box.height = 115;

    z.is_hypno = false;
    z.is_eating = false;
    z.is_dead = false;
    z.is_blown = false;
    z.is_not_dying = true;
    z.has_item_or_walk_left = false;
    z.is_in_water = false;
    z.has_balloon = false;
    z.has_eaten_garlic = false;

    z.accessory_1.type = zombie_accessories_type_1::none;
    z.accessory_1.hp = 0;
    z.accessory_2.type = zombie_accessories_type_2::none;
    z.accessory_1.hp = 0;

    memset(&z.countdown, 0, sizeof(z.countdown));
    z.countdown.dead = -1;

    z.time_since_spawn = 0;
    z.time_since_ate_garlic = 0;

    z.ladder_col = -1;

    z.bungee_col = -1;
    z.bungee_target = -1;

    z.garlic_tick.a = 0;
    z.garlic_tick.b = 12;
    z.garlic_tick.c = 12;

    z.master_id = -1;
    for (auto& p : z.partners) {
        p = -1;
    }

    z.catapult_or_jackson.n_basketballs = 0;

    memset(&z.reanim, 0, sizeof(z.reanim));
    z.reanim.prev_progress = -1;

    reanim.update_dx(z, false);
    z.init_reanim();
    reanim.update_status(z);
}

void zombie_base::set_common_fields(zombie &z) {
    z.max_hp = z.hp;
    z.accessory_1.max_hp = z.accessory_1.hp;
    z.accessory_2.max_hp = z.accessory_2.hp;

    z.int_x = static_cast<int>(z.x);
    z.int_y = static_cast<int>(z.y);

    reanim.update_progress(z.reanim);
}

bool zombie_base::can_attack_plant(zombie& z, plant& p, zombie_attack_type type) {
    if (p.is_squash_attacking() ||
        p.is_smashed ||
        p.edible == plant_edible_status::invisible_and_not_edible ||
        p.is_dead ||
        p.type == plant_type::tangle_kelp ||
        (!z.is_in_water &&
            (scene.type == scene_type::fog || scene.type == scene_type::pool) &&
            (p.row == 2 || p.row == 3)))
    {
        return false;
    }

    if (z.status == zombie_status::digger_dig) {
        return p.type == plant_type::potato_mine &&
            p.status == plant_status::idle;
    }

    if (p.type == plant_type::spikeweed || p.type == plant_type::spikerock) {
        if (z.type != zombie_type::gargantuar &&
            z.type != zombie_type::giga_gargantuar &&
            z.type != zombie_type::zomboni &&
            !scene.is_water_grid(p.row, p.col) &&
            scene.find_flower_pot(p.row, p.col) == nullptr)
        {
            return false;
        } else {
            return true;
        }
    }

    switch (type) {
    case zombie_attack_type::smash_or_eat: {
        auto& gs = scene.plant_map[p.row][p.col];

        auto other = gs.pumpkin;
        if (gs.pumpkin == nullptr) {
            other = gs.content;
            if (gs.content == nullptr) {
                other = gs.base;
            }
        }

        if (other != nullptr &&
            other != &p &&
            can_attack_plant(z, *other, zombie_attack_type::smash_or_eat))
        {
            return false;
        } else {
            return true;
        }
    }

    case zombie_attack_type::crush:
        return p.type != plant_type::cherry_bomb &&
            p.type != plant_type::jalapeno &&
            p.type != plant_type::blover &&
            p.type != plant_type::squash &&
            ((p.type != plant_type::doomshroom &&
                p.type != plant_type::iceshroom) || p.is_sleeping);

    case zombie_attack_type::jump: {
        auto& gp = scene.plant_map[p.row][p.col];

        if (gp.content != nullptr &&
            gp.content != &p &&
            can_attack_plant(z, *gp.content, zombie_attack_type::jump)) {
            return false;
        } else {
            return true;
        }
    }

    case pvz_emulator::system::zombie_attack_type::place_ladder:
        if (z.status != zombie_status::ladder_walking &&
            z.status != zombie_status::ladder_placing &&
            p.type != plant_type::tallnut &&
            p.type != plant_type::pumpkin &&
            p.type != plant_type::wallnut)

        {
            return false;
        }

        for (auto& item : scene.griditems) {
            if (item.col == p.col &&
                item.row == p.row &&
                item.type == griditem_type::ladder)
            {
                return false;
            }
        }

        return true;

    default:
        return false;
    }
}

plant* zombie_base::find_target(zombie& z, zombie_attack_type type) {
    struct rect zr;

    z.get_attack_box(zr);

    for (auto& p : scene.plants) {
        if (p.row != z.row) {
            continue;
        }

        if (can_attack_plant(z, p, type)) {
            struct rect pr;
            p.get_hit_box(pr);

            if (zr.get_overlap_len(pr) >= 20) {
                return &p;
            }
        }
    }

    return nullptr;
}

void common_zombie::init(zombie& z, zombie_type type, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, type, row);

    z.attack_box.x = 20;
    z.attack_box.y = 0;
    z.attack_box.width = 50;
    z.attack_box.height = 115;

    if (type == zombie_type::conehead) {
        z.accessory_1.type = zombie_accessories_type_1::roadcone;
        z.accessory_1.hp = 370;
    } else if (type == zombie_type::buckethead) {
        z.accessory_1.type = zombie_accessories_type_1::bucket;
        z.accessory_1.hp = 1100;
    } else if (type == zombie_type::screendoor) {
        z.accessory_2.type = zombie_accessories_type_2::screen_door;
        z.accessory_2.hp = 1100;
    } else if (type == zombie_type::flag) {
        z.has_item_or_walk_left = true;
        z.x = 800;
    }

    set_common_fields(z);
}

long double zombie_base::predict_after(zombie& z, float cs) {
    double dx = z.dx;
    
    if (z.countdown.slow > 0) {
        dx *= 0.4000000059604645;
    }

    if (z.is_walk_right()) {
        dx = -dx;
    }

    if (is_not_movable(scene, z)) {
        dx = 0;
    }

    rect rect;
    z.get_hit_box(rect);

    return rect.x + static_cast<long double>(rect.width) / 2 - dx * cs;
}

}
