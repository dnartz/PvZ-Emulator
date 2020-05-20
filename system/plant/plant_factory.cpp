#include <cassert>

#include "plant_factory.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

bool plant_factory::can_plant_advanced_plant(
    grid_plant_status& status,
    plant_type advanced)
{
    if (advanced != plant_type::cattail &&
        (status.content == nullptr ||
        status.content->edible == plant_edible_status::invisible_and_edible))
    {
        return false;
    }

    switch (advanced) {
    case plant_type::gatling_pea:
        return status.content->type == plant_type::gatling_pea;

    case plant_type::twin_sunflower:
        return status.content->type == plant_type::sunflower;

    case plant_type::gloomshroom:
        return status.content->type == plant_type::fumeshroom;

    case plant_type::winter_melon:
        return status.content->type == plant_type::melonpult;

    case plant_type::gold_magnet:
        return status.content->type == plant_type::magnetshroom;

    case plant_type::spikerock:
        return status.content->type == plant_type::spikeweed;

    case plant_type::cattail: {
        return status.base != nullptr &&
            status.base->type == plant_type::lily_pad &&
            status.content == nullptr;
    }

    case plant_type::cob_cannon:
        if (status.content->type != plant_type::kernelpult || 
            status.content->col >= 8)
        {
            return false;
        }

        for (auto& b : scene.plants) {
            if (b.type == status.content->type &&
                b.row == status.content->row &&
                b.col == status.content->col + 1)
            {
                auto& s1 = scene.plant_map[b.row][b.col];

                return s1.base == nullptr && status.base == nullptr ||
                    s1.base != nullptr && status.base != nullptr;
            }
        }

    default:
        return false;
    }
}

bool plant_factory::is_pos_valid(unsigned row, unsigned col) {
    if (scene.type == scene_type::pool || scene.type == scene_type::fog) {
        if (row > 5) {
            return false;
        }
    } else if (row > 4) {
        return false;
    }

    if (col > 8) {
        return false;
    }

    return true;
}

bool plant_factory::is_not_covered_by_ice_path(unsigned row, unsigned col) {
    static const int ICE_PATH_TABLE[] = {
        108, 188, 268, 348, 428, 508, 588, 668, 751
    };

    return scene.ice_path.countdown[row] <= 0 ||
        scene.ice_path.x[col] >= ICE_PATH_TABLE[col];
}

void plant_factory::is_covered_by_griditem(
    unsigned row,
    unsigned col,
    bool& has_grave,
    bool& has_crater)
{
    has_grave = false;
    has_crater = false;

    for (auto& item : scene.griditems) {
        if (item.col == col && item.row == row) {
            if (item.type == griditem_type::grave) {
                has_grave = true;
            } else if (item.type == griditem_type::crater) {
                has_crater = true;
            }

            if (has_grave && has_crater) {
                return;
            }
        }
    }
}

unsigned int plant_factory::get_cost(object::plant_type type) {
    if (type >= plant_type::gatling_pea) {
        int n = 0;

        for (auto& p : scene.plants) {
            if (p.type == type) {
                n++;
            }
        }

        return plant::COST_TABLE[static_cast<int>(type)] + 50 * n;
    } else {
        return plant::COST_TABLE[static_cast<int>(type)];
    }
}

