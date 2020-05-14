#include "scene.h"

namespace pvz_emulator::object {

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

}
