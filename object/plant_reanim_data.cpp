#include <cassert>
#include <utility>
#include "object/plant.h"
#include "system/plant/plant.h"

namespace pvz_emulator::system {

const std::pair<float, float> plant_gatling_pea::PEA_OFFSETS[93] = {
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {37.6f, 48.7f},
    {38.0f, 47.0f}, {38.3f, 45.5f}, {38.6f, 43.8f}, {39.1f, 43.5f}, {39.6f, 43.1f},
    {40.0f, 42.8f}, {41.1f, 43.1f}, {42.1f, 43.5f}, {43.2f, 43.8f}, {44.6f, 44.6f},
    {46.0f, 45.5f}, {47.4f, 46.3f}, {46.0f, 45.5f}, {44.6f, 44.6f}, {43.2f, 43.8f},
    {42.1f, 43.5f}, {41.1f, 43.1f}, {40.0f, 42.8f}, {39.6f, 43.1f}, {39.1f, 43.5f},
    {38.6f, 43.8f}, {38.3f, 45.5f}, {38.0f, 47.0f}, {37.6f, 48.7f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f} 
};

const std::pair<float, float> plant_pea_shooter::PEA_OFFSETS[104] = {
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {37.6f, 48.7f},
    {38.0f, 47.0f}, {38.3f, 45.5f}, {38.6f, 43.8f}, {39.1f, 43.5f}, {39.6f, 43.1f},
    {40.0f, 42.8f}, {41.1f, 43.1f}, {42.1f, 43.5f}, {43.2f, 43.8f}, {44.6f, 44.6f},
    {46.0f, 45.5f}, {47.4f, 46.3f}, {46.0f, 45.5f}, {44.6f, 44.6f}, {43.2f, 43.8f},
    {42.1f, 43.5f}, {41.1f, 43.1f}, {40.0f, 42.8f}, {39.6f, 43.1f}, {39.1f, 43.5f},
    {38.6f, 43.8f}, {38.3f, 45.5f}, {38.0f, 47.0f}, {37.6f, 48.7f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {38.5f, 47.0f}, {39.3f, 45.3f}, {40.1f, 43.5f}, {41.1f, 43.0f}, {42.1f, 42.6f},
    {43.0f, 42.1f}, {45.1f, 41.8f}, {47.1f, 41.5f}, {49.2f, 41.1f}, {52.0f, 42.2f},
    {54.8f, 43.4f}, {57.6f, 44.5f}, {55.0f, 43.3f}, {52.4f, 42.0f}, {49.8f, 40.8f},
    {47.6f, 40.0f}, {45.5f, 39.3f}, {43.4f, 38.5f}, {42.2f, 39.0f}, {41.0f, 39.5f},
    {39.9f, 39.9f}, {39.2f, 42.9f}, {38.5f, 45.8f}, {37.9f, 48.7f} 
};

const std::pair<float, float> plant_repeater::PEA_OFFSETS[104] = {
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {37.6f, 48.7f},
    {38.0f, 47.0f}, {38.3f, 45.5f}, {38.6f, 43.8f}, {39.1f, 43.5f}, {39.6f, 43.1f},
    {40.0f, 42.8f}, {41.1f, 43.1f}, {42.1f, 43.5f}, {43.2f, 43.8f}, {44.6f, 44.6f},
    {46.0f, 45.5f}, {47.4f, 46.3f}, {46.0f, 45.5f}, {44.6f, 44.6f}, {43.2f, 43.8f},
    {42.1f, 43.5f}, {41.1f, 43.1f}, {40.0f, 42.8f}, {39.6f, 43.1f}, {39.1f, 43.5f},
    {38.6f, 43.8f}, {38.3f, 45.5f}, {38.0f, 47.0f}, {37.6f, 48.7f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {38.0f, 47.0f}, {38.3f, 45.5f}, {38.6f, 43.8f}, {39.1f, 43.5f}, {39.6f, 43.1f},
    {40.0f, 42.8f}, {41.1f, 43.1f}, {42.1f, 43.5f}, {43.2f, 43.8f}, {44.6f, 44.6f},
    {46.0f, 45.5f}, {47.4f, 46.3f}, {46.0f, 45.5f}, {44.6f, 44.6f}, {43.2f, 43.8f},
    {42.1f, 43.5f}, {41.1f, 43.1f}, {40.0f, 42.8f}, {39.6f, 43.1f}, {39.1f, 43.5f},
    {38.6f, 43.8f}, {38.3f, 45.5f}, {38.0f, 47.0f}, {37.6f, 48.7f} 
};

const std::pair<float, float> plant_snow_pea::PEA_OFFSETS[104] = {
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {37.6f, 48.7f},
    {38.0f, 47.0f}, {38.3f, 45.5f}, {38.6f, 43.8f}, {39.1f, 43.5f}, {39.6f, 43.1f},
    {40.0f, 42.8f}, {41.1f, 43.1f}, {42.1f, 43.5f}, {43.2f, 43.8f}, {44.6f, 44.6f},
    {46.0f, 45.5f}, {47.4f, 46.3f}, {46.0f, 45.5f}, {44.6f, 44.6f}, {43.2f, 43.8f},
    {42.1f, 43.5f}, {41.1f, 43.1f}, {40.0f, 42.8f}, {39.6f, 43.1f}, {39.1f, 43.5f},
    {38.6f, 43.8f}, {38.3f, 45.5f}, {38.0f, 47.0f}, {37.6f, 48.7f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {38.0f, 47.0f}, {38.3f, 45.5f}, {38.6f, 43.8f}, {39.1f, 43.5f}, {39.6f, 43.1f},
    {40.0f, 42.8f}, {41.1f, 43.1f}, {42.1f, 43.5f}, {43.2f, 43.8f}, {44.6f, 44.6f},
    {46.0f, 45.5f}, {47.4f, 46.3f}, {46.0f, 45.5f}, {44.6f, 44.6f}, {43.2f, 43.8f},
    {42.1f, 43.5f}, {41.1f, 43.1f}, {40.0f, 42.8f}, {39.6f, 43.1f}, {39.1f, 43.5f},
    {38.6f, 43.8f}, {38.3f, 45.5f}, {38.0f, 47.0f}, {37.6f, 48.7f} 
};

const std::pair<float, float> plant_split_pea::PEA_OFFSETS[132] = {
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {37.6f, 48.7f}, {38.0f, 47.1f}, {38.3f, 45.5f},
    {38.6f, 43.8f}, {39.1f, 43.5f}, {39.6f, 43.1f}, {40.1f, 42.8f}, {41.1f, 43.1f},
    {42.1f, 43.5f}, {43.2f, 43.8f}, {44.6f, 44.6f}, {46.0f, 45.5f}, {47.4f, 46.3f},
    {46.0f, 45.5f}, {44.6f, 44.6f}, {43.2f, 43.8f}, {42.1f, 43.5f}, {41.1f, 43.1f},
    {40.1f, 42.8f}, {39.6f, 43.1f}, {39.1f, 43.5f}, {38.6f, 43.8f}, {38.3f, 45.5f},
    {38.0f, 47.1f}, {37.6f, 48.7f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f} 
};

}

namespace pvz_emulator::object {

void plant::init_reanim() {
    reanim.prev_progress = -1;
    switch (type) {
    case plant_type::blover:
        reanim.n_frames = 62;
        reanim.fps = 12.000000f;
        break;
    case plant_type::cabbagepult:
        reanim.n_frames = 74;
        reanim.fps = 12.000000f;
        break;
    case plant_type::cactus:
        reanim.n_frames = 95;
        reanim.fps = 12.000000f;
        break;
    case plant_type::cattail:
        reanim.n_frames = 40;
        reanim.fps = 12.000000f;
        break;
    case plant_type::cherry_bomb:
        reanim.n_frames = 27;
        reanim.fps = 12.000000f;
        break;
    case plant_type::chomper:
        reanim.n_frames = 94;
        reanim.fps = 12.000000f;
        break;
    case plant_type::cob_cannon:
        reanim.n_frames = 100;
        reanim.fps = 12.000000f;
        break;
    case plant_type::coffee_bean:
        reanim.n_frames = 29;
        reanim.fps = 12.000000f;
        break;
    case plant_type::doomshroom:
        reanim.n_frames = 77;
        reanim.fps = 12.000000f;
        break;
    case plant_type::flower_pot:
        reanim.n_frames = 39;
        reanim.fps = 12.000000f;
        break;
    case plant_type::fumeshroom:
        reanim.n_frames = 68;
        reanim.fps = 12.000000f;
        break;
    case plant_type::garlic:
        reanim.n_frames = 19;
        reanim.fps = 12.000000f;
        break;
    case plant_type::gatling_pea:
        reanim.n_frames = 93;
        reanim.fps = 12.000000f;
        break;
    case plant_type::gloomshroom:
        reanim.n_frames = 71;
        reanim.fps = 12.000000f;
        break;
    case plant_type::gold_magnet:
        reanim.n_frames = 83;
        reanim.fps = 12.000000f;
        break;
    case plant_type::grave_buster:
        reanim.n_frames = 39;
        reanim.fps = 12.000000f;
        break;
    case plant_type::hypnoshroom:
        reanim.n_frames = 40;
        reanim.fps = 12.000000f;
        break;
    case plant_type::iceshroom:
        reanim.n_frames = 38;
        reanim.fps = 12.000000f;
        break;
    case plant_type::imitater:
        reanim.n_frames = 84;
        reanim.fps = 12.000000f;
        break;
    case plant_type::jalapeno:
        reanim.n_frames = 25;
        reanim.fps = 12.000000f;
        break;
    case plant_type::kernelpult:
        reanim.n_frames = 77;
        reanim.fps = 12.000000f;
        break;
    case plant_type::lily_pad:
        reanim.n_frames = 4;
        reanim.fps = 12.000000f;
        break;
    case plant_type::magnetshroom:
        reanim.n_frames = 131;
        reanim.fps = 12.000000f;
        break;
    case plant_type::marigold:
        reanim.n_frames = 30;
        reanim.fps = 12.000000f;
        break;
    case plant_type::melonpult:
        reanim.n_frames = 74;
        reanim.fps = 12.000000f;
        break;
    case plant_type::pea_shooter:
        reanim.n_frames = 104;
        reanim.fps = 12.000000f;
        break;
    case plant_type::plantern:
        reanim.n_frames = 25;
        reanim.fps = 12.000000f;
        break;
    case plant_type::potato_mine:
        reanim.n_frames = 37;
        reanim.fps = 12.000000f;
        break;
    case plant_type::puffshroom:
        reanim.n_frames = 51;
        reanim.fps = 12.000000f;
        break;
    case plant_type::pumpkin:
        reanim.n_frames = 21;
        reanim.fps = 12.000000f;
        break;
    case plant_type::repeater:
        reanim.n_frames = 104;
        reanim.fps = 12.000000f;
        break;
    case plant_type::scaredyshroom:
        reanim.n_frames = 86;
        reanim.fps = 12.000000f;
        break;
    case plant_type::seashroom:
        reanim.n_frames = 93;
        reanim.fps = 12.000000f;
        break;
    case plant_type::snow_pea:
        reanim.n_frames = 104;
        reanim.fps = 12.000000f;
        break;
    case plant_type::spikerock:
        reanim.n_frames = 33;
        reanim.fps = 12.000000f;
        break;
    case plant_type::spikeweed:
        reanim.n_frames = 35;
        reanim.fps = 12.000000f;
        break;
    case plant_type::split_pea:
        reanim.n_frames = 132;
        reanim.fps = 12.000000f;
        break;
    case plant_type::squash:
        reanim.n_frames = 73;
        reanim.fps = 12.000000f;
        break;
    case plant_type::starfruit:
        reanim.n_frames = 39;
        reanim.fps = 12.000000f;
        break;
    case plant_type::sunflower:
        reanim.n_frames = 29;
        reanim.fps = 12.000000f;
        break;
    case plant_type::sunshroom:
        reanim.n_frames = 63;
        reanim.fps = 12.000000f;
        break;
    case plant_type::tallnut:
        reanim.n_frames = 37;
        reanim.fps = 12.000000f;
        break;
    case plant_type::tangle_kelp:
        reanim.n_frames = 54;
        reanim.fps = 12.000000f;
        break;
    case plant_type::threepeater:
        reanim.n_frames = 149;
        reanim.fps = 12.000000f;
        break;
    case plant_type::torchwood:
        reanim.n_frames = 25;
        reanim.fps = 12.000000f;
        break;
    case plant_type::twin_sunflower:
        reanim.n_frames = 34;
        reanim.fps = 12.000000f;
        break;
    case plant_type::umbrella_leaf:
        reanim.n_frames = 32;
        reanim.fps = 12.000000f;
        break;
    case plant_type::wallnut:
        reanim.n_frames = 56;
        reanim.fps = 12.000000f;
        break;
    case plant_type::winter_melon:
        reanim.n_frames = 74;
        reanim.fps = 12.000000f;
        break;
    default:
        assert(false);
    }
}

void plant::set_reanim_frame(plant_reanim_name name) {
    if (type == plant_type::blover)  {
        if(name == plant_reanim_name::anim_loop) {
            reanim.begin_frame = 52;
            reanim.n_frames = 10;
        } else if (name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 33;
        } else {
            assert(false);
        }
    } else if (type == plant_type::cabbagepult)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 31;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 36;
            reanim.n_frames = 38;
        } else {
            assert(false);
        }
    } else if (type == plant_type::cactus)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 15;
        } else if (name == plant_reanim_name::anim_idlehigh) {
            reanim.begin_frame = 53;
            reanim.n_frames = 15;
        } else if (name == plant_reanim_name::anim_lower) {
            reanim.begin_frame = 82;
            reanim.n_frames = 13;
        } else if (name == plant_reanim_name::anim_rise) {
            reanim.begin_frame = 38;
            reanim.n_frames = 15;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 20;
            reanim.n_frames = 18;
        } else if (name == plant_reanim_name::anim_shootinghigh) {
            reanim.begin_frame = 68;
            reanim.n_frames = 14;
        } else {
            assert(false);
        }
    } else if (type == plant_type::cattail)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 19;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 24;
            reanim.n_frames = 16;
        } else {
            assert(false);
        }
    } else if (type == plant_type::cherry_bomb)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 14;
            reanim.n_frames = 13;
        } else if (name == plant_reanim_name::anim_explode) {
            reanim.begin_frame = 0;
            reanim.n_frames = 14;
        } else {
            assert(false);
        }
    } else if (type == plant_type::chomper)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 25;
        } else if (name == plant_reanim_name::anim_bite) {
            reanim.begin_frame = 25;
            reanim.n_frames = 25;
        } else if (name == plant_reanim_name::anim_chew) {
            reanim.begin_frame = 50;
            reanim.n_frames = 16;
        } else if (name == plant_reanim_name::anim_swallow) {
            reanim.begin_frame = 66;
            reanim.n_frames = 28;
        } else {
            assert(false);
        }
    } else if (type == plant_type::cob_cannon)  {
        if(name == plant_reanim_name::anim_charge) {
            reanim.begin_frame = 24;
            reanim.n_frames = 15;
        } else if (name == plant_reanim_name::anim_unarmed_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 19;
        } else if (name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 39;
            reanim.n_frames = 19;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 58;
            reanim.n_frames = 42;
        } else {
            assert(false);
        }
    } else if (type == plant_type::coffee_bean)  {
        if(name == plant_reanim_name::anim_crumble) {
            reanim.begin_frame = 14;
            reanim.n_frames = 15;
        } else if (name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 9;
        } else {
            assert(false);
        }
    } else if (type == plant_type::doomshroom)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 20;
        } else if (name == plant_reanim_name::anim_sleep) {
            reanim.begin_frame = 52;
            reanim.n_frames = 25;
        } else if (name == plant_reanim_name::anim_explode) {
            reanim.begin_frame = 19;
            reanim.n_frames = 33;
        } else {
            assert(false);
        }
    } else if (type == plant_type::flower_pot)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 13;
        } else {
            assert(false);
        }
    } else if (type == plant_type::fumeshroom)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 17;
        } else if (name == plant_reanim_name::anim_sleep) {
            reanim.begin_frame = 51;
            reanim.n_frames = 17;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 21;
            reanim.n_frames = 30;
        } else {
            assert(false);
        }
    } else if (type == plant_type::garlic)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 15;
        } else {
            assert(false);
        }
    } else if (type == plant_type::gatling_pea)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 25;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 54;
            reanim.n_frames = 39;
        } else {
            assert(false);
        }
    } else if (type == plant_type::gloomshroom)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 19;
        } else if (name == plant_reanim_name::anim_sleep) {
            reanim.begin_frame = 24;
            reanim.n_frames = 19;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 43;
            reanim.n_frames = 28;
        } else {
            assert(false);
        }
    } else if (type == plant_type::gold_magnet)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 47;
        } else {
            assert(false);
        }
    } else if (type == plant_type::grave_buster)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 11;
            reanim.n_frames = 28;
        } else if (name == plant_reanim_name::anim_land) {
            reanim.begin_frame = 0;
            reanim.n_frames = 7;
        } else {
            assert(false);
        }
    } else if (type == plant_type::hypnoshroom)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 20;
        } else if (name == plant_reanim_name::anim_sleep) {
            reanim.begin_frame = 20;
            reanim.n_frames = 20;
        } else {
            assert(false);
        }
    } else if (type == plant_type::iceshroom)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 17;
        } else if (name == plant_reanim_name::anim_sleep) {
            reanim.begin_frame = 21;
            reanim.n_frames = 17;
        } else {
            assert(false);
        }
    } else if (type == plant_type::imitater)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 3;
            reanim.n_frames = 50;
        } else if (name == plant_reanim_name::anim_explode) {
            reanim.begin_frame = 53;
            reanim.n_frames = 31;
        } else {
            assert(false);
        }
    } else if (type == plant_type::jalapeno)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 7;
        } else if (name == plant_reanim_name::anim_explode) {
            reanim.begin_frame = 12;
            reanim.n_frames = 13;
        } else {
            assert(false);
        }
    } else if (type == plant_type::kernelpult)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 19;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 24;
            reanim.n_frames = 34;
        } else {
            assert(false);
        }
    } else if (type == plant_type::lily_pad)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 1;
        } else {
            assert(false);
        }
    } else if (type == plant_type::magnetshroom)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 29;
        } else if (name == plant_reanim_name::anim_sleep) {
            reanim.begin_frame = 108;
            reanim.n_frames = 18;
        } else if (name == plant_reanim_name::anim_nonactive_idle2) {
            reanim.begin_frame = 126;
            reanim.n_frames = 5;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 71;
            reanim.n_frames = 26;
        } else {
            assert(false);
        }
    } else if (type == plant_type::marigold)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 25;
        } else {
            assert(false);
        }
    } else if (type == plant_type::melonpult)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 31;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 36;
            reanim.n_frames = 38;
        } else {
            assert(false);
        }
    } else if (type == plant_type::pea_shooter)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 25;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 54;
            reanim.n_frames = 25;
        } else {
            assert(false);
        }
    } else if (type == plant_type::plantern)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 20;
        } else {
            assert(false);
        }
    } else if (type == plant_type::potato_mine)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 1;
        } else if (name == plant_reanim_name::anim_glow) {
            reanim.begin_frame = 20;
            reanim.n_frames = 1;
        } else if (name == plant_reanim_name::anim_armed) {
            reanim.begin_frame = 20;
            reanim.n_frames = 11;
        } else if (name == plant_reanim_name::anim_rise) {
            reanim.begin_frame = 1;
            reanim.n_frames = 19;
        } else {
            assert(false);
        }
    } else if (type == plant_type::puffshroom)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 17;
        } else if (name == plant_reanim_name::anim_sleep) {
            reanim.begin_frame = 34;
            reanim.n_frames = 17;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 21;
            reanim.n_frames = 13;
        } else {
            assert(false);
        }
    } else if (type == plant_type::pumpkin)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 21;
        } else {
            assert(false);
        }
    } else if (type == plant_type::repeater)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 25;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 54;
            reanim.n_frames = 25;
        } else {
            assert(false);
        }
    } else if (type == plant_type::scaredyshroom)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 16;
        } else if (name == plant_reanim_name::anim_scared) {
            reanim.begin_frame = 36;
            reanim.n_frames = 13;
        } else if (name == plant_reanim_name::anim_scaredidle) {
            reanim.begin_frame = 49;
            reanim.n_frames = 11;
        } else if (name == plant_reanim_name::anim_sleep) {
            reanim.begin_frame = 70;
            reanim.n_frames = 16;
        } else if (name == plant_reanim_name::anim_grow) {
            reanim.begin_frame = 60;
            reanim.n_frames = 10;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 21;
            reanim.n_frames = 15;
        } else {
            assert(false);
        }
    } else if (type == plant_type::seashroom)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 25;
        } else if (name == plant_reanim_name::anim_sleep) {
            reanim.begin_frame = 43;
            reanim.n_frames = 25;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 29;
            reanim.n_frames = 14;
        } else {
            assert(false);
        }
    } else if (type == plant_type::snow_pea)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 25;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 54;
            reanim.n_frames = 25;
        } else {
            assert(false);
        }
    } else if (type == plant_type::spikerock)  {
        if(name == plant_reanim_name::anim_attack) {
            reanim.begin_frame = 18;
            reanim.n_frames = 15;
        } else if (name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 3;
            reanim.n_frames = 15;
        } else {
            assert(false);
        }
    } else if (type == plant_type::spikeweed)  {
        if(name == plant_reanim_name::anim_attack) {
            reanim.begin_frame = 25;
            reanim.n_frames = 10;
        } else if (name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 21;
        } else {
            assert(false);
        }
    } else if (type == plant_type::split_pea)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 7;
            reanim.n_frames = 25;
        } else if (name == plant_reanim_name::anim_splitpea_shooting) {
            reanim.begin_frame = 57;
            reanim.n_frames = 13;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 107;
            reanim.n_frames = 25;
        } else {
            assert(false);
        }
    } else if (type == plant_type::squash)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 19;
        } else if (name == plant_reanim_name::anim_lookleft) {
            reanim.begin_frame = 24;
            reanim.n_frames = 6;
        } else if (name == plant_reanim_name::anim_lookright) {
            reanim.begin_frame = 37;
            reanim.n_frames = 6;
        } else if (name == plant_reanim_name::anim_jumpup) {
            reanim.begin_frame = 50;
            reanim.n_frames = 15;
        } else if (name == plant_reanim_name::anim_jumpdown) {
            reanim.begin_frame = 65;
            reanim.n_frames = 8;
        } else {
            assert(false);
        }
    } else if (type == plant_type::starfruit)  {
        if(name == plant_reanim_name::anim_shoot) {
            reanim.begin_frame = 22;
            reanim.n_frames = 17;
        } else if (name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 17;
        } else {
            assert(false);
        }
    } else if (type == plant_type::sunflower)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 25;
        } else {
            assert(false);
        }
    } else if (type == plant_type::sunshroom)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 12;
        } else if (name == plant_reanim_name::anim_bigidle) {
            reanim.begin_frame = 39;
            reanim.n_frames = 12;
        } else if (name == plant_reanim_name::anim_sleep) {
            reanim.begin_frame = 17;
            reanim.n_frames = 10;
        } else if (name == plant_reanim_name::anim_grow) {
            reanim.begin_frame = 27;
            reanim.n_frames = 12;
        } else {
            assert(false);
        }
    } else if (type == plant_type::tallnut)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 20;
            reanim.n_frames = 17;
        } else {
            assert(false);
        }
    } else if (type == plant_type::tangle_kelp)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 22;
        } else {
            assert(false);
        }
    } else if (type == plant_type::threepeater)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 124;
            reanim.n_frames = 25;
        } else {
            assert(false);
        }
    } else if (type == plant_type::torchwood)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 21;
        } else {
            assert(false);
        }
    } else if (type == plant_type::twin_sunflower)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 9;
            reanim.n_frames = 25;
        } else {
            assert(false);
        }
    } else if (type == plant_type::umbrella_leaf)  {
        if(name == plant_reanim_name::anim_block) {
            reanim.begin_frame = 19;
            reanim.n_frames = 13;
        } else if (name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 4;
            reanim.n_frames = 15;
        } else {
            assert(false);
        }
    } else if (type == plant_type::wallnut)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 17;
        } else {
            assert(false);
        }
    } else if (type == plant_type::winter_melon)  {
        if(name == plant_reanim_name::anim_idle) {
            reanim.begin_frame = 5;
            reanim.n_frames = 31;
        } else if (name == plant_reanim_name::anim_shooting) {
            reanim.begin_frame = 36;
            reanim.n_frames = 38;
        } else {
            assert(false);
        }
    } else {
        assert(false);
    }
}

