#include <cassert>
#include <array>

#include "system/zombie/zombie_factory.h"
#include "system/plant/plant_factory.h"
#include "system/zombie/zombie.h"
#include "system/util.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

zombie& zombie_factory::create(zombie_type type, int row, int col)
{
	if (row == -1) {
		row = static_cast<int>(get_spawn_row(type));
	} else {
		row -= 1;
	}

	auto& z = scene.zombies.alloc();

	switch (type) {
    case zombie_type::zombie:
    case zombie_type::conehead:
    case zombie_type::buckethead:
    case zombie_type::screendoor:
    case zombie_type::flag:
    case zombie_type::ducky_tube:
		subsystems.zombie.init(z, type, row);
        break;

    case zombie_type::pole_vaulting:
		subsystems.pole_vaulting.init(z, row);
        break;

    case zombie_type::newspaper:
		subsystems.newspaper.init(z, row);
        break;

    case zombie_type::football:
		subsystems.football.init(z, row);
        break;

    case zombie_type::dancing:
		subsystems.dancing.init(z, row);
        break;

    case zombie_type::backup_dancer:
		subsystems.backup_dancer.init(z, row);
        break;

    case zombie_type::snorkel:
		subsystems.snorkel.init(z, row);
        break;

    case zombie_type::zomboni:
		subsystems.zomboni.init(z, row);
        break;

    case zombie_type::dolphin_rider:
		subsystems.dolphin_rider.init(z, row);
        break;

    case zombie_type::jack_in_the_box:
		subsystems.jack_in_the_box.init(z, row);
        break;

    case zombie_type::balloon:
		subsystems.balloon.init(z, row);
        break;

    case zombie_type::digger:
		subsystems.digger.init(z, row);
        break;

    case zombie_type::pogo:
		subsystems.pogo.init(z, row);
        break;

    case zombie_type::yeti:
		subsystems.yeti.init(z, row);
        break;

    case zombie_type::bungee:
		subsystems.bungee.init(z, row);
		break;

    case zombie_type::ladder:
		subsystems.ladder.init(z, row);
        break;

    case zombie_type::catapult:
		subsystems.catapult.init(z, row);
        break;

    case zombie_type::gargantuar:
		subsystems.gargantuar.init(z, zombie_type::gargantuar, row);
        break;

    case zombie_type::giga_gargantuar:
		subsystems.gargantuar.init(z, zombie_type::giga_gargantuar, row);
        break;

    case zombie_type::imp:
		subsystems.imp.init(z, row);
        break;

	default:
		assert(false);
	}

	if (col != -1) {
		z.x = 10 + col * 80;
		z.int_x = static_cast<int>(z.x);
	}

	return z;
}

void zombie_factory::create_roof_lurking(
	zombie_type type,
	unsigned int row,
	unsigned int col)
{
	auto& b = create(zombie_type::bungee);
	auto& z = create(type);

	b.bungee_col = col;
	b.row = row;
	b.x = static_cast<float>(80 * col + 40);
	b.y = zombie_init_y(scene.type, b, row);
	b.master_id = static_cast<int>(scene.zombies.get_index(z));
	reanim.set(b, zombie_reanim_name::anim_raise, reanim_type::once, 36);

	z.x = b.x - 15;
	z.row = row;
	z.y = zombie_init_y(scene.type, z, row);
	z.action = zombie_action::fall_from_sky;
	reanim.set(z, zombie_reanim_name::anim_idle, reanim_type::repeat, 0);
}

void zombie_factory::create_pool_or_night_lurking(
	zombie_type type,
	unsigned int row,
	unsigned int col)
{
	auto& z = create(type);
	
	z.x = static_cast<float>(80 * col + 15);
	z.y = zombie_init_y(scene.type, z, row);
	z.int_x = static_cast<int>(z.x);
	z.int_y = static_cast<int>(z.y);
	z.row = row;
	z.dy = static_cast<float>(scene.type == scene_type::night ? -200 : -150);
	z.status = zombie_status::rising_from_ground;
	z.is_in_water = scene.type != scene_type::night;
	z.countdown.action = scene.type == scene_type::night ? 150 : 50;

	if (scene.type != scene_type::night) {
		z.action = zombie_action::none;
	}
}

