#pragma once
#include <vector>
#include <string>
#include "object/scene.h"
#include "system/sun.h"
#include "system/spawn.h"
#include "system/ice_path.h"
#include "system/plant/plant_factory.h"
#include "system/plant/plant_system.h"
#include "system/zombie/zombie_system.h"
#include "system/projectile/projectile_system.h"
#include "system/endgame.h"
#include "system/griditem.h"

namespace pvz_emulator {

class world
{
public:
	object::scene scene;

	system::sun sun;
	system::spawn spawn;
	system::ice_path ice_path;
	system::endgame endgame;
	system::griditem griditem;
	system::plant_system plant_system;
	system::plant_factory plant_factory;
	system::zombie_factory zombie_factory;
	system::griditem_factory griditem_factory;
	system::zombie_system zombie;
	system::projectile_system projectile;

private:
	void clean_obj_lists();

public:
	world(object::scene_type t):
		scene(t),
		sun(scene),
		spawn(scene),
		ice_path(scene),
		endgame(scene),
		griditem(scene),
		plant_system(scene),
		plant_factory(scene),
		griditem_factory(scene),
		zombie_factory(scene),
		zombie(scene),
		projectile(scene)
	{}

	bool update();

#ifdef PVZEMU_BUILD_DEBUGGER
	void to_json(std::string& s);
#endif // PVZEMU_BUILD_DEBUGGER

	bool select_plants(
		std::vector<object::plant_type> cards,
		object::plant_type imitater_type = object::plant_type::none);

	bool plant(unsigned int i, unsigned int row, unsigned int col) {
		return plant_factory.plant(i, row, col) != nullptr;
	}

	bool plant(object::plant_type type, unsigned int row, unsigned int col);

	void reset() {
		scene.reset();
		spawn.reset();
	}

	void reset(object::scene_type type) {
		scene.reset(type);
		spawn.reset();
	}
};

}
