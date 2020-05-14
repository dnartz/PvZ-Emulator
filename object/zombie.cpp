#include <cmath>
#include <cassert>
#include <algorithm>
#include "scene.h"
#include "zombie.h"

namespace pvz_emulator::object {

float zombie::get_delta_x_from_ground() const {
    assert(has_reanim("_ground"));

    reanim_frame_status rfs;
    reanim.get_frame_status(rfs);

    return (_ground[rfs.next_frame] - _ground[rfs.frame]) *
        0.009999999776482582f *
        reanim.fps;
}

bool zombie::is_walk_right() const {
    if (is_hypno) {
        return true;
    }

    if (type == zombie_type::digger) {
        if (status == zombie_status::digger_drill ||
            status == zombie_status::digger_dizzy ||
            status == zombie_status::digger_walk_right)
        {
            return true;
        }

        if (has_death_status()) {
            return has_item_or_walk_left;
        }

        return false;
    }

    return type == zombie_type::yeti && !has_item_or_walk_left;
}

bool zombie::can_be_slowed() const {
    if (type == zombie_type::zomboni || is_dead) {
        return false;
    }

    return !has_death_status() &&
        status != zombie_status::digger_dig &&
        status != zombie_status::digger_drill &&
        status != zombie_status::digger_lost_dig &&
        status != zombie_status::digger_landing &&
        status != zombie_status::rising_from_ground &&
        status != zombie_status::backup_spawning &&
        !is_hypno;
}

bool zombie::can_be_freezed() const {
    if (!can_be_slowed()) {
        return false;
    }

    return status != zombie_status::pole_valuting_jumpping &&
        status != zombie_status::dophin_jump_in_pool &&
        status != zombie_status::dophin_jump &&
        status != zombie_status::snorkel_jump_in_the_pool &&
        !is_flying_or_falling() &&
        status != zombie_status::imp_flying &&
        status != zombie_status::imp_landing &&
        static_cast<int>(status) != 19 &&
        !(status >= zombie_status::pogo_with_stick &&
            status <= zombie_status::pogo_jump_across) &&
        (type != zombie_type::bungee || status == zombie_status::bungee_idle_after_drop);
}

void zombie::get_attack_box(rect& rect) const {
    rect.x = attack_box.x;
    rect.y = attack_box.y;
    rect.width = attack_box.width;
    rect.height = attack_box.height;

    if (status == zombie_status::pole_valuting_jumpping ||
        status == zombie_status::dophin_jump)
    {
        rect.x = -40;
        rect.y = 0;
        rect.width = 100;
        rect.height = 115;
    }

    if (is_walk_right()) {
        rect.x = hit_box.offset_x - rect.width - rect.x;
    }

    rect.x += int_x;
    rect.y += static_cast<int>(int_y - dy);

    auto b = get_height_bias();

    if (b > -100) {
        rect.height -= static_cast<int>(b);
    }
}

void zombie::get_hit_box(rect& rect) const {
    rect.x = hit_box.x;
    rect.y = hit_box.y;
    rect.width = hit_box.width;
    rect.height = hit_box.height;

    if (is_walk_right()) {
        rect.x = hit_box.offset_x - rect.width - hit_box.x;
    }

    rect.x += int_x;
    rect.y += static_cast<int>(int_y - dy);

    auto bias = get_height_bias();
    if (bias > -100) {
        rect.height -= static_cast<int>(bias);
    }
}

float zombie::get_height_bias() const {
    if (status == zombie_status::rising_from_ground) {
        if (is_in_water) {
            return -dy;
        } else {
            return -dy + std::min(countdown.action, 40);
        }
    }

    if (type == zombie_type::dolphin_rider) {
        switch (status) {
        case zombie_status::dophin_jump_in_pool:
            if (reanim.progress >= 0.56 && reanim.progress <= 0.64999998) {
                return 0;
            } else if (reanim.progress >= 0.75) {
                return -dy - 10;
            } else {
                return -200;
            }

        case zombie_status::dophin_ride:
            if (action == zombie_action::caught_by_kelp) {
                return -dy - 15;
            } else {
                return -dy - 10;
            }

        case zombie_status::dophin_jump:
            if (reanim.progress <= 0.05999999865889549) {
                return -dy - 10;
            } else if (reanim.progress >= 0.5 && reanim.progress <= 0.75999999) {
                return -13;
            } else {
                return -200;
            }

        case zombie_status::dying:
            return 44 - dy;

        case zombie_status::dophin_walk_in_pool:
            if (action == zombie_action::caught_by_kelp) {
                return 36 - dy;
            } else {
                return -200;
            }

        case zombie_status::dophin_walk_with_dophin:
        case zombie_status::dophin_walk_without_dophin:
            if (action == zombie_action::leaving_pool) {
                return -dy;
            } else {
                return -200;
            }

        default:
            return -200;
        }
    }

    if (type == zombie_type::snorkel) {
        if (status == zombie_status::snorkel_jump_in_the_pool &&
            reanim.progress >= 0.800000011920929)
        {
            return -10;
        } else if (is_in_water) {
            return -dy - 5;
        } else {
            return -200;
        }
    } else if (is_in_water) {
        if (is_eating) {
            return -dy;
        } else {
            return -dy - 7;
        }
    }

    if (status == zombie_status::backup_spawning) {
        return -dy;
    }

    if ((status == zombie_status::digger_drill ||
        status == zombie_status::digger_landing) &&
        countdown.action > 20)
    {
        return -dy;
    }

    return -200;
}

void zombie::set_reanim(const char* name, reanim_type type, float fps) {
    if (fps != 0) {
        reanim.fps = fps;
    }

    reanim.type = type;
    reanim.n_repeated = 0;

    reanim.progress = static_cast<float>(reanim.fps >= 0 ? 0 : 0.99999988);
    reanim.prev_progress = -1;

    set_reanim_frame(name);

    if (reanim.fps == 0) {
        
    } else {

    }
}

void zombie::to_json(
    scene& scene,
    rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartObject();

    writer.Key("id");
    writer.Uint64(scene.zombies.get_index(*this));

    writer.Key("type");
    writer.String(type_to_string(type));

    writer.Key("status");
    writer.String(status_to_string(status));

    writer.Key("int_x");
    writer.Int(int_x);

    writer.Key("int_y");
    writer.Int(int_y);

    writer.Key("x");
    writer.Double(x);
    writer.Key("y");
    writer.Double(y);
    writer.Key("dx");
    writer.Double(dx);
    writer.Key("dy");
    writer.Double(dy);
    writer.Key("d2y");
    writer.Double(d2y);

    writer.Key("reanim");
    reanim.to_json(writer);

    writer.Key("row");
    writer.Uint(row);

    writer.Key("bungee_col");
    writer.Int(bungee_col);

    writer.Key("ladder_col");
    writer.Int(ladder_col);

    rect rect;
    get_hit_box(rect);
    writer.Key("hit_box");
    rect.to_json(writer);

    writer.Key("attack_box");
    get_attack_box(rect);
    rect.to_json(writer);

    writer.Key("countdown");
    writer.StartObject();
    writer.Key("butter");
    writer.Uint(countdown.butter);

    writer.Key("freeze");
    writer.Uint(countdown.freeze);

    writer.Key("slow");
    writer.Uint(countdown.slow);

    writer.Key("action");
    writer.Int(countdown.action);

    writer.Key("dead");
    writer.Int(countdown.dead);
    writer.EndObject();

    writer.Key("spawn_wave");
    writer.Uint(spawn_wave);

    writer.Key("time_since_ate_garlic");
    writer.Uint(time_since_ate_garlic);

    writer.Key("time_since_spawn");
    writer.Uint(time_since_spawn);

    writer.Key("hp");
    writer.Int(hp);

    writer.Key("max_hp");
    writer.Uint(max_hp);

    writer.Key("accessory_1");
    writer.StartObject();
    writer.Key("type");
    writer.String(accessories_type_to_string(accessory_1.type));

    writer.Key("hp");
    writer.Uint(accessory_1.hp);

    writer.Key("max_hp");
    writer.Uint(accessory_1.max_hp);
    writer.EndObject();

    writer.Key("accessory_2");
    writer.StartObject();

    writer.Key("type");
    writer.String(accessories_type_to_string(accessory_2.type));

    writer.Key("hp");
    writer.Uint(accessory_2.hp);

    writer.Key("max_hp");
    writer.Uint(accessory_2.max_hp);
    writer.EndObject();

    writer.Key("master_id");
    if (master_id == -1) {
        writer.Null();
    } else {
        writer.Int(master_id);
    }

    writer.Key("partners");
    writer.StartArray();
    for (auto i : partners) {
        if (i == -1) {
            writer.Null();
        } else {
            writer.Int(i);
        }
    }
    writer.EndArray();

    writer.Key("garlic_tick");
    writer.StartObject();
    writer.Key("a");
    writer.Uint(garlic_tick.a);

    writer.Key("b");
    writer.Uint(garlic_tick.b);

    writer.Key("c");
    writer.Uint(garlic_tick.c);
    writer.EndObject();

    writer.Key("bungee_target");
    if (bungee_target == nullptr) {
        writer.Null();
    } else {
        writer.Uint64(scene.plants.get_index(*bungee_target));
    }

    if (type == zombie_type::catapult) {
        writer.Key("n_basketballs");
        writer.Int(catapult_or_jackson.n_basketballs);
    } else if (type == zombie_type::dancing) {
        writer.Key("summon_countdown");
        writer.Int(catapult_or_jackson.summon_countdown);
    }

    writer.Key("is_eating");
    writer.Bool(is_eating);

    writer.Key("is_dead");
    writer.Bool(is_dead);

    writer.Key("is_blown");
    writer.Bool(is_blown);

    writer.Key("is_not_dying");
    writer.Bool(is_not_dying);

    writer.Key("is_hypno");
    writer.Bool(is_hypno);

    writer.Key("has_item_or_walk_left");
    writer.Bool(has_item_or_walk_left);

    writer.Key("is_in_water");
    writer.Bool(is_in_water);

    writer.Key("has_balloon");
    writer.Bool(has_balloon);

    writer.Key("has_eaten_garlic");
    writer.Bool(has_eaten_garlic);

    writer.EndObject();
}

const char* zombie::status_to_string(zombie_status status) {
    switch (status) {
    case zombie_status::walking: return "walking";
    case zombie_status::dying: return "dying";
    case zombie_status::dying_from_instant_kill:
        return "dying_from_instant_kill";
    case zombie_status::dying_from_lawnmower: return "dying_from_lawnmower";
    case zombie_status::bungee_target_drop: return "bungee_target_drop";
    case zombie_status::bungee_body_drop: return "bungee_body_drop";
    case zombie_status::bungee_idle_after_drop:
        return "bungee_idle_after_drop";
    case zombie_status::bungee_grab: return "bungee_grab";
    case zombie_status::bungee_raise: return "bungee_raise";
    case zombie_status::bungee_idle: return "bungee_idle";
    case zombie_status::pole_valuting_running: 
        return "pole_valuting_running";
    case zombie_status::pole_valuting_jumpping:
        return "pole_valuting_jumpping";
    case zombie_status::pole_vaulting_walking:
        return "pole_vaulting_walking";
    case zombie_status::rising_from_ground: return "rising_from_ground";
    case zombie_status::jackbox_walking: return "jackbox_walking";
    case zombie_status::jackbox_pop: return "jackbox_pop";
    case zombie_status::pogo_with_stick: return "pogo_with_stick";
    case zombie_status::pogo_idle_before_target:
        return "pogo_idle_before_target";
    case zombie_status::pogo_jump_across: return "pogo_jump_across";
    case zombie_status::newspaper_walking: return "newspaper_walking";
    case zombie_status::newspaper_destoryed: return "newspaper_destoryed";
    case zombie_status::newspaper_running: return "newspaper_running";
    case zombie_status::digger_dig: return "digger_dig";
    case zombie_status::digger_drill: return "digger_drill";
    case zombie_status::digger_lost_dig: return "digger_lost_dig";
    case zombie_status::digger_landing: return "digger_landing";
    case zombie_status::digger_dizzy: return "digger_dizzy";
    case zombie_status::digger_walk_right: return "digger_walk_right";
    case zombie_status::digger_walk_left: return "digger_walk_left";
    case zombie_status::digger_idle: return "digger_idle";
    case zombie_status::dancing_moonwalk: return "dancing_moonwalk";
    case zombie_status::dancing_point: return "dancing_point";
    case zombie_status::dancing_wait_summoning:
        return "dancing_wait_summoning";
    case zombie_status::dancing_summoning: return "dancing_summoning";
    case zombie_status::dancing_walking: return "dancing_walking";
    case zombie_status::dancing_armrise1: return "dancing_armrise1";
    case zombie_status::dancing_armrise2: return "dancing_armrise2";
    case zombie_status::dancing_armrise3: return "dancing_armrise3";
    case zombie_status::dancing_armrise4: return "dancing_armrise4";
    case zombie_status::dancing_armrise5: return "dancing_armrise5";
    case zombie_status::backup_spawning: return "backup_spawning";
    case zombie_status::dophin_walk_with_dophin: 
        return "dophin_walk_with_dophin";
    case zombie_status::dophin_jump_in_pool: return "dophin_jump_in_pool";
    case zombie_status::dophin_ride: return "dophin_ride";
    case zombie_status::dophin_jump: return "dophin_jump";
    case zombie_status::dophin_walk_in_pool: return "dophin_walk_in_pool";
    case zombie_status::dophin_walk_without_dophin: 
        return "dophin_walk_without_dophin";
    case zombie_status::snorkel_walking: return "snorkel_walking";
    case zombie_status::snorkel_jump_in_the_pool:
        return "snorkel_jump_in_the_pool";
    case zombie_status::snorkel_swim: return "snorkel_swim";
    case zombie_status::snorkel_up_to_eat: return "snorkel_up_to_eat";
    case zombie_status::snorkel_eat: return "snorkel_eat";
    case zombie_status::snorkel_finied_eat: return "snorkel_finied_eat";
    case zombie_status::catapult_shoot: return "catapult_shoot";
    case zombie_status::catapult_idle: return "catapult_idle";
    case zombie_status::balloon_flying: return "balloon_flying";
    case zombie_status::balloon_falling: return "balloon_falling";
    case zombie_status::balloon_walking: return "balloon_walking";
    case zombie_status::imp_flying: return "imp_flying";
    case zombie_status::imp_landing: return "imp_landing";
    case zombie_status::gargantuar_throw: return "gargantuar_throw";
    case zombie_status::gargantuar_smash: return "gargantuar_smash";
    case zombie_status::ladder_walking: return "ladder_walking";
    case zombie_status::ladder_placing: return "ladder_placing";
    case zombie_status::yeti_escape: return "yeti_escape";
    default:
        return nullptr;
    }
}

const char* zombie::type_to_string(zombie_type type) {
    switch (type) {
    case zombie_type::none: return "none";
    case zombie_type::zombie: return "zombie";
    case zombie_type::flag: return "flag";
    case zombie_type::conehead: return "conehead";
    case zombie_type::pole_vaulting: return "pole_vaulting";
    case zombie_type::buckethead: return "buckethead";
    case zombie_type::newspaper: return "newspaper";
    case zombie_type::screendoor: return "screendoor";
    case zombie_type::football: return "football";
    case zombie_type::dancing: return "dancing";
    case zombie_type::backup_dancer: return "backup_dancer";
    case zombie_type::ducky_tube: return "ducky_tube";
    case zombie_type::snorkel: return "snorkel";
    case zombie_type::zomboni: return "zomboni";
    case zombie_type::dolphin_rider: return "dolphin_rider";
    case zombie_type::jack_in_the_box: return "jack_in_the_box";
    case zombie_type::balloon: return "balloon";
    case zombie_type::digger: return "digger";
    case zombie_type::pogo: return "pogo";
    case zombie_type::yeti: return "yeti";
    case zombie_type::bungee: return "bungee";
    case zombie_type::ladder: return "ladder";
    case zombie_type::catapult: return "catapult";
    case zombie_type::gargantuar: return "gargantuar";
    case zombie_type::imp: return "imp";
    case zombie_type::giga_gargantuar: return "giga_gargantuar";
    default:
        return nullptr;
    }
}

const char* zombie::accessories_type_to_string(zombie_accessories_type_1 type)
{
    switch (type) {
    case zombie_accessories_type_1::none: return "none";
    case zombie_accessories_type_1::roadcone: return "roadcone";
    case zombie_accessories_type_1::bucket: return "bucket";
    case zombie_accessories_type_1::football_cap: return "football_cap";
    case zombie_accessories_type_1::miner_hat: return "miner_hat";
    default:
        return nullptr;
    }
}

const char* zombie::accessories_type_to_string(zombie_accessories_type_2 type)
{
    switch (type) {
    case zombie_accessories_type_2::none: return "none";
    case zombie_accessories_type_2::screen_door: return "screen_door";
    case zombie_accessories_type_2::newspaper: return "newspaper";
    case zombie_accessories_type_2::ladder: return "ladder";
    default:
        return nullptr;
    }
}


}