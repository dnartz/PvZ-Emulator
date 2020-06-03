#pragma once
#include "object/plant.h"
#include "object/scene.h"
#include "system/rng.h"
#include "system/plant/plant.h"
#include "system/zombie/zombie_factory.h"
#include "system/griditem_factory.h"

namespace pvz_emulator::system {

class plant_factory {
    object::scene& scene;
    system::zombie_factory zombie_factory;
    system::griditem_factory griditem_factory;
    system::plant_subsystems subsystems;
    system::rng rng;

    bool is_pos_valid(unsigned row, unsigned col) const;
    bool is_not_covered_by_ice_path(unsigned row, unsigned col) const;
    void is_covered_by_griditem(
        unsigned row,
        unsigned col,
        bool& has_grave,
        bool& has_crater) const;

    bool can_plant_advanced_plant(
        grid_plant_status& status,
        object::plant_type advanced) const;

    unsigned int get_cost(object::plant_type type) const;

public:
    bool can_plant(
        unsigned int row,
        unsigned int col,
        object::plant_type type,
        object::plant_type imitater_type = object::plant_type::none) const;

    object::plant* plant(
        unsigned int i,
        unsigned int row,
        unsigned int col);

    object::plant& create(
        object::plant_type type,
        int row,
        int col,
        object::plant_type imitater_target = object::plant_type::none);

    void destroy(object::plant &p);
    plant_factory(object::scene &s):
        scene(s),
        zombie_factory(s),
        griditem_factory(s),
        subsystems(s),
        rng(s) {}
};

}
