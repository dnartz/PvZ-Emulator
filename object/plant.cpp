#include <cstring>

#include "scene.h"
#include "plant.h"
#include "system/plant/plant.h"

namespace pvz_emulator::object {

const std::array<unsigned int, 49> plant::EFFECT_INTERVAL_TABLE = {
    150, 2500, 0, 0, 0, 150, 0, 150,
    150, 2500, 150, 0, 0, 150, 0, 0,
    0, 0, 150, 0, 0, 0, 0, 0,
    150, 2500, 150, 0, 150, 150, 0, 0,
    300, 0, 300, 0, 0, 0, 2500, 300,
    150, 2500, 200, 150, 300, 0, 0, 600,
    0
};

const std::array<bool, 49> plant::CAN_ATTACK_TABLE = {
    true, false, false, false, false, true, false, true,
    true, false, true, false, false, true, false, false,
    false, false, true, false, false, false, false, false,
    true, false, true, false, true, true, false, false,
    true, false, true, false, false, false, false, true,
    true, false, true, true, true, false, false, false,
    false
};

const std::array<unsigned int, 48> plant::COST_TABLE = {
    100, 50, 150, 50, 25, 175, 150, 200,
    0, 25, 75, 75, 75, 25, 75, 125,
    25, 50, 325, 25, 125, 100, 175, 125,
    0, 25, 125, 100, 125, 125, 125, 100,
    100, 25, 100, 75, 50, 100, 50, 300,
    250, 150, 150, 225, 200, 50, 125, 500
};

const std::array<unsigned int, 48> plant::CD_TABLE = {
    750, 750, 5000, 3000, 3000, 750, 750, 750,
    750, 750, 750, 750, 3000, 750, 5000, 5000,
    750, 3000, 750, 3000, 5000, 750, 750, 3000,
    3000, 3000, 750, 750, 750, 750, 3000, 750,
    750, 750, 750, 750, 750, 750, 3000, 750,
    5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000
};

void plant::get_hit_box(rect& rect) {
    switch (type) {
    case plant_type::tallnut:
        rect.x = x + 10;
        rect.y = y;
        rect.width = attack_box.width;
        rect.height = attack_box.height;
        break;

    case plant_type::pumpkin:
        rect.x = x;
        rect.y = y;
        rect.width = attack_box.width - 20;
        rect.height = attack_box.height;
        break;

    case plant_type::cob_cannon:
        rect.x = x;
        rect.y = y;
        rect.width = 140;
        rect.height = 80;
        break;

    default:
        rect.x = x + 10;
        rect.y = y;
        rect.width = attack_box.width - 20;
        rect.height = attack_box.height;
    }
}

void plant::get_attack_box(rect& rect, bool is_alt_attack) {
    if (is_alt_attack && type == plant_type::split_pea) {
        rect.x = 0;
        rect.y = y;
        rect.width = x + 16;
        rect.height = attack_box.height;
        return;
    }

    switch (type) {
    case plant_type::squash:
        rect.x = x + 20;
        rect.y = y;
        rect.width = attack_box.width - 35;
        rect.height = attack_box.height;
        return;

    case plant_type::chomper:
        rect.x = x + 80;
        rect.y = y;
        rect.width = 40;
        rect.height = attack_box.height;
        return;

    case plant_type::spikeweed:
    case plant_type::spikerock:
        rect.x = x + 20;
        rect.y = y;
        rect.width = attack_box.width - 50;
        rect.height = attack_box.height;
        return;

    case plant_type::potato_mine:
        rect.x = x;
        rect.y = y;
        rect.width = attack_box.width - 25;
        rect.height = attack_box.height;
        return;

    case plant_type::torchwood:
        rect.x = x + 50;
        rect.y = y;
        rect.width = 30;
        rect.height = attack_box.height;
        return;

    case plant_type::puffshroom:
    case plant_type::seashroom:
        rect.x = x + 60;
        rect.y = y;
        rect.width = 230;
        rect.height = attack_box.height;
        return;

    case plant_type::fumeshroom:
        rect.x = x + 60;
        rect.y = y;
        rect.width = 340;
        rect.height = attack_box.height;
        return;

    case plant_type::gloomshroom:
        rect.x = x - 80;
        rect.y = y - 80;
        rect.width = 240;
        rect.height = 240;
        return;

    case plant_type::tangle_kelp:
        rect.x = x;
        rect.y = y;
        rect.width = attack_box.width;
        rect.height = attack_box.height;
        return;

    case plant_type::cattail:
        rect.x = -800;
        rect.y = -600;
        rect.width = 1600;
        rect.height = 1200;
        return;

    default:
        rect.x = x + 60;
        rect.y = y;
        rect.width = 800;
        rect.height = attack_box.height;
        break;
    }
}

unsigned int plant::get_attack_flags(bool is_alt_attack) {
    switch (type) {
    case plant_type::cactus:
        return is_alt_attack ?
            static_cast<unsigned int>(attack_flags::ground):
            static_cast<unsigned int>(attack_flags::flying_balloon);

    case plant_type::cob_cannon:
    case plant_type::cherry_bomb:
    case plant_type::jalapeno:
    case plant_type::doomshroom:
        return attack_flags::digging_digger |
            attack_flags::dying_zombies |
            attack_flags::animating_zombies |
            attack_flags::lurking_snorkel |
            attack_flags::flying_balloon |
            attack_flags::ground |
            0x8;

    case plant_type::squash:
    case plant_type::cabbagepult:
    case plant_type::melonpult:
    case plant_type::kernelpult:
    case plant_type::winter_melon:
        return attack_flags::lurking_snorkel | attack_flags::ground | 0x8;

    case plant_type::potato_mine:
        return attack_flags::digging_digger |
            attack_flags::lurking_snorkel |
            attack_flags::ground |
            0x8;

    case plant_type::puffshroom:
    case plant_type::scaredyshroom:
    case plant_type::fumeshroom:
    case plant_type::chomper:
    case plant_type::seashroom:
        return static_cast<unsigned int>(attack_flags::ground);

    case plant_type::cattail:
        return attack_flags::flying_balloon | attack_flags::ground;
        break;

    case plant_type::tangle_kelp:
        return attack_flags::lurking_snorkel |
            attack_flags::ground;

    default:
        return attack_flags::animating_zombies | attack_flags::ground;
    }
}

void plant::set_sleep(bool flag) {
    if (is_sleeping == flag ||
        is_squash_attacking() ||
        is_smashed ||
        edible == plant_edible_status::invisible_and_not_edible ||
        is_dead)
    {
        return;
    }

    is_sleeping = flag;

    if (is_sleeping) {
        if (has_reanim(plant_reanim_name::anim_sleep)) {
            set_reanim_frame(plant_reanim_name::anim_sleep);
        } else {
            reanim.fps = 1;
        }
    } else {
        if (has_reanim(plant_reanim_name::anim_idle)) {
            set_reanim_frame(plant_reanim_name::anim_idle);
        }
    }
}

void plant::set_reanim(plant_reanim_name name, reanim_type type, float fps) {
    if (fps != 0) {
        reanim.fps = fps;
    }

    reanim.type = type;
    reanim.n_repeated = 0;

    reanim.progress = static_cast<float>(reanim.fps >= 0 ? 0 : 0.99999988);
    reanim.prev_progress = -1;

    set_reanim_frame(name);
}

const char* plant::type_to_string(plant_type type) {
    switch (type) {
    case plant_type::none: return "none";
    case plant_type::pea_shooter: return "pea_shooter";
    case plant_type::sunflower: return "sunflower";
    case plant_type::cherry_bomb: return "cherry_bomb";
    case plant_type::wallnut: return "wallnut";
    case plant_type::potato_mine: return "potato_mine";
    case plant_type::snow_pea: return "snow_pea";
    case plant_type::chomper: return "chomper";
    case plant_type::repeater: return "repeater";
    case plant_type::puffshroom: return "puffshroom";
    case plant_type::sunshroom: return "sunshroom";
    case plant_type::fumeshroom: return "fumeshroom";
    case plant_type::grave_buster: return "grave_buster";
    case plant_type::hypnoshroom: return "hypnoshroom";
    case plant_type::scaredyshroom: return "scaredyshroom";
    case plant_type::iceshroom: return "iceshroom";
    case plant_type::doomshroom: return "doomshroom";
    case plant_type::lily_pad: return "lily_pad";
    case plant_type::squash: return "squash";
    case plant_type::threepeater: return "threepeater";
    case plant_type::tangle_kelp: return "tangle_kelp";
    case plant_type::jalapeno: return "jalapeno";
    case plant_type::spikeweed: return "spikeweed";
    case plant_type::torchwood: return "torchwood";
    case plant_type::tallnut: return "tallnut";
    case plant_type::seashroom: return "seashroom";
    case plant_type::plantern: return "plantern";
    case plant_type::cactus: return "cactus";
    case plant_type::blover: return "blover";
    case plant_type::split_pea: return "split_pea";
    case plant_type::starfruit: return "starfruit";
    case plant_type::pumpkin: return "pumpkin";
    case plant_type::magnetshroom: return "magnetshroom";
    case plant_type::cabbagepult: return "cabbagepult";
    case plant_type::flower_pot: return "flower_pot";
    case plant_type::kernelpult: return "kernelpult";
    case plant_type::coffee_bean: return "coffee_bean";
    case plant_type::garlic: return "garlic";
    case plant_type::umbrella_leaf: return "umbrella_leaf";
    case plant_type::marigold: return "marigold";
    case plant_type::melonpult: return "melonpult";
    case plant_type::gatling_pea: return "gatling_pea";
    case plant_type::twin_sunflower: return "twin_sunflower";
    case plant_type::gloomshroom: return "gloomshroom";
    case plant_type::cattail: return "cattail";
    case plant_type::winter_melon: return "winter_melon";
    case plant_type::gold_magnet: return "gold_magnet";
    case plant_type::spikerock: return "spikerock";
    case plant_type::cob_cannon: return "cob_cannon";
    case plant_type::imitater: return "imitater";
    default:
        return nullptr;
    }
}

void plant::to_json(
    scene& scene,
    rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartObject();

    writer.Key("id");
    writer.Uint64(scene.plants.get_index(*this));

    writer.Key("type");
    writer.String(type_to_string(type));

    writer.Key("status");
    writer.String(status_to_string(status));

    writer.Key("x");
    writer.Int(x);

    writer.Key("y");
    writer.Int(y);

    writer.Key("cannon");
    writer.StartObject();
    writer.Key("x");
    writer.Int(x);

    writer.Key("y");
    writer.Int(y);
    writer.EndObject();

    writer.Key("hp");
    writer.Int(hp);

    writer.Key("max_hp");
    writer.Int(max_hp);

    writer.Key("attack_box");
    rect rect;
    get_attack_box(rect);
    rect.to_json(writer);

    writer.Key("hit_box");
    get_hit_box(rect);
    rect.to_json(writer);

    writer.Key("row");
    writer.Uint(row);

    writer.Key("col");
    writer.Uint(col);

    writer.Key("max_boot_delay");
    writer.Uint(max_boot_delay);

    writer.Key("direction");
    writer.String(direction == plant_direction::left ? "left" : "right");

    writer.Key("target");
    if (target == -1) {
        writer.Null();
    } else {
        writer.Uint64(target);
    }

    writer.Key("imitater_target");
    writer.String(type_to_string(imitater_target));

    writer.Key("countdown");
    writer.StartObject();
    writer.Key("status");
    writer.Int(countdown.status);

    writer.Key("generate");
    writer.Int(countdown.generate);

    writer.Key("launch");
    writer.Int(countdown.launch);

    writer.Key("eaten");
    writer.Int(countdown.eaten);

    writer.Key("awake");
    writer.Int(countdown.awake);

    writer.Key("effect");
    writer.Int(countdown.effect);

    writer.Key("dead");
    writer.Int(countdown.dead);

    writer.Key("blover_disappear");
    writer.Int(countdown.blover_disappear);
    writer.EndObject();

    writer.Key("reanim");
    reanim.to_json(writer);

    writer.Key("edible");
    switch (edible) {
    case plant_edible_status::visible_and_edible:
        writer.String("visible_and_edible");
        break;

    case plant_edible_status::invisible_and_edible:
        writer.String("invisible_and_edible");
        break;

    case plant_edible_status::invisible_and_not_edible:
        writer.String("invisible_and_not_edible");
        break;

    default:
        writer.Null();
    }

    writer.Key("threepeater_time_since_first_shot");
    writer.Uint(threepeater_time_since_first_shot);

    writer.Key("split_pea_attack_flags");
    writer.StartObject();
    writer.Key("front");
    writer.Bool(split_pea_attack_flags.front);
    writer.Key("back");
    writer.Bool(split_pea_attack_flags.back);
    writer.EndObject();

    writer.Key("is_dead");
    writer.Bool(is_dead);
    writer.Key("is_smashed");
    writer.Bool(is_smashed);
    writer.Key("is_sleeping");
    writer.Bool(is_sleeping);
    writer.Key("can_attack");
    writer.Bool(can_attack);

    writer.EndObject();
}

const char* plant::status_to_string(plant_status status) {
    switch (status) {
    case plant_status::idle: return "idle";
    case plant_status::wait: return "wait";
    case plant_status::work: return "work";
    case plant_status::squash_look: return "squash_look";
    case plant_status::squash_jump_up: return "squash_jump_up";
    case plant_status::squash_stop_in_the_air:
        return "squash_stop_in_the_air";
    case plant_status::squash_jump_down: return "squash_jump_down";
    case plant_status::squash_crushed: return "squash_crushed";
    case plant_status::grave_buster_land: return "grave_buster_land";
    case plant_status::grave_buster_idle: return "grave_buster_idle";
    case plant_status::chomper_bite_begin: return "chomper_bite_begin";
    case plant_status::chomper_bite_success: return "chomper_bite_success";
    case plant_status::chomper_bite_fail: return "chomper_bite_fail";
    case plant_status::chomper_chew: return "chomper_chew";
    case plant_status::chomper_swallow: return "chomper_swallow";
    case plant_status::potato_sprout_out: return "potato_sprout_out";
    case plant_status::potato_armed: return "potato_armed";
    case plant_status::spike_attack: return "spike_attack";
    case plant_status::scaredyshroom_scared: return "scaredyshroom_scared";
    case plant_status::scaredyshroom_scared_idle:
        return "scaredyshroom_scared_idle";
    case plant_status::scaredyshroom_grow: return "scaredyshroom_grow";
    case plant_status::sunshroom_small: return "sunshroom_small";
    case plant_status::sunshroom_grow: return "sunshroom_grow";
    case plant_status::sunshroom_big: return "sunshroom_big";
    case plant_status::magnetshroom_working: return "magnetshroom_working";
    case plant_status::magnetshroom_inactive_idle:
        return "magnetshroom_inactive_idle";
    case plant_status::cactus_short_idle: return "cactus_short_idle";
    case plant_status::cactus_grow_tall: return "cactus_grow_tall";
    case plant_status::cactus_tall_idle: return "cactus_tall_idle";
    case plant_status::cactus_get_short: return "cactus_get_short";
    case plant_status::tangle_kelp_grab: return "tangle_kelp_grab";
    case plant_status::cob_cannon_unaramed_idle:
        return "cob_cannon_unaramed_idle";
    case plant_status::cob_cannon_charge: return "cob_cannon_charge";
    case plant_status::cob_cannon_launch: return "cob_cannon_launch";
    case plant_status::cob_cannon_aramed_idle:
        return "cob_cannon_aramed_idle";
    case plant_status::kernelpult_launch_butter:
        return "kernelpult_launch_butter";
    case plant_status::umbrella_leaf_block: return "umbrella_leaf_block";
    case plant_status::umbrella_leaf_shrink: return "umbrella_leaf_shrink";
    case plant_status::imitater_explode: return "imitater_explode";
    case plant_status::flower_pot_placed: return "flower_pot_placed";
    case plant_status::lily_pad_placed: return "lily_pad_placed";
    default:
        return nullptr;
    }
}

}