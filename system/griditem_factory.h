#pragma once
#include "object/scene.h"
#include "object/griditem.h"
#include "system/rng.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

class griditem_factory {
private:
    object::scene& scene;
    system::rng rng;

public:
    griditem_factory(object::scene& s) : scene(s), rng(s) {}

    griditem& create(griditem_type type, unsigned int row, unsigned int col);

    void destroy(object::griditem& item) {
        item.is_disappeared = true;
    }
};

}