bool plant_factory::can_plant(
    unsigned int row,
    unsigned int col,
    plant_type type,
    plant_type imitater_type)
{
    if (!is_pos_valid(row, col) || !is_not_covered_by_ice_path(row, col)) {
        return false;
    }

    bool has_grave, has_crater;
    is_covered_by_griditem(row, col, has_grave, has_crater);

    if (has_crater) {
        return false;
    }

    auto& status = scene.plant_map[row][col];

    auto target_type = type == plant_type::imitater ?
        imitater_type :
        type;

    if (get_cost(target_type) > scene.sun.sun) {
        return false;
    }

    if (target_type == plant_type::grave_buster) {
        return status.content == nullptr && has_grave;
    }

    if (has_grave) {
        return false;
    }

    bool is_roof_scene = scene.type == scene_type::roof ||
        scene.type == scene_type::moon_night;

    bool is_water_row =
        (scene.type == scene_type::pool || scene.type == scene_type::fog) &&
        (row == 2 || row == 3);

    bool has_pot = status.base &&
        status.base->type == plant_type::flower_pot &&
        status.base->edible != plant_edible_status::invisible_and_edible;

    bool has_lily = status.base &&
        status.base->type == plant_type::lily_pad &&
        status.base->edible != plant_edible_status::invisible_and_edible;

    switch (target_type) {
    case plant_type::lily_pad:
    case plant_type::tangle_kelp:
    case plant_type::seashroom:
        return is_water_row &&
            status.base == nullptr &&
            status.content == nullptr;

    case plant_type::spikeweed:
        return !is_water_row &&
            !is_roof_scene &&
            status.base == nullptr &&
            status.content == nullptr;

    case plant_type::spikerock:
        return can_plant_advanced_plant(status, plant_type::spikerock) &&
            !is_water_row &&
            !is_roof_scene &&
            status.base == nullptr;

    case plant_type::flower_pot:
        return !is_water_row &&
            status.pumpkin == nullptr &&
            status.content == nullptr &&
            status.base == nullptr;

    case plant_type::coffee_bean:
        return status.coffee_bean == nullptr &&
            status.content &&
            status.content->is_sleeping &&
            status.content->countdown.awake == 0 &&
            status.content->edible != plant_edible_status::invisible_and_edible;

    case plant_type::pumpkin:
        return (!is_roof_scene || has_pot) &&
            (!is_water_row ||
                status.content && status.content->type == plant_type::cattail ||
                has_lily) &&
            (status.content == nullptr ||
            status.content->type != plant_type::cob_cannon) &&
            (status.pumpkin == nullptr ||
                status.pumpkin->type == plant_type::pumpkin &&
                status.pumpkin->hp < 2 * 4000 / 3 &&
                status.pumpkin->edible !=
                plant_edible_status::invisible_and_edible);
        
    default:
        if (is_roof_scene && !has_pot || is_water_row && !has_lily) {
            return false;
        }
    }

    switch (target_type) {
    case plant_type::wallnut:
        return status.content == nullptr ||
            status.content->type == plant_type::wallnut &&
            status.content->hp < 2 * 4000 / 3 &&
            status.content->edible != plant_edible_status::invisible_and_edible;

    case plant_type::tallnut:
        return status.content == nullptr ||
            status.content->type == plant_type::tallnut &&
            status.content->hp < 2 * 8000 / 3 &&
            status.content->edible != plant_edible_status::invisible_and_edible;

    default:
        if (status.content != nullptr) {
            return false;
        }
    }

    switch (target_type) {
    case plant_type::potato_mine:
        return !is_water_row;

    case plant_type::gatling_pea:
    case plant_type::twin_sunflower:
    case plant_type::gloomshroom:
    case plant_type::cattail:
    case plant_type::winter_melon:
    case plant_type::gold_magnet:
    case plant_type::cob_cannon:
        return can_plant_advanced_plant(status, target_type);

    default:
        return true;
    }
}

plant& plant_factory::create(
    plant_type type,
    int row,
    int col,
    plant_type imitater_target)
{
    auto& p = scene.plants.alloc();

    switch (type) {
    case plant_type::pea_shooter:
        subsystems.pea_shooter.init(p, row, col);
        break;
    case plant_type::sunflower:
        subsystems.sunflower.init(p, row, col);
        break;
    case plant_type::cherry_bomb:
        subsystems.cherry_bomb.init(p, row, col);
        break;
    case plant_type::wallnut:
        subsystems.wallnut.init(p, row, col);
        break;
    case plant_type::potato_mine:
        subsystems.potato_mine.init(p, row, col);
        break;
    case plant_type::snow_pea:
        subsystems.snow_pea.init(p, row, col);
        break;
    case plant_type::chomper:
        subsystems.chomper.init(p, row, col);
        break;
    case plant_type::repeater:
        subsystems.repeater.init(p, row, col);
        break;
    case plant_type::puffshroom:
        subsystems.puffshroom.init(p, row, col);
        break;
    case plant_type::sunshroom:
        subsystems.sunshroom.init(p, row, col);
        break;
    case plant_type::fumeshroom:
        subsystems.fumeshroom.init(p, row, col);
        break;
    case plant_type::grave_buster:
        subsystems.grave_buster.init(p, row, col);
        break;
    case plant_type::hypnoshroom:
        subsystems.hypnoshroom.init(p, row, col);
        break;
    case plant_type::scaredyshroom:
        subsystems.scaredyshroom.init(p, row, col);
        break;
    case plant_type::iceshroom:
        subsystems.iceshroom.init(p, row, col);
        break;
    case plant_type::doomshroom:
        subsystems.doomshroom.init(p, row, col);
        break;
    case plant_type::lily_pad:
        subsystems.lily_pad.init(p, row, col);
        break;
    case plant_type::squash:
        subsystems.squash.init(p, row, col);
        break;
    case plant_type::threepeater:
        subsystems.threepeater.init(p, row, col);
        break;
    case plant_type::tangle_kelp:
        subsystems.tangle_kelp.init(p, row, col);
        break;
    case plant_type::jalapeno:
        subsystems.jalapeno.init(p, row, col);
        break;
    case plant_type::spikeweed:
        subsystems.spickweed.init(p, row, col);
        break;
    case plant_type::torchwood:
        subsystems.torchwood.init(p, row, col);
        break;
    case plant_type::tallnut:
        subsystems.tallnut.init(p, row, col);
        break;
    case plant_type::seashroom:
        subsystems.seashroom.init(p, row, col);
        break;
    case plant_type::plantern:
        subsystems.plantern.init(p, row, col);
        break;
    case plant_type::cactus:
        subsystems.cactus.init(p, row, col);
        break;
    case plant_type::blover:
        subsystems.blover.init(p, row, col);
        break;
    case plant_type::split_pea:
        subsystems.split_pea.init(p, row, col);
        break;
    case plant_type::starfruit:
        subsystems.starfruit.init(p, row, col);
        break;
    case plant_type::pumpkin:
        subsystems.pumpkin.init(p, row, col);
        break;
    case plant_type::magnetshroom:
        subsystems.magnetshroom.init(p, row, col);
        break;
    case plant_type::cabbagepult:
        subsystems.cabbagepult.init(p, row, col);
        break;
    case plant_type::flower_pot:
        subsystems.flower_pot.init(p, row, col);
        break;
    case plant_type::kernelpult:
        subsystems.kernelpult.init(p, row, col);
        break;
    case plant_type::coffee_bean:
        subsystems.coffee_bean.init(p, row, col);
        break;
    case plant_type::garlic:
        subsystems.garlic.init(p, row, col);
        break;
    case plant_type::umbrella_leaf:
        subsystems.umbrella_leaf.init(p, row, col);
        break;
    case plant_type::marigold:
        subsystems.marigold.init(p, row, col);
        break;
    case plant_type::melonpult:
        subsystems.melonpult.init(p, row, col);
        break;
    case plant_type::gatling_pea:
        subsystems.gatling_pea.init(p, row, col);
        break;
    case plant_type::twin_sunflower:
        subsystems.twin_sunflower.init(p, row, col);
        break;
    case plant_type::gloomshroom:
        subsystems.gloomshroom.init(p, row, col);
        break;
    case plant_type::cattail:
        subsystems.cattail.init(p, row, col);
        break;
    case plant_type::winter_melon:
        subsystems.winter_melon.init(p, row, col);
        break;
    case plant_type::gold_magnet:
        subsystems.gold_magnet.init(p, row, col);
        break;
    case plant_type::spikerock:
        subsystems.spikerock.init(p, row, col);
        break;
    case plant_type::cob_cannon:
        subsystems.cob_cannon.init(p, row, col);
        break;
    case plant_type::imitater:
        subsystems.imitater.init(p, imitater_target, row, col);
        break;
    default:
        assert(false);
        break;
    }

    if (p.type == plant_type::pumpkin) {
        assert(!scene.plants.is_active(scene.plant_map[p.row][p.col].pumpkin));
        scene.plant_map[p.row][p.col].pumpkin = &p;
    } else if (p.type == plant_type::coffee_bean) {
        assert(!scene.plants.is_active(scene.plant_map[p.row][p.col].coffee_bean));
        scene.plant_map[p.row][p.col].coffee_bean = &p;
    } else if (p.type == plant_type::flower_pot || p.type == plant_type::lily_pad) {
        assert(!scene.plants.is_active(scene.plant_map[p.row][p.col].base));
        scene.plant_map[p.row][p.col].base = &p;
    } else if (p.type == plant_type::cob_cannon) {
        assert(!scene.plants.is_active(scene.plant_map[p.row][p.col].base));
        assert(!scene.plants.is_active(scene.plant_map[p.row][p.col + 1].base));
        scene.plant_map[p.row][p.col].content = &p;
        scene.plant_map[p.row][p.col + 1].content = &p;
    } else {
        assert(!scene.plants.is_active(scene.plant_map[p.row][p.col].content));
        scene.plant_map[p.row][p.col].content = &p;
    }

    return p;
}

