#include "world.h"
#include <unordered_set>
#include <unordered_map>

#ifdef PVZEMU_BUILD_DEBUGGER
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#endif // PVZEMU_BUILD_DEBUGGER

#include "world.h"

using namespace pvz_emulator::object;

namespace pvz_emulator {

void world::clean_obj_lists() {
    scene.plants.shrink_to_fit();
    scene.zombies.shrink_to_fit();
    scene.projectiles.shrink_to_fit();
    scene.griditems.shrink_to_fit();
}

bool world::update() {
    if (scene.is_game_over) {
        return true;
    }

    scene.zombie_dancing_clock += 1;

    clean_obj_lists();

    griditem.update();

    plant_system.update();

    if (zombie.update()) {
        scene.is_game_over = true;
        return true;
    }

    projectile.update();

    for (auto& card : scene.cards) {
        if (card.cold_down > 0) {
            --card.cold_down;
        }
    }

    sun.update();

    if (!scene.stop_spawn) {
        spawn.update();
    }

    ice_path.update();

    if (scene.spawn.countdown.pool > 0) {
        --scene.spawn.countdown.pool;
    }

    if (endgame.update()) {
        spawn.reset();
        return true;
    } else {
        return false;
    }
}
void world::update_all(
    std::vector<world *>& w,
    batch_action_vector& actions,
    std::vector<int>& res,
    batch_action_vector& available_actions)
{
    std::atomic<decltype(w.size())> i = 0;
    res.resize(w.size());
    available_actions.resize(w.size(), std::vector<std::tuple<int, int, int>>());

    std::vector<std::thread> threads;
    for (unsigned int j = 0; j < std::thread::hardware_concurrency(); j++) {
        threads.emplace_back([&]() {
            for (auto k = i.fetch_add(1); k < w.size(); k = i.fetch_add(1)) {
                res[k] = w[k]->update();

                for (auto& t : actions[k]) {
                    auto op = std::get<0>(t);
                    auto row = std::get<1>(t);
                    auto col = std::get<2>(t);

                    if (op < 10) {
                        auto& card = w[k]->scene.cards[op];
                        if (card.cold_down == 0 &&
                            card.type != plant_type::none &&
                            w[k]->plant_factory.can_plant(
                                row, col, card.type, card.imitater_type)) {
                            available_actions[k].emplace_back(t);
                        }
                    }
                    else if (op == 10) {
                        if (w[k]->scene.plant_map[row][col].base ||
                            w[k]->scene.plant_map[row][col].coffee_bean ||
                            w[k]->scene.plant_map[row][col].content) {
                            available_actions[k].emplace_back(t);
                        }
                    }
                    else if (w[k]->scene.plant_map[row][col].pumpkin) {
                        available_actions[k].emplace_back(t);
                    }
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }
}

#ifdef PVZEMU_BUILD_DEBUGGER
void world::to_json(std::string& s) {
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

    scene.to_json(writer);

    s = sb.GetString();
}
#endif // PVZEMU_BUILD_DEBUGGER

using namespace pvz_emulator::object;

bool world::select_plants(
    std::vector<plant_type> cards,
    plant_type imitater_type)
{
    auto n = cards.size();
    if (n > 10) {
        return false;
    }

    std::unordered_set<plant_type> unique_set;

    for (int i = 0; i < n; i++) {
        if (static_cast<int>(cards[i]) < 0 || static_cast<int>(cards[i]) > 48) {
            return false;
        }

        if (cards[i] == plant_type::imitater &&
            (static_cast<int>(imitater_type) < 0 ||
            static_cast<int>(imitater_type) > 47))
        {
            return false;
        }

        if (unique_set.find(cards[i]) != unique_set.end()) {
            return false;
        }

        unique_set.insert(cards[i]);
    }

    unsigned int prev_imitater_cd = 0;
    plant_type prev_imitater_type = plant_type::none;

    std::unordered_map<plant_type, unsigned int> cd;

    for (auto &card : scene.cards) {
        if (card.type != plant_type::none) {
            if (card.type == plant_type::imitater) {
                prev_imitater_type = card.imitater_type;
                prev_imitater_cd = card.cold_down;
            } else {
                cd[card.type] = card.cold_down;
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        if (i < n) {
            if (cards[i] == plant_type::imitater) {
                scene.cards[i].type = plant_type::imitater;
                scene.cards[i].imitater_type = imitater_type;

                if (imitater_type == prev_imitater_type) {
                    scene.cards[i].cold_down = prev_imitater_cd;
                } else {
                    scene.cards[i].cold_down = 0;
                }
            } else {
                scene.cards[i].type = cards[i];
                scene.cards[i].imitater_type = plant_type::none;
                scene.cards[i].cold_down = cd.find(cards[i]) == cd.end() ?
                    0 :
                    cd[cards[i]];
            }
        } else {
            scene.cards[i].type =
                scene.cards[i].imitater_type =
                plant_type::none;

            scene.cards[i].cold_down = 0;
        }
    }

    return true;
}

bool world::plant(object::plant_type type, unsigned int row, unsigned int col) {
    for (int i = 0; i < 10; i++) {
        if (scene.cards[i].type == type) {
            return plant_factory.plant(i, row, col) != nullptr;
        }
    }

    return false;
}

}