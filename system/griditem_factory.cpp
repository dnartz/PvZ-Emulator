#include <cassert>
#include "system/plant/plant_factory.h"
#include "griditem_factory.h"

namespace pvz_emulator::system {

griditem& griditem_factory::create(
    griditem_type type,
    unsigned int row,
    unsigned int col)
{
    auto& item = scene.griditems.alloc();

    item.type = type;
    item.col = col;
    item.row = row;

    switch (type) {
    case pvz_emulator::object::griditem_type::grave:
        for (auto& p : scene.plants) {
            if (p.row == item.row && p.col == item.col) {
                plant_factory(scene).destroy(p);
            }
        }

        item.countdown = -static_cast<int>(rng.randint(50));
        break;

    case pvz_emulator::object::griditem_type::crater:
        item.countdown = 18000;
        break;

    case pvz_emulator::object::griditem_type::ladder:
        item.countdown = 0;
        break;

    default:
        assert(false);
        break;
    }

    return item;
}

}