plant* plant_factory::plant(
    unsigned int i,
    unsigned int row,
    unsigned int col)
{
    if (i >= 10 ||
        scene.cards[i].cold_down > 0 ||
        scene.cards[i].type == plant_type::none)
    {
        return nullptr;
    }

    auto type = scene.cards[i].type;
    auto imitater_type = scene.cards[i].imitater_type;
    auto target_type = type == plant_type::imitater ? imitater_type : type;

    if (!can_plant(row, col, type, imitater_type)) {
        return nullptr;
    }

    auto& status = scene.plant_map[row][col];

    bool is_gloom_awake = false;
    unsigned int gloom_awake_countdown;

    switch (target_type) {
    case plant_type::wallnut:
    case plant_type::tallnut:
        if (status.content && status.content->type == target_type) {
            destroy(*status.content);
        }
        break;

    case plant_type::pumpkin:
        if (target_type == plant_type::pumpkin && status.pumpkin) {
            destroy(*status.pumpkin);
        }
        break;

    case plant_type::gatling_pea:
    case plant_type::twin_sunflower:
    case plant_type::gloomshroom:
    case plant_type::winter_melon:
    case plant_type::gold_magnet:
    case plant_type::spikerock:
        if (status.content) {
            if (target_type == plant_type::gloomshroom) {
                gloom_awake_countdown = status.content->countdown.awake;
                is_gloom_awake = !status.content->is_sleeping;
            }

            destroy(*status.content);
        }
        break;

    case plant_type::cattail:
        if (status.base) {
            destroy(*status.base);
        }
        break;
        
    case plant_type::cob_cannon:
        if (status.content) {
            destroy(*status.content);
        }

        if (col + 1 < 9) {
            auto& s = scene.plant_map[row][col + 1];
            if (s.content) {
                destroy(*s.content);
            }
        }

        break;

    default:
        break;
    }

    scene.sun.sun -= get_cost(target_type);

    auto p = &create(type, row, col, imitater_type);

    if (p->type == plant_type::gloomshroom && is_gloom_awake) {
        if (is_gloom_awake) {
            p->set_sleep(false);
        }

        p->countdown.awake = gloom_awake_countdown;
    }

    scene.cards[i].cold_down = plant::CD_TABLE[static_cast<int>(target_type)];

    return p;
}

void plant_factory::destroy(object::plant& p) {
    p.is_dead = true;

    if (p.type == plant_type::tangle_kelp && p.target != -1) {
        if (auto z = scene.zombies.get(p.target)) {
            zombie_factory.destroy(*z);
        }
    }

    if (p.type != plant_type::coffee_bean) {
        for (auto& item : scene.griditems) {
            if (item.row == p.row &&
                item.col == p.col &&
                item.type == griditem_type::ladder)
            {
                griditem_factory.destroy(item);
            }
        }
    }

    auto& status = scene.plant_map[p.row][p.col];
    if (status.coffee_bean == nullptr &&
        status.content == nullptr &&
        status.pumpkin == nullptr &&
        status.base != nullptr &&
        status.base->type == plant_type::flower_pot &&
        !status.base->is_squash_attacking() &&
        status.base->edible != plant_edible_status::invisible_and_not_edible &&
        !status.base->is_dead &&
        !status.base->is_smashed)
    {
        status.base->reanim.fps = rng.randfloat(10, 15);
    }

    if (status.pumpkin == &p) {
        status.pumpkin = nullptr;
    }

    if (status.coffee_bean == &p) {
        status.coffee_bean = nullptr;
    }

    if (status.base == &p) {
        status.base = nullptr;
    }

    if (status.content == &p) {
        status.content = nullptr;
    }

    if (p.type == plant_type::cob_cannon &&
        scene.plant_map[p.row][p.col + 1].content == &p)
    {
        scene.plant_map[p.row][p.col + 1].content = nullptr;
    }
}

}