bool plant::has_reanim(plant_reanim_name name) {
   switch(type) {
    case plant_type::blover:
        return name == plant_reanim_name::anim_loop ||
            name == plant_reanim_name::anim_idle;

    case plant_type::cabbagepult:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::cactus:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_idlehigh ||
            name == plant_reanim_name::anim_lower ||
            name == plant_reanim_name::anim_rise ||
            name == plant_reanim_name::anim_shooting ||
            name == plant_reanim_name::anim_shootinghigh;

    case plant_type::cattail:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::cherry_bomb:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_explode;

    case plant_type::chomper:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_bite ||
            name == plant_reanim_name::anim_chew ||
            name == plant_reanim_name::anim_swallow;

    case plant_type::cob_cannon:
        return name == plant_reanim_name::anim_charge ||
            name == plant_reanim_name::anim_unarmed_idle ||
            name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::coffee_bean:
        return name == plant_reanim_name::anim_crumble ||
            name == plant_reanim_name::anim_idle;

    case plant_type::doomshroom:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_sleep ||
            name == plant_reanim_name::anim_explode;

    case plant_type::flower_pot:
        return name == plant_reanim_name::anim_idle;

    case plant_type::fumeshroom:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_sleep ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::garlic:
        return name == plant_reanim_name::anim_idle;

    case plant_type::gatling_pea:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::gloomshroom:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_sleep ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::gold_magnet:
        return name == plant_reanim_name::anim_idle;

    case plant_type::grave_buster:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_land;

    case plant_type::hypnoshroom:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_sleep;

    case plant_type::iceshroom:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_sleep;

    case plant_type::imitater:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_explode;

    case plant_type::jalapeno:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_explode;

    case plant_type::kernelpult:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::lily_pad:
        return name == plant_reanim_name::anim_idle;

    case plant_type::magnetshroom:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_sleep ||
            name == plant_reanim_name::anim_nonactive_idle2 ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::marigold:
        return name == plant_reanim_name::anim_idle;

    case plant_type::melonpult:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::pea_shooter:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::plantern:
        return name == plant_reanim_name::anim_idle;

    case plant_type::potato_mine:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_glow ||
            name == plant_reanim_name::anim_armed ||
            name == plant_reanim_name::anim_rise;

    case plant_type::puffshroom:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_sleep ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::pumpkin:
        return name == plant_reanim_name::anim_idle;

    case plant_type::repeater:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::scaredyshroom:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_scared ||
            name == plant_reanim_name::anim_scaredidle ||
            name == plant_reanim_name::anim_sleep ||
            name == plant_reanim_name::anim_grow ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::seashroom:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_sleep ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::snow_pea:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::spikerock:
        return name == plant_reanim_name::anim_attack ||
            name == plant_reanim_name::anim_idle;

    case plant_type::spikeweed:
        return name == plant_reanim_name::anim_attack ||
            name == plant_reanim_name::anim_idle;

    case plant_type::split_pea:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_splitpea_shooting ||
            name == plant_reanim_name::anim_shooting;

    case plant_type::squash:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_lookleft ||
            name == plant_reanim_name::anim_lookright ||
            name == plant_reanim_name::anim_jumpup ||
            name == plant_reanim_name::anim_jumpdown;

    case plant_type::starfruit:
        return name == plant_reanim_name::anim_shoot ||
            name == plant_reanim_name::anim_idle;

    case plant_type::sunflower:
        return name == plant_reanim_name::anim_idle;

    case plant_type::sunshroom:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_bigidle ||
            name == plant_reanim_name::anim_sleep ||
            name == plant_reanim_name::anim_grow;

    case plant_type::tallnut:
        return name == plant_reanim_name::anim_idle;

    case plant_type::tangle_kelp:
        return name == plant_reanim_name::anim_idle;

    case plant_type::threepeater:
        return name == plant_reanim_name::anim_idle;

    case plant_type::torchwood:
        return name == plant_reanim_name::anim_idle;

    case plant_type::twin_sunflower:
        return name == plant_reanim_name::anim_idle;

    case plant_type::umbrella_leaf:
        return name == plant_reanim_name::anim_block ||
            name == plant_reanim_name::anim_idle;

    case plant_type::wallnut:
        return name == plant_reanim_name::anim_idle;

    case plant_type::winter_melon:
        return name == plant_reanim_name::anim_idle ||
            name == plant_reanim_name::anim_shooting;

    default:
        return false;
    }
}

}
