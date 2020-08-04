#pragma once
#include <vector>
#include <tuple>
#include <string>
#include <atomic>
#include <thread>
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
		zombie_factory(scene),
		griditem_factory(scene),
		zombie(scene),
		projectile(scene)
	{}

	world(const world& w) :
		scene(w.scene),
		sun(scene),
		spawn(scene),
		ice_path(scene),
		endgame(scene),
		griditem(scene),
		plant_system(scene),
		plant_factory(scene),
		zombie_factory(scene),
		griditem_factory(scene),
		zombie(scene),
		projectile(scene)
	{}

	bool update();
	bool update(const std::tuple<int, int, int>& action);

    using action_vector = std::vector<std::tuple<int, int, int>>;

    using action_masks = std::vector<int>;
    using batch_action_masks = std::vector<action_masks>;

	void get_available_actions(
		const action_vector& actions,
		std::vector<int>& action_masks) const;

    using check_list = std::vector<std::tuple<
            object::plant_type,
            unsigned int,
            unsigned int>>;

    static void update_all(
		std::vector<world *>& v,
        const action_vector & all_actions,
        action_vector &actions,
		batch_action_masks &batch_action_masks,
        const check_list &build_check_list,
        std::vector<int>& check_result,
        std::vector<int>& done,
		unsigned int frames = 1);

	void to_json(std::string& s);

	bool select_plants(
		const std::vector<object::plant_type>& cards,
		object::plant_type imitater_type = object::plant_type::none);

	bool plant(unsigned int i, unsigned int row, unsigned int col) {
		return plant_factory.plant(i, row, col) != nullptr;
	}

	bool plant(object::plant_type type, unsigned int row, unsigned int col);

	bool check_build(const check_list &plants);

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
