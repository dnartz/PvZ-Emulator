#pragma once
#include "object/scene.h"
#include "object/griditem.h"
#include "griditem_factory.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

class griditem {
    object::scene& scene;
    system::griditem_factory griditem_factory;

public:
    griditem(object::scene& s): scene(s), griditem_factory(s) {}

    void update() {
        for (auto& item : scene.griditems) {
            switch (item.type) {
            case griditem_type::grave:
                if (item.countdown < 100) {
                    item.countdown++;
                }
                break;

            case griditem_type::crater:
                if (item.countdown > 0 && --item.countdown == 0) {
                    griditem_factory.destroy(item);
                }
                break;

            default:
                break;
            }
        }
    }
};

}