void zombie_factory::create_lurking(
	zombie_type type,
	unsigned int row,
	unsigned int col)
{
	switch (scene.type) {
	case scene_type::pool:
	case scene_type::fog:
		create_pool_or_night_lurking(type, row, col);
		break;

	case scene_type::roof:
	case scene_type::moon_night:
		create_roof_lurking(type, row, col);
		break;

    case scene_type::night:
		create_pool_or_night_lurking(type, row, col);
        break;

	default:
		break;
	}
}

void zombie_factory::destroy(object::zombie& z) {
	z.is_dead = true;

	if (z.type == zombie_type::bungee) {
        auto p = scene.plants.get(z.bungee_target);

		if (p != nullptr && z.status == zombie_status::bungee_grab) {
			if (p->edible == plant_edible_status::invisible_and_edible) {
				p->edible = plant_edible_status::visible_and_edible;
			}
			else if (p->edible == plant_edible_status::invisible_and_not_edible) {
				plant_factory(scene).destroy(*p);
			}

			z.bungee_target = -1;
		}

		if (auto p = scene.plants.get(z.bungee_target)) {
			plant_factory(scene).destroy(*p);
		}

		if (auto partner = scene.zombies.get(z.master_id)) {
			if (!partner->is_dead) {
				destroy(*partner);
			}
		}
	}
}

bool zombie_factory::can_spawn_at_row(zombie_type type, unsigned int row) {
	if (row >= scene.rows) {
		return false;
	}

	switch (type) {
	case zombie_type::zombie:
	case zombie_type::flag:
	case zombie_type::conehead:
	case zombie_type::buckethead:
	case zombie_type::balloon:
	case zombie_type::bungee:
		return true;

	case zombie_type::pole_vaulting:
	case zombie_type::newspaper:
	case zombie_type::screendoor:
	case zombie_type::football:
	case zombie_type::jack_in_the_box:
	case zombie_type::backup_dancer:
	case zombie_type::digger:
	case zombie_type::zomboni:
	case zombie_type::pogo:
	case zombie_type::yeti:
	case zombie_type::ladder:
	case zombie_type::catapult:
	case zombie_type::gargantuar:
	case zombie_type::imp:
	case zombie_type::giga_gargantuar:
		switch (scene.type) {
		case scene_type::fog:
		case scene_type::pool:
			return row == 0 || row == 1 || row == 4 || row == 5;
		default:
			return true;
		}

	case zombie_type::dancing:
		switch (scene.type) {
		case scene_type::day:
		case scene_type::night:
			return row == 1 || row == 2 || row == 3;

		case scene_type::pool:
		case scene_type::fog:
            return row == 0 || row == 1 || row == 4 || row == 5;

		default:
			return false;
		}

	case zombie_type::ducky_tube:
	case zombie_type::snorkel:
	case zombie_type::dolphin_rider:
		return (scene.type == scene_type::pool || scene.type == scene_type::fog) &&
			(row == 2 || row == 3);

	default:
		assert(false);
		return false;
	}
}

unsigned int zombie_factory::get_spawn_row(zombie_type type) {
	float sigma_b = 0;

	for (int i = 0; i < 6; i++) {
		if (can_spawn_at_row(type, i)) {
			sigma_b += data.row_random[i].b = 1;
		} else {
			data.row_random[i].b = 0;
		}
	}

	std::array<float, 6> f = { 0 };
	for (int i = 0; i < 6; i++) {
		if (data.row_random[i].b == 0) {
			f[i] = 0;
			continue;
		}

		f[i] = data.row_random[i].b *
			static_cast<float>(data.row_random[i].d + 1.5 * data.row_random[i].c + 2.5) /
			sigma_b;

		if (f[i] > 0.01) {
			f[i] = std::min(100.0f, f[i]);
		} else {
			f[i] = static_cast<float>(data.row_random[i].b / sigma_b * 0.01);
		}
	}

	auto row = rng.random_weighted_sample(f);

	for (int i = 0; i < 6; i++) {
		if (data.row_random[i].b > 0) {
			data.row_random[i].c++;
			data.row_random[i].d++;
		}
	}

	data.row_random[row].d = data.row_random[row].c;
	data.row_random[row].c = 0;

	return static_cast<unsigned int>(row);
}

}