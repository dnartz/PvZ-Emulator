#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "world.h"

namespace py = pybind11;

using namespace pvz_emulator;
using namespace pvz_emulator::object;

PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(world::batch_action_vector);

PYBIND11_MODULE(pvzemu, m) {
    py::class_<std::vector<int>>(m, "IntVector")
        .def(py::init<>())
        .def("__getitem__", [](const std::vector<int>& v, std::vector<int>::size_type i) {
            return v[i];
        }).def("__len__", [](const std::vector<int>& v) { return v.size(); })
        .def("__iter__", [](std::vector<int>& v) {
            return py::make_iterator(v.begin(), v.end());
        }, py::keep_alive<0, 1>());

    py::class_<world::action_vector>(m, "ActionVector")
        .def(py::init<>())
        .def("__getitem__", [](
            world::action_vector& v,
            world::action_vector::size_type i) -> std::tuple<int, int, int>&
        {
            return v[i];
        }).def("__len__", [](const world::action_vector& v) { return v.size(); })
        .def("__iter__", [](world::action_vector& v) {
            return py::make_iterator(v.begin(), v.end());
        }, py::keep_alive<0, 1>());

    py::class_<world::batch_action_vector>(m, "BatchActionVector")
        .def(py::init<>())
        .def(py::init([](py::list &list) {
            auto v = new world::batch_action_vector();

            for (auto& actions : list) {
                v->emplace_back();
                for (auto& t : actions.cast<py::list>()) {
                    v->back().emplace_back(
                        t.cast<py::tuple>()[0].cast<int>(),
                        t.cast<py::tuple>()[1].cast<int>(),
                        t.cast<py::tuple>()[2].cast<int>());
                }
            }

            return v;
        }), py::return_value_policy::take_ownership)
        .def("clear", &world::batch_action_vector::clear)
        .def("resize",
            (void (world::batch_action_vector::*)(world::batch_action_vector::size_type))
            &world::batch_action_vector::resize)
        .def("__getitem__", [](
            world::batch_action_vector& v,
            world::batch_action_vector::size_type i) -> world::action_vector&
        {
            return v[i];
        })
        .def("__len__", [](const world::batch_action_vector& v) { return v.size(); })
        .def("__iter__", [](world::batch_action_vector& v) {
            return py::make_iterator(v.begin(), v.end());
        }, py::keep_alive<0, 1>());

    py::class_<world>(m, "World")
        .def_readwrite("scene", &world::scene)
        .def_readonly("sun", &world::sun)
        .def_readonly("spawn", &world::spawn)
        .def_readonly("ice_path", &world::ice_path)
        .def_readonly("endgame", &world::endgame)
        .def_readonly("griditem", &world::griditem)
        .def_readonly("plant_system", &world::plant_system)
        .def_readonly("plant_factory", &world::plant_factory)
        .def_readonly("zombie_factory", &world::zombie_factory)
        .def_readonly("griditem_factory", &world::griditem_factory)
        .def_readonly("zombie", &world::zombie)
        .def_readonly("projectile", &world::projectile)
        .def("update", (bool (world::*)(void)) & world::update)
        .def_static("update_all", &world::update_all)
        .def(py::init<scene_type>())
        .def("select_plants", &world::select_plants)
        .def("plant",
            (bool (world::*)(unsigned int, unsigned int, unsigned int)) & world::plant)
        .def("plant",
            (bool (world::*)(plant_type, unsigned int, unsigned int)) & world::plant)
        .def("reset", (void (world::*)(void)) & world::reset)
        .def("reset", (void (world::*)(scene_type)) & world::reset);

    py::enum_<scene_type>(m, "SceneType")
        .value("day", scene_type::day)
        .value("night", scene_type::night)
        .value("pool", scene_type::pool)
        .value("fog", scene_type::fog)
        .value("roof", scene_type::roof)
        .value("moon_night", scene_type::moon_night);

    py::enum_<plant_type>(m, "PlantType")
        .value("none", plant_type::none)
        .value("pea_shooter", plant_type::pea_shooter)
        .value("sunflower", plant_type::sunflower)
        .value("cherry_bomb", plant_type::cherry_bomb)
        .value("wallnut", plant_type::wallnut)
        .value("potato_mine", plant_type::potato_mine)
        .value("snow_pea", plant_type::snow_pea)
        .value("chomper", plant_type::chomper)
        .value("repeater", plant_type::repeater)
        .value("puffshroom", plant_type::puffshroom)
        .value("sunshroom", plant_type::sunshroom)
        .value("fumeshroom", plant_type::fumeshroom)
        .value("grave_buster", plant_type::grave_buster)
        .value("hypnoshroom", plant_type::hypnoshroom)
        .value("scaredyshroom", plant_type::scaredyshroom)
        .value("iceshroom", plant_type::iceshroom)
        .value("doomshroom", plant_type::doomshroom)
        .value("lily_pad", plant_type::lily_pad)
        .value("squash", plant_type::squash)
        .value("threepeater", plant_type::threepeater)
        .value("tangle_kelp", plant_type::tangle_kelp)
        .value("jalapeno", plant_type::jalapeno)
        .value("spikeweed", plant_type::spikeweed)
        .value("torchwood", plant_type::torchwood)
        .value("tallnut", plant_type::tallnut)
        .value("seashroom", plant_type::seashroom)
        .value("plantern", plant_type::plantern)
        .value("cactus", plant_type::cactus)
        .value("blover", plant_type::blover)
        .value("split_pea", plant_type::split_pea)
        .value("starfruit", plant_type::starfruit)
        .value("pumpkin", plant_type::pumpkin)
        .value("magnetshroom", plant_type::magnetshroom)
        .value("cabbagepult", plant_type::cabbagepult)
        .value("flower_pot", plant_type::flower_pot)
        .value("kernelpult", plant_type::kernelpult)
        .value("coffee_bean", plant_type::coffee_bean)
        .value("garlic", plant_type::garlic)
        .value("umbrella_leaf", plant_type::umbrella_leaf)
        .value("marigold", plant_type::marigold)
        .value("melonpult", plant_type::melonpult)
        .value("gatling_pea", plant_type::gatling_pea)
        .value("twin_sunflower", plant_type::twin_sunflower)
        .value("gloomshroom", plant_type::gloomshroom)
        .value("cattail", plant_type::cattail)
        .value("winter_melon", plant_type::winter_melon)
        .value("gold_magnet", plant_type::gold_magnet)
        .value("spikerock", plant_type::spikerock)
        .value("cob_cannon", plant_type::cob_cannon)
        .value("imitater", plant_type::imitater);

    py::enum_<zombie_type>(m, "ZombieType")
        .value("none", zombie_type::none)
        .value("zombie", zombie_type::zombie)
        .value("flag", zombie_type::flag)
        .value("conehead", zombie_type::conehead)
        .value("pole_vaulting", zombie_type::pole_vaulting)
        .value("buckethead", zombie_type::buckethead)
        .value("newspaper", zombie_type::newspaper)
        .value("screendoor", zombie_type::screendoor)
        .value("football", zombie_type::football)
        .value("dancing", zombie_type::dancing)
        .value("backup_dancer", zombie_type::backup_dancer)
        .value("ducky_tube", zombie_type::ducky_tube)
        .value("snorkel", zombie_type::snorkel)
        .value("zomboni", zombie_type::zomboni)
        .value("dolphin_rider", zombie_type::dolphin_rider)
        .value("jack_in_the_box", zombie_type::jack_in_the_box)
        .value("balloon", zombie_type::balloon)
        .value("digger", zombie_type::digger)
        .value("pogo", zombie_type::pogo)
        .value("yeti", zombie_type::yeti)
        .value("bungee", zombie_type::bungee)
        .value("ladder", zombie_type::ladder)
        .value("catapult", zombie_type::catapult)
        .value("gargantuar", zombie_type::gargantuar)
        .value("imp", zombie_type::imp)
        .value("giga_gargantuar", zombie_type::giga_gargantuar);

    py::class_<grid_plant_status>(m, "GridPlantStatus")
        .def_readonly("pumpkin", &grid_plant_status::pumpkin)
        .def_readonly("base", &grid_plant_status::base)
        .def_readonly("content", &grid_plant_status::content)
        .def_readonly("coffee_bean", &grid_plant_status::coffee_bean);

    py::class_<scene::sun_data>(m, "SunData")
        .def_readonly("sun", &scene::sun_data::sun)
        .def_readonly("natural_sun_countdown", &scene::sun_data::natural_sun_countdown)
        .def_readonly("natural_sun_generated", &scene::sun_data::natural_sun_generated);

    py::class_<scene>(m, "Scene")
        .def_readonly("type", &scene::type)
        .def_readonly("zombie_dancing_clock", &scene::zombie_dancing_clock)
        .def_readonly("rows", &scene::rows)
        .def_readonly("zombies", &scene::zombies)
        .def_readonly("plants", &scene::plants)
        .def_readonly("griditems", &scene::griditems)
        .def_readonly("projectiles", &scene::projectiles)
        .def_readonly("plant_map", &scene::plant_map)
        .def_readonly("spawn", &scene::spawn)
        .def_readonly("sun", &scene::sun)
        .def_readonly("ice_path", &scene::ice_path)
        .def_readonly("cards", &scene::cards)
        .def_readonly("is_game_over", &scene::is_game_over)
        .def_readwrite("is_zombie_dance", &scene::is_zombie_dance)
        .def_readwrite("is_future_enabled", &scene::is_future_enabled)
        .def_readwrite("stop_spawn", &scene::stop_spawn)
        .def_readwrite("enable_split_pea_bug", &scene::enable_split_pea_bug)
        .def(py::init<scene_type>())
        .def("is_water_grid", &scene::is_water_grid)
        .def("get_max_row", &scene::get_max_row)
        .def("reset", (void (scene::*)(void)) & scene::reset)
        .def("reset", (void (scene::*)(scene_type)) & scene::reset);

    py::class_<decltype(scene::zombies)>(m, "ZombieList")
        .def("__iter__", [](decltype(scene::zombies) &s) {
            return py::make_iterator(s.begin(), s.end());
        }, py::keep_alive<0, 1>())
        .def("__len__", &decltype(scene::zombies)::size);

    py::class_<decltype(scene::plants)>(m, "PlantList")
        .def("__iter__", [](decltype(scene::plants) &s) {
            return py::make_iterator(s.begin(), s.end());
        }, py::keep_alive<0, 1>())
        .def("__len__", &decltype(scene::plants)::size);

    py::class_<decltype(scene::griditems)>(m, "GriditemList")
        .def("__iter__", [](decltype(scene::griditems) &s) {
            return py::make_iterator(s.begin(), s.end());
        }, py::keep_alive<0, 1>())
        .def("__len__", &decltype(scene::griditems)::size);

    py::class_<decltype(scene::projectiles)>(m, "ProjectileList")
        .def("__iter__", [](decltype(scene::projectiles) &s) {
            return py::make_iterator(s.begin(), s.end());
        }, py::keep_alive<0, 1>())
        .def("__len__", &decltype(scene::projectiles)::size);

    py::class_<scene::card_data>(m, "Card")
        .def_readonly("type", &scene::card_data::type)
        .def_readonly("imitater_type", &scene::card_data::imitater_type)
        .def_readonly("cold_down", &scene::card_data::cold_down);

    py::class_<scene::spawn_data>(m, "SpawnData")
        .def_readonly("spawn_list", &scene::spawn_data::spawn_list)
        .def_readonly("total_flags", &scene::spawn_data::total_flags)
        .def_readonly("wave", &scene::spawn_data::wave)
        .def_readonly("hp", &scene::spawn_data::hp)
        .def_readonly("countdown", &scene::spawn_data::countdown)
        .def_readonly("spawn_flags", &scene::spawn_data::spawn_flags)
        .def_readonly("is_hugewave_shown", &scene::spawn_data::is_hugewave_shown)
        .def(py::init<>());

    py::class_<decltype(scene::spawn_data::hp)>(m, "SpawnDataHp")
        .def_readonly("initial", &decltype(scene::spawn_data::hp)::initial)
        .def_readonly("threshold", &decltype(scene::spawn_data::hp)::threshold);

    py::class_<decltype(scene::spawn_data::countdown)>(m, "SpawnDataCountdown")
        .def_readonly("next_wave", &decltype(scene::spawn_data::countdown)::next_wave)
        .def_readonly(
            "next_wave_initial",
            &decltype(scene::spawn_data::countdown)::next_wave_initial)
        .def_readonly(
            "lurking_squad",
            &decltype(scene::spawn_data::countdown)::lurking_squad)
        .def_readonly(
            "hugewave_fade",
            &decltype(scene::spawn_data::countdown)::hugewave_fade)
        .def_readonly(
            "endgame",
            &decltype(scene::spawn_data::countdown)::endgame)
        .def_readonly("pool", &decltype(scene::spawn_data::countdown)::pool);

    py::class_<scene::ice_path_data>(m, "IcePathData")
        .def_readonly("countdown", &scene::ice_path_data::countdown)
        .def_readonly("x", &scene::ice_path_data::x);

    py::enum_<zombie_status>(m, "ZombieStatus")
        .value("walking", zombie_status::walking)
        .value("dying", zombie_status::dying)
        .value("dying_from_instant_kill", zombie_status::dying_from_instant_kill)
        .value("dying_from_lawnmower", zombie_status::dying_from_lawnmower)
        .value("bungee_target_drop", zombie_status::bungee_target_drop)
        .value("bungee_body_drop", zombie_status::bungee_body_drop)
        .value("bungee_idle_after_drop", zombie_status::bungee_idle_after_drop)
        .value("bungee_grab", zombie_status::bungee_grab)
        .value("bungee_raise", zombie_status::bungee_raise)
        .value("bungee_idle", zombie_status::bungee_idle)
        .value("pole_valuting_running", zombie_status::pole_valuting_running)
        .value("pole_valuting_jumpping", zombie_status::pole_valuting_jumpping)
        .value("pole_vaulting_walking", zombie_status::pole_vaulting_walking)
        .value("rising_from_ground", zombie_status::rising_from_ground)
        .value("jackbox_walking", zombie_status::jackbox_walking)
        .value("jackbox_pop", zombie_status::jackbox_pop)
        .value("pogo_with_stick", zombie_status::pogo_with_stick)
        .value("pogo_idle_before_target", zombie_status::pogo_idle_before_target)
        .value("pogo_jump_across", zombie_status::pogo_jump_across)
        .value("newspaper_walking", zombie_status::newspaper_walking)
        .value("newspaper_destoryed", zombie_status::newspaper_destoryed)
        .value("newspaper_running", zombie_status::newspaper_running)
        .value("digger_dig", zombie_status::digger_dig)
        .value("digger_drill", zombie_status::digger_drill)
        .value("digger_lost_dig", zombie_status::digger_lost_dig)
        .value("digger_landing", zombie_status::digger_landing)
        .value("digger_dizzy", zombie_status::digger_dizzy)
        .value("digger_walk_right", zombie_status::digger_walk_right)
        .value("digger_walk_left", zombie_status::digger_walk_left)
        .value("digger_idle", zombie_status::digger_idle)
        .value("dancing_moonwalk", zombie_status::dancing_moonwalk)
        .value("dancing_point", zombie_status::dancing_point)
        .value("dancing_wait_summoning", zombie_status::dancing_wait_summoning)
        .value("dancing_summoning", zombie_status::dancing_summoning)
        .value("dancing_walking", zombie_status::dancing_walking)
        .value("dancing_armrise1", zombie_status::dancing_armrise1)
        .value("dancing_armrise2", zombie_status::dancing_armrise2)
        .value("dancing_armrise3", zombie_status::dancing_armrise3)
        .value("dancing_armrise4", zombie_status::dancing_armrise4)
        .value("dancing_armrise5", zombie_status::dancing_armrise5)
        .value("backup_spawning", zombie_status::backup_spawning)
        .value("dophin_walk_with_dophin", zombie_status::dophin_walk_with_dophin)
        .value("dophin_jump_in_pool", zombie_status::dophin_jump_in_pool)
        .value("dophin_ride", zombie_status::dophin_ride)
        .value("dophin_jump", zombie_status::dophin_jump)
        .value("dophin_walk_in_pool", zombie_status::dophin_walk_in_pool)
        .value("dophin_walk_without_dophin", zombie_status::dophin_walk_without_dophin)
        .value("snorkel_walking", zombie_status::snorkel_walking)
        .value("snorkel_jump_in_the_pool", zombie_status::snorkel_jump_in_the_pool)
        .value("snorkel_swim", zombie_status::snorkel_swim)
        .value("snorkel_up_to_eat", zombie_status::snorkel_up_to_eat)
        .value("snorkel_eat", zombie_status::snorkel_eat)
        .value("snorkel_finied_eat", zombie_status::snorkel_finied_eat)
        .value("catapult_shoot", zombie_status::catapult_shoot)
        .value("catapult_idle", zombie_status::catapult_idle)
        .value("balloon_flying", zombie_status::balloon_flying)
        .value("balloon_falling", zombie_status::balloon_falling)
        .value("balloon_walking", zombie_status::balloon_walking)
        .value("imp_flying", zombie_status::imp_flying)
        .value("imp_landing", zombie_status::imp_landing)
        .value("gargantuar_throw", zombie_status::gargantuar_throw)
        .value("gargantuar_smash", zombie_status::gargantuar_smash)
        .value("ladder_walking", zombie_status::ladder_walking)
        .value("ladder_placing", zombie_status::ladder_placing)
        .value("yeti_escape", zombie_status::yeti_escape);

    py::enum_<zombie_action>(m, "ZombieAction")
        .value("none", zombie_action::none)
        .value("entering_pool", zombie_action::entering_pool)
        .value("leaving_pool", zombie_action::leaving_pool)
        .value("caught_by_kelp", zombie_action::caught_by_kelp)
        .value("climbing_ladder", zombie_action::climbing_ladder)
        .value("falling", zombie_action::falling)
        .value("fall_from_sky", zombie_action::fall_from_sky);

    py::enum_<zombie_accessories_type_1>(m, "ZombieAccessoriesType1")
        .value("none", zombie_accessories_type_1::none)
        .value("roadcone", zombie_accessories_type_1::roadcone)
        .value("bucket", zombie_accessories_type_1::bucket)
        .value("football_cap", zombie_accessories_type_1::football_cap)
        .value("miner_hat", zombie_accessories_type_1::miner_hat);

    py::enum_<zombie_accessories_type_2>(m, "ZombieAccessoriesType2")
        .value("none", zombie_accessories_type_2::none)
        .value("screen_door", zombie_accessories_type_2::screen_door)
        .value("newspaper", zombie_accessories_type_2::newspaper)
        .value("ladder", zombie_accessories_type_2::ladder);

    py::class_<decltype(zombie::hit_box)>(m, "ZombieHitBox")
        .def_readonly("x", &decltype(zombie::hit_box)::x)
        .def_readonly("y", &decltype(zombie::hit_box)::y)
        .def_readonly("width", &decltype(zombie::hit_box)::width)
        .def_readonly("height", &decltype(zombie::hit_box)::height)
        .def_readonly("offset_x", &decltype(zombie::hit_box)::offset_x)
        .def_readonly("offset_y", &decltype(zombie::hit_box)::offset_y);

    py::class_<decltype(zombie::attack_box)>(m, "ZombieAttackBox")
        .def_readonly("x", &decltype(zombie::attack_box)::x)
        .def_readonly("y", &decltype(zombie::attack_box)::y)
        .def_readonly("width", &decltype(zombie::attack_box)::width)
        .def_readonly("height", &decltype(zombie::attack_box)::height);

    py::class_<decltype(zombie::countdown)>(m, "ZombieCountdown")
        .def_readonly("butter", &decltype(zombie::countdown)::butter)
        .def_readonly("freeze", &decltype(zombie::countdown)::freeze)
        .def_readonly("slow", &decltype(zombie::countdown)::slow)
        .def_readonly("action", &decltype(zombie::countdown)::action)
        .def_readonly("dead", &decltype(zombie::countdown)::dead);

    py::class_<decltype(zombie::accessory_1)>(m, "ZombieAccessory1")
        .def_readonly("type", &decltype(zombie::accessory_1)::type)
        .def_readonly("hp", &decltype(zombie::accessory_1)::hp)
        .def_readonly("max_hp", &decltype(zombie::accessory_1)::max_hp);

    py::class_<decltype(zombie::accessory_2)>(m, "ZombieAccessory2")
        .def_readonly("type", &decltype(zombie::accessory_2)::type)
        .def_readonly("hp", &decltype(zombie::accessory_2)::hp)
        .def_readonly("max_hp", &decltype(zombie::accessory_2)::max_hp);

    py::class_<decltype(zombie::garlic_tick)>(m, "ZombieGarlicTick")
        .def_readonly("a", &decltype(zombie::garlic_tick)::a)
        .def_readonly("b", &decltype(zombie::garlic_tick)::b)
        .def_readonly("c", &decltype(zombie::garlic_tick)::c);

    py::class_<decltype(zombie::catapult_or_jackson)>(m, "ZombieUnionCatapultOrJackson")
        .def_property("n_basketballs", [](decltype(zombie::catapult_or_jackson)& u) {
                return u.n_basketballs;
        }, [](decltype(zombie::catapult_or_jackson) &u, unsigned int v) {
                u.n_basketballs = v;
        }).def_property("summon_countdown", [](decltype(zombie::catapult_or_jackson)& u) {
            return u.summon_countdown;
        }, [](decltype(zombie::catapult_or_jackson)& u, unsigned int v) {
            u.summon_countdown = v;
        });

    py::class_<zombie>(m, "Zombie")
        .def_readonly("type", &zombie::type)
        .def_readonly("status", &zombie::status)
        .def_readonly("action", &zombie::action)
        .def_readonly("int_x", &zombie::int_x)
        .def_readonly("int_y", &zombie::int_y)
        .def_readonly("x", &zombie::x)
        .def_readonly("y", &zombie::y)
        .def_readonly("dx", &zombie::dx)
        .def_readonly("dy", &zombie::dy)
        .def_readonly("d2y", &zombie::d2y)
        .def_readonly("reanim", &zombie::reanim)
        .def_readonly("_ground", &zombie::_ground)
        .def_readonly("row", &zombie::row)
        .def_readonly("bungee_col", &zombie::bungee_col)
        .def_readonly("ladder_col", &zombie::ladder_col)
        .def_readonly("hit_box", &zombie::hit_box)
        .def_readonly("attack_box", &zombie::attack_box)
        .def_readonly("countdown", &zombie::countdown)
        .def_readonly("spawn_wave", &zombie::spawn_wave)
        .def_readonly("time_since_ate_garlic", &zombie::time_since_ate_garlic)
        .def_readonly("time_since_spawn", &zombie::time_since_spawn)
        .def_readonly("hp", &zombie::hp)
        .def_readonly("max_hp", &zombie::max_hp)
        .def_readonly("accessory_1", &zombie::accessory_1)
        .def_readonly("accessory_2", &zombie::accessory_2)
        .def_readonly("master_id", &zombie::master_id)
        .def_readonly("partners", &zombie::partners)
        .def_readonly("garlic_tick", &zombie::garlic_tick)
        .def_readonly("bungee_target", &zombie::bungee_target)
        .def_readonly("catapult_or_jackson", &zombie::catapult_or_jackson)
        .def_readonly("is_eating", &zombie::is_eating)
        .def_readonly("is_dead", &zombie::is_dead)
        .def_readonly("is_blown", &zombie::is_blown)
        .def_readonly("is_not_dying", &zombie::is_not_dying)
        .def_readonly("is_hypno", &zombie::is_hypno)
        .def_readonly("has_item_or_walk_left", &zombie::has_item_or_walk_left)
        .def_readonly("is_in_water", &zombie::is_in_water)
        .def_readonly("has_balloon", &zombie::has_balloon)
        .def_readonly("has_eaten_garlic", &zombie::has_eaten_garlic)
        .def("has_reanim", &zombie::has_reanim)
        .def("is_flying_or_falling", &zombie::is_flying_or_falling)
        .def("get_dx_from_ground", &zombie::get_dx_from_ground)
        .def("is_walk_right", &zombie::is_walk_right)
        .def("has_death_status", &zombie::has_death_status)
        .def("has_pogo_status", &zombie::has_pogo_status)
        .def("can_be_slowed", &zombie::can_be_slowed)
        .def("can_be_freezed", &zombie::can_be_freezed)
        .def("is_freeable", &zombie::is_freeable)
        .def("get_attack_box", &zombie::get_attack_box)
        .def("get_hit_box", &zombie::get_hit_box)
        .def("get_height_bias", &zombie::get_height_bias)
        .def("init_reanim", &zombie::init_reanim)
        .def("set_reanim_frame", &zombie::set_reanim_frame);

    py::enum_<plant_status>(m, "PlantStatus")
        .value("idle", plant_status::idle)
        .value("wait", plant_status::wait)
        .value("work", plant_status::work)
        .value("squash_look", plant_status::squash_look)
        .value("squash_jump_up", plant_status::squash_jump_up)
        .value("squash_stop_in_the_air", plant_status::squash_stop_in_the_air)
        .value("squash_jump_down", plant_status::squash_jump_down)
        .value("squash_crushed", plant_status::squash_crushed)
        .value("grave_buster_land", plant_status::grave_buster_land)
        .value("grave_buster_idle", plant_status::grave_buster_idle)
        .value("chomper_bite_begin", plant_status::chomper_bite_begin)
        .value("chomper_bite_success", plant_status::chomper_bite_success)
        .value("chomper_bite_fail", plant_status::chomper_bite_fail)
        .value("chomper_chew", plant_status::chomper_chew)
        .value("chomper_swallow", plant_status::chomper_swallow)
        .value("potato_sprout_out", plant_status::potato_sprout_out)
        .value("potato_armed", plant_status::potato_armed)
        .value("spike_attack", plant_status::spike_attack)
        .value("scaredyshroom_scared", plant_status::scaredyshroom_scared)
        .value("scaredyshroom_scared_idle", plant_status::scaredyshroom_scared_idle)
        .value("scaredyshroom_grow", plant_status::scaredyshroom_grow)
        .value("sunshroom_small", plant_status::sunshroom_small)
        .value("sunshroom_grow", plant_status::sunshroom_grow)
        .value("sunshroom_big", plant_status::sunshroom_big)
        .value("magnetshroom_working", plant_status::magnetshroom_working)
        .value("magnetshroom_inactive_idle", plant_status::magnetshroom_inactive_idle)
        .value("cactus_short_idle", plant_status::cactus_short_idle)
        .value("cactus_grow_tall", plant_status::cactus_grow_tall)
        .value("cactus_tall_idle", plant_status::cactus_tall_idle)
        .value("cactus_get_short", plant_status::cactus_get_short)
        .value("tangle_kelp_grab", plant_status::tangle_kelp_grab)
        .value("cob_cannon_unaramed_idle", plant_status::cob_cannon_unaramed_idle)
        .value("cob_cannon_charge", plant_status::cob_cannon_charge)
        .value("cob_cannon_launch", plant_status::cob_cannon_launch)
        .value("cob_cannon_aramed_idle", plant_status::cob_cannon_aramed_idle)
        .value("kernelpult_launch_butter", plant_status::kernelpult_launch_butter)
        .value("umbrella_leaf_block", plant_status::umbrella_leaf_block)
        .value("umbrella_leaf_shrink", plant_status::umbrella_leaf_shrink)
        .value("imitater_explode", plant_status::imitater_explode)
        .value("flower_pot_placed", plant_status::flower_pot_placed)
        .value("lily_pad_placed", plant_status::lily_pad_placed);

    py::enum_<attack_flags>(m, "PlantAttackFlags", py::arithmetic())
        .value("ground", attack_flags::ground)
        .value("flying_balloon", attack_flags::flying_balloon)
        .value("lurking_snorkel", attack_flags::lurking_snorkel)
        .value("animating_zombies", attack_flags::animating_zombies)
        .value("dying_zombies", attack_flags::dying_zombies)
        .value("digging_digger", attack_flags::digging_digger)
        .value("hypno_zombies", attack_flags::hypno_zombies);

    py::enum_<plant_edible_status>(m, "PlantEdibleStatus")
        .value("visible_and_edible", plant_edible_status::visible_and_edible)
        .value("invisible_and_edible", plant_edible_status::invisible_and_edible)
        .value("invisible_and_not_edible", plant_edible_status::invisible_and_not_edible);

    py::enum_<plant_direction>(m, "plant_direction")
        .value("left", plant_direction::left)
        .value("right", plant_direction::right);

    py::enum_<reanim_type>(m, "reanim_type")
        .value("repeat", reanim_type::repeat)
        .value("once", reanim_type::once);

    py::class_<rect>(m, "Rect")
        .def(py::init<>())
        .def_readonly("x", &rect::x)
        .def_readonly("y", &rect::y)
        .def_readonly("width", &rect::width)
        .def_readonly("height", &rect::height)
        .def("get_overlap_len", &rect::get_overlap_len)
        .def("is_overlap_with_circle", &rect::is_overlap_with_circle);

    py::class_<reanim>(m, "Reanim")
        .def_readonly("fps", &reanim::fps)
        .def_readonly("prev_fps", &reanim::prev_fps)
        .def_readonly("begin_frame", &reanim::begin_frame)
        .def_readonly("n_frames", &reanim::n_frames)
        .def_readonly("n_repeated", &reanim::n_repeated)
        .def_readonly("progress", &reanim::progress)
        .def_readonly("prev_progress", &reanim::prev_progress)
        .def_readonly("type", &reanim::type);

    py::class_<plant>(m, "Plant")
        .def_readonly("type", &plant::type)
        .def_readonly("status", &plant::status)
        .def_readonly("x", &plant::x)
        .def_readonly("y", &plant::y)
        .def_readonly("cannon", &plant::cannon)
        .def_readonly("hp", &plant::hp)
        .def_readonly("max_hp", &plant::max_hp)
        .def_readonly("attack_box", &plant::attack_box)
        .def_readonly("row", &plant::row)
        .def_readonly("col", &plant::col)
        .def_readonly("max_boot_delay", &plant::max_boot_delay)
        .def_readonly("direction", &plant::direction)
        .def_readonly("target", &plant::target)
        .def_readonly("imitater_target", &plant::imitater_target)
        .def_readonly("countdown", &plant::countdown)
        .def_readonly("reanim", &plant::reanim)
        .def_readonly("edible", &plant::edible)
        .def_readonly("threepeater_time_since_first_shot",
            &plant::threepeater_time_since_first_shot)
        .def_readonly("split_pea_attack_flags", &plant::split_pea_attack_flags)
        .def_readonly("is_dead", &plant::is_dead)
        .def_readonly("is_smashed", &plant::is_smashed)
        .def_readonly("is_sleeping", &plant::is_sleeping)
        .def_readonly("can_attack", &plant::can_attack)
        .def("is_squash_attacking", &plant::is_squash_attacking)
        .def("is_sun_plant", &plant::is_sun_plant)
        .def("is_freeable", &plant::is_freeable)
        .def("get_hit_box", &plant::get_hit_box)
        .def("get_attack_box", &plant::get_attack_box)
        .def("get_attack_flags", &plant::get_attack_flags)
        .def("set_sleep", &plant::set_sleep)
        .def("init_reanim", &plant::init_reanim)
        .def("set_reanim_frame", &plant::set_reanim_frame)
        .def("set_reanim", &plant::set_reanim)
        .def("has_reanim", &plant::has_reanim);

    py::class_<decltype(plant::cannon)>(m, "PlantCannonPos")
        .def_readonly("x", &decltype(plant::cannon)::x)
        .def_readonly("y", &decltype(plant::cannon)::y);

    py::class_<decltype(plant::attack_box)>(m, "PlantAttackBox")
        .def_readonly("width", &decltype(plant::attack_box)::width)
        .def_readonly("height", &decltype(plant::attack_box)::height);

    py::class_<decltype(plant::countdown)>(m, "PlantCountdown")
        .def_readonly("status", &decltype(plant::countdown)::status)
        .def_readonly("generate", &decltype(plant::countdown)::generate)
        .def_readonly("launch", &decltype(plant::countdown)::launch)
        .def_readonly("eaten", &decltype(plant::countdown)::eaten)
        .def_readonly("awake", &decltype(plant::countdown)::awake)
        .def_readonly("effect", &decltype(plant::countdown)::effect)
        .def_readonly("dead", &decltype(plant::countdown)::dead)
        .def_readonly("blover_disappear", &decltype(plant::countdown)::blover_disappear);

    py::class_<decltype(plant::split_pea_attack_flags)>(m, "PlantSplitPeaAttackFlags")
        .def_readonly("front", &decltype(plant::split_pea_attack_flags)::front)
        .def_readonly("back", &decltype(plant::split_pea_attack_flags)::back);
    
    py::enum_<projectile_type>(m, "ProjectileType")
        .value("none", projectile_type::none)
        .value("pea", projectile_type::pea)
        .value("snow_pea", projectile_type::snow_pea)
        .value("cabbage", projectile_type::cabbage)
        .value("melon", projectile_type::melon)
        .value("puff", projectile_type::puff)
        .value("wintermelon", projectile_type::wintermelon)
        .value("fire_pea", projectile_type::fire_pea)
        .value("star", projectile_type::star)
        .value("cactus", projectile_type::cactus)
        .value("basketball", projectile_type::basketball)
        .value("kernel", projectile_type::kernel)
        .value("cob_cannon", projectile_type::cob_cannon)
        .value("butter", projectile_type::butter);

    py::enum_<projectile_motion_type>(m, "ProjectileMotionType")
        .value("straight", projectile_motion_type::straight)
        .value("parabola", projectile_motion_type::parabola)
        .value("switch_way", projectile_motion_type::switch_way)
        .value("puff", projectile_motion_type::puff)
        .value("left_straight", projectile_motion_type::left_straight)
        .value("starfruit", projectile_motion_type::starfruit)
        .value("cattail", projectile_motion_type::cattail);

    py::class_<projectile>(m, "Projectile")
        .def_readonly("type", &projectile::type)
        .def_readonly("motion_type", &projectile::motion_type)
        .def_readonly("int_x", &projectile::int_x)
        .def_readonly("int_y", &projectile::int_y)
        .def_readonly("row", &projectile::row)
        .def_readonly("y", &projectile::y)
        .def_readonly("x", &projectile::x)
        .def_readonly("shadow_y", &projectile::shadow_y)
        .def_readonly("dx", &projectile::dx)
        .def_readonly("dy1", &projectile::dy1)
        .def_readonly("dy2", &projectile::dy2)
        .def_readonly("ddy", &projectile::ddy)
        .def_readonly("dddy", &projectile::dddy)
        .def_readonly("attack_box", &projectile::attack_box)
        .def_readonly("flags", &projectile::flags)
        .def_readonly("time_since_created", &projectile::time_since_created)
        .def_readonly("countdown", &projectile::countdown)
        .def_readonly("last_torchwood_col", &projectile::last_torchwood_col)
        .def_readonly("cannon_x", &projectile::cannon_x)
        .def_readonly("cannon_row", &projectile::cannon_row)
        .def_readonly("target", &projectile::target)
        .def_readonly("is_visible", &projectile::is_visible)
        .def_readonly("is_disappeared", &projectile::is_disappeared);

    py::class_<decltype(projectile::attack_box)>(m, "ProjectileAttackBox")
        .def_readonly("width", &decltype(projectile::attack_box)::width)
        .def_readonly("height", &decltype(projectile::attack_box)::height);

    py::class_<griditem>(m, "Griditem")
        .def_readonly("type", &griditem::type)
        .def_readonly("col", &griditem::col)
        .def_readonly("row", &griditem::row)
        .def_readonly("countdown", &griditem::countdown)
        .def_readonly("is_disappeared", &griditem::is_disappeared)
        .def("is_freeable", &griditem::is_freeable);

    py::enum_<griditem_type>(m, "GriditemType")
        .value("grave", griditem_type::grave)
        .value("crater", griditem_type::crater)
        .value("ladder", griditem_type::ladder);

    using namespace pvz_emulator::system;

    py::class_<sun>(m, "Sun")
        .def(py::init<scene>())
        .def("add_sun", &sun::add_sun)
        .def("update", &sun::update);

    py::class_<spawn>(m, "Spawn")
        .def(py::init<scene>())
        .def("get_current_hp", &spawn::get_current_hp)
        .def("reset", &spawn::reset)
        .def("update", &spawn::update);

    py::class_<ice_path>(m, "IcePath")
        .def(py::init<scene>())
        .def("update", &ice_path::update);

    py::class_<endgame>(m, "Endgame")
        .def(py::init<scene>())
        .def("update", &endgame::update);

    py::class_<system::griditem>(m, "GriditemSystem")
        .def(py::init<scene>())
        .def("update", &system::griditem::update);

    py::class_<plant_system>(m, "PlantSystem")
        .def(py::init<scene>())
        .def("launch", &plant_system::launch)
        .def("update", &plant_system::update);

    py::class_<plant_factory>(m, "PlantFactory")
        .def(py::init<scene>())
        .def("can_plant", &plant_factory::can_plant)
        .def("plant", &plant_factory::plant)
        .def("create",
            &plant_factory::create,
            py::arg("type"),
            py::arg("row"),
            py::arg("col"),
            py::arg("imitater_target") = plant_type::none)
        .def("destroy", &plant_factory::destroy);

    py::class_<zombie_factory>(m, "ZombieFactory")
        .def(py::init<scene>())
        .def("create", &zombie_factory::create)
        .def("create_lurking", &zombie_factory::create_lurking)
        .def("destroy", &zombie_factory::destroy);

    py::class_<griditem_factory>(m, "GriditemFactory")
        .def(py::init<scene>())
        .def("create", &griditem_factory::create)
        .def("destroy", &griditem_factory::destroy);

    py::class_<zombie_system>(m, "ZombieSystem")
        .def(py::init<scene>())
        .def("update", &zombie_system::update);

    py::class_<projectile_system>(m, "ProjectileSystem")
        .def(py::init<scene>())
        .def("update", &projectile_system::update);
}
