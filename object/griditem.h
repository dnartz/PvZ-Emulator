#pragma once
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace pvz_emulator::object {

enum class griditem_type {
    grave = 0x1,
    crater = 0x2,
    ladder = 0x3,
    // brain = 0x6,
    // vase = 0x7,
    // rake = 0xB,
};

class scene;

class griditem {
public:
    griditem_type type;
    unsigned int col;
    unsigned int row;
    int countdown;
    bool is_disappeared;

    bool is_freeable() {
        return is_disappeared;
    }

    void to_json(
        scene& scene,
        rapidjson::Writer<rapidjson::StringBuffer>& writer);
};

}
