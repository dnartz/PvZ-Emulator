#include "scene.h"

namespace pvz_emulator::object {

scene::scene(const scene& s) :
    type(s.type),
    rng(s.rng),
    zombie_dancing_clock(s.zombie_dancing_clock),
    rows(s.rows),
    zombies(s.zombies),
    plants(s.plants),
    griditems(s.griditems),
    projectiles(s.projectiles),
    spawn(s.spawn),
    sun(s.sun),
    ice_path(s.ice_path),
    cards(s.cards),
    is_game_over(s.is_game_over),
    is_zombie_dance(s.is_zombie_dance),
    is_future_enabled(s.is_future_enabled),
    stop_spawn(s.stop_spawn),
    enable_split_pea_bug(s.enable_split_pea_bug)
{
    memset(&plant_map, 0, sizeof(plant_map));

    for (auto& p : plants) {
        if (p.type == plant_type::flower_pot ||
            p.type == plant_type::lily_pad)
        {
            plant_map[p.row][p.col].base = &p;
        } else if (p.type == plant_type::coffee_bean) {
            plant_map[p.row][p.col].coffee_bean = &p;
        } else if (p.type == plant_type::pumpkin) {
            plant_map[p.row][p.col].pumpkin = &p;
        } else {
            plant_map[p.row][p.col].content = &p;
        }
    }
}

void scene::to_json(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
    writer.StartObject();

    writer.Key("type");

    switch (type) {
    case scene_type::day:
        writer.String("day");
        break;

    case scene_type::night:
        writer.String("night");
        break;

    case scene_type::pool:
        writer.String("pool");
        break;

    case scene_type::fog:
        writer.String("fog");
        break;

    case scene_type::roof:
        writer.String("roof");
        break;

    case scene_type::moon_night:
        writer.String("moon_night");
        break;

    default:
        break;
    }

    writer.Key("rows");
    writer.Uint(rows);

    writer.Key("zombies");
    writer.StartArray();
    for (auto& z : zombies) {
        z.to_json(*this, writer);
    }
    writer.EndArray();

    writer.Key("plants");
    writer.StartArray();
    for (auto& p : plants) {
        p.to_json(*this, writer);
    }
    writer.EndArray();

    writer.Key("griditems");
    writer.StartArray();
    for (auto& item : griditems) {
        item.to_json(*this, writer);
    }
    writer.EndArray();

    writer.Key("projectiles");
    writer.StartArray();
    for (auto& proj : projectiles) {
        proj.to_json(*this, writer);
    }
    writer.EndArray();

    writer.Key("spawn");
    writer.StartObject();

    writer.Key("spawn_list");
    writer.StartArray();
    for (auto& wave : spawn.spawn_list) {
        writer.StartArray();
        for (auto& type : wave) {
            writer.String(zombie::type_to_string(type));
        }
        writer.EndArray();
    }
    writer.EndArray();

    writer.Key("total_flags");
    writer.Uint(spawn.total_flags);

    writer.Key("wave");
    writer.Uint(spawn.wave);

    writer.Key("hp");
    writer.StartObject();

    writer.Key("initial");
    writer.Uint(spawn.hp.initial);

    writer.Key("threshold");
    writer.Uint(spawn.hp.threshold);
    writer.EndObject();

    writer.Key("countdown");
    writer.StartObject();

    writer.Key("next_wave");
    writer.Uint(spawn.countdown.next_wave);

    writer.Key("next_wave_initial");
    writer.Uint(spawn.countdown.next_wave_initial);

    writer.Key("lurking_squad");
    writer.Uint(spawn.countdown.lurking_squad);

    writer.Key("hugewave_fade");
    writer.Uint(spawn.countdown.hugewave_fade);

    writer.Key("endgame");
    writer.Uint(spawn.countdown.hugewave_fade);

    writer.Key("pool");
    writer.Uint(spawn.countdown.pool);
    writer.EndObject();

    static const zombie_type ALL_ZOMBIES[] = {
        zombie_type::zombie,
        zombie_type::flag,
        zombie_type::conehead,
        zombie_type::pole_vaulting,
        zombie_type::buckethead,
        zombie_type::newspaper,
        zombie_type::screendoor,
        zombie_type::football,
        zombie_type::dancing,
        zombie_type::backup_dancer,
        zombie_type::ducky_tube,
        zombie_type::snorkel,
        zombie_type::zomboni,
        zombie_type::dolphin_rider,
        zombie_type::jack_in_the_box,
        zombie_type::balloon,
        zombie_type::digger,
        zombie_type::pogo,
        zombie_type::yeti,
        zombie_type::bungee,
        zombie_type::ladder,
        zombie_type::catapult,
        zombie_type::gargantuar,
        zombie_type::imp,
        zombie_type::giga_gargantuar,
    };

    writer.Key("spawn_flags");
    writer.StartObject();
    for (auto& type : ALL_ZOMBIES) {
        writer.Key(zombie::type_to_string(type));
        writer.Bool(spawn.spawn_flags[static_cast<int>(type)]);
    }
    writer.EndObject();

    writer.Key("is_hugewave_shown");
    writer.Bool(spawn.is_hugewave_shown);
    writer.EndObject();

    writer.Key("sun");
    writer.StartObject();

    writer.Key("sun");
    writer.Uint(sun.sun);

    writer.Key("natural_sun_generated");
    writer.Uint(sun.natural_sun_generated);

    writer.Key("natural_sun_countdown");
    writer.Uint(sun.natural_sun_countdown);
    writer.EndObject();

    writer.Key("ice_path");
    writer.StartObject();

    writer.Key("countdown");
    writer.StartArray();
    for (auto& t : ice_path.countdown) {
        writer.Uint(t);
    }
    writer.EndArray();

    writer.Key("x");
    writer.StartArray();
    for (auto& x : ice_path.x) {
        writer.Uint(x);
    }
    writer.EndArray();
    writer.EndObject();

    writer.Key("cards");
    writer.StartArray();
    for (auto& card : cards) {
        writer.StartObject();
        writer.Key("type");
        writer.String(plant::type_to_string(card.type));
        
        writer.Key("imitater_type");
        writer.String(plant::type_to_string(card.imitater_type));

        writer.Key("cold_down");
        writer.Uint(card.cold_down);
        writer.EndObject();
    }
    writer.EndArray();

    writer.Key("is_zombie_dance");
    writer.Bool(is_zombie_dance);

    writer.Key("is_future_enabled");
    writer.Bool(is_future_enabled);

    writer.Key("is_game_over");
    writer.Bool(is_game_over);

    writer.Key("stop_spawn");
    writer.Bool(stop_spawn);

    writer.EndObject();
}

void scene::reset() {
    rng = std::mt19937(std::random_device()());

    zombie_dancing_clock = rng() % 10000;
    is_zombie_dance = false;
    is_future_enabled = false;
    is_game_over = false;
    stop_spawn = false;
    enable_split_pea_bug = true;

    zombies.clear();
    plants.clear();
    griditems.clear();
    projectiles.clear();

    memset(&spawn, 0, sizeof(spawn));
    spawn.total_flags = 1000;
    spawn.countdown.next_wave = 600;
    spawn.countdown.next_wave_initial = 600;

    memset(&plant_map, 0, sizeof(plant_map));

    sun.sun = 9990;
    sun.natural_sun_generated = 0;
    sun.natural_sun_countdown = 0;

    memset(&ice_path.countdown, 0, sizeof(ice_path.countdown));
    for (auto& p : ice_path.x) {
        p = 800;
    }

    for (auto& card : cards) {
        card.type = plant_type::none;
        card.imitater_type = plant_type::none;
        card.cold_down = 0;
    }
}

}
