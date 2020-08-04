#include "common.h"
#include "projectile.h"
#include "zombie.h"
#include "system/damage.h"

namespace pvz_emulator::object {

const unsigned int projectile::DAMAGE[14] = {
    20, 20, 40, 80, 20, 80, 40, 20, 20, 75, 20, 300, 40
};

void projectile::get_attack_box(rect& r) const {
	switch (type) {
    case projectile_type::pea:
    case projectile_type::snow_pea:
        r.x = int_x - 15;
        r.y = int_y;
        r.width = attack_box.width + 15;
        r.height = attack_box.height;
        break;

    case projectile_type::wintermelon:
    case projectile_type::melon:
        r.x = int_x + 20;
        r.y = int_y;
        r.width = 60;
        r.height = attack_box.height;
        break;

    case projectile_type::fire_pea:
        r.x = int_x;
        r.y = int_y;
        r.height = attack_box.height;
        r.width = attack_box.width - 10;
        break;

    case projectile_type::cactus:
        r.x = int_x - 25;
        r.y = int_y;
        r.width = attack_box.width + 25;
        r.height = attack_box.height;
        break;

    case projectile_type::cob_cannon:
        r.x = attack_box.width / 2 + int_x - 115;
        r.y = attack_box.height / 2 + int_y - 115;
        r.height = 230;
        r.width = 230;
        break;

    default:
        r.x = int_x;
        r.y = int_y;
        r.width = attack_box.width;
        r.height = attack_box.height;
        break;
	}
}

using pvz_emulator::system::zombie_damage_flags;

unsigned int projectile::get_flags_with_zombie(const zombie& z) const {
    unsigned int flags = 0;

    if (type == projectile_type::fire_pea &&
        (z.type == zombie_type::catapult ||
        z.type == zombie_type::zomboni ||
        z.accessory_2.type == zombie_accessories_type_2::screen_door ||
        z.accessory_2.type == zombie_accessories_type_2::ladder) &&
        (motion_type == projectile_motion_type::parabola ||
        motion_type == projectile_motion_type::left_straight ||
        motion_type == projectile_motion_type::starfruit))
    {
        flags = static_cast<unsigned int>(zombie_damage_flags::ignore_accessory_2);
    } else {
        flags = static_cast<unsigned int>(zombie_damage_flags::not_reduce);
    }


    if (type == projectile_type::snow_pea ||
        type == projectile_type::wintermelon)
    {
        flags = flags | zombie_damage_flags::slow_effect;
    }

    return flags;
}

void projectile::to_json(
    scene& scene,
    rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartObject();
    writer.Key("id");
    writer.Uint64(scene.projectiles.get_index(*this));

    writer.Key("type");
    writer.String(type_to_string(type));

    writer.Key("motion_type");
    writer.String(motion_type_to_string(motion_type));

    writer.Key("int_x");
    writer.Int(int_x);

    writer.Key("int_y");
    writer.Int(int_y);

    writer.Key("row");
    writer.Int(row);

    writer.Key("y");
    writer.Double(y);

    writer.Key("x");
    writer.Double(x);

    writer.Key("shadow_y");
    writer.Double(shadow_y);

    writer.Key("dx");
    writer.Double(dx);

    writer.Key("dy1");
    writer.Double(dy1);

    writer.Key("dy2");
    writer.Double(dy2);

    writer.Key("ddy");
    writer.Double(ddy);

    writer.Key("dddy");
    writer.Double(dddy);

    writer.Key("attack_box");
    rect rect;
    get_attack_box(rect);
    rect.to_json(writer);

    writer.Key("flags");
    writer.StartObject();
    writer.Key("disable_ballon_pop");
    writer.Bool(flags & zombie_damage_flags::disable_ballon_pop);

    writer.Key("ignore_accessory_2");
    writer.Bool(flags & zombie_damage_flags::ignore_accessory_2);

    writer.Key("not_reduce");
    writer.Bool(flags & zombie_damage_flags::not_reduce);

    writer.Key("no_flash");
    writer.Bool(flags & zombie_damage_flags::no_flash);

    writer.Key("slow_effect");
    writer.Bool(flags & zombie_damage_flags::slow_effect);

    writer.Key("spike");
    writer.Bool(flags & zombie_damage_flags::spike);
    writer.EndObject();

    writer.Key("time_since_created");
    writer.Uint(time_since_created);

    writer.Key("countdown");
    writer.Uint(countdown);

    writer.Key("last_torchwood_col");
    writer.Int(last_torchwood_col);

    writer.Key("cannon_x");
    writer.Double(cannon_x);

    writer.Key("cannon_row");
    writer.Int(cannon_row);

    writer.Key("target");
    if (target == -1) {
        writer.Null();
    } else {
        writer.Uint64(target);
    }

    writer.Key("is_visible");
    writer.Bool(is_visible);

    writer.Key("is_disappeared");
    writer.Bool(is_disappeared);

    writer.EndObject();
}

const char* projectile::type_to_string(projectile_type type) {
    switch (type) {
    case projectile_type::none: return "none";
    case projectile_type::pea: return "pea";
    case projectile_type::snow_pea: return "snow_pea";
    case projectile_type::cabbage: return "cabbage";
    case projectile_type::melon: return "melon";
    case projectile_type::puff: return "puff";
    case projectile_type::wintermelon: return "wintermelon";
    case projectile_type::fire_pea: return "fire_pea";
    case projectile_type::star: return "star";
    case projectile_type::cactus: return "cactus";
    case projectile_type::basketball: return "basketball";
    case projectile_type::kernel: return "kernel";
    case projectile_type::cob_cannon: return "cob_cannon";
    case projectile_type::butter: return "butter";
    default:
        return nullptr;
    }
}

const char* projectile::motion_type_to_string(projectile_motion_type type)
{
    switch (type) {
    case projectile_motion_type::straight: return "straight";
    case projectile_motion_type::parabola: return "parabola";
    case projectile_motion_type::switch_way: return "switch_way";
    case projectile_motion_type::puff: return "puff";
    case projectile_motion_type::left_straight: return "left_straight";
    case projectile_motion_type::starfruit: return "starfruit";
    case projectile_motion_type::cattail: return "cattail";
    default:
        return nullptr;
    }
}

}
