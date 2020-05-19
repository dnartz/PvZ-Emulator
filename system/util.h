#pragma once

#include "object/scene.h"
#include "object/zombie.h"
#include "object/plant.h"
#include "system/rng.h"

namespace pvz_emulator::system {

int get_col_by_x(int x);

inline int get_x_by_col(int col) {
    return 80 * col + 40;
}

float get_y_by_col(object::scene_type s, unsigned int row, unsigned int col);

float get_y_by_row_and_x(object::scene_type s, unsigned int row, float x);

int get_y_by_row_and_col(object::scene_type s, int row, int col);

float zombie_init_y(
    object::scene_type s,
    const object::zombie& z,
    unsigned int row);

int get_row_by_x_and_y(object::scene_type s, int x, int y);

bool is_slowed(object::scene& scene, object::zombie& z);

bool is_not_movable(object::scene& scene, object::zombie& z);

enum class zombie_attack_type {
    smash_or_eat = 0,
    crush = 1,
    jump = 2,
    place_ladder = 3
};

bool is_target_of_kelp(object::scene& s, object::zombie& z);

}