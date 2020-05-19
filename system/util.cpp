#include <cmath>
#include <algorithm>

#include "system/rng.h"
#include "object/scene.h"
#include "util.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

int get_col_by_x(int x) {
    if (x >= 40) {
        return std::max(0, std::min(8, (x - 40) / 80));
    } else {
        return -1;
    }
}

float get_y_by_col(scene_type s, unsigned int row, unsigned int col) {
    float y;

    if (s == scene_type::roof || s == scene_type::moon_night) {
        float offset = static_cast<float>(col < 5 ? 20 * (5 - col) : 0);
        y = (85 * row + offset + 80) - 10;
    } else if (s == scene_type::fog || s == scene_type::pool) {
        y = static_cast<float>(85 * row + 80);
    } else {
        y = static_cast<float>(100 * row + 80);
    }

    return y;
}

float get_y_by_row_and_x(scene_type s, unsigned int row, float x) {
    if (s == scene_type::roof || s == scene_type::moon_night) {
        float offset = static_cast<float>(x < 440 ?
            (440.0f - static_cast<long double>(x)) * 0.25 :
            0);
        return get_y_by_col(s, row, 8) + offset;
    } else {
        return get_y_by_col(s, row, 0);
    }
}

int get_y_by_row_and_col(scene_type s, int row, int col) {
    if (s == scene_type::roof || s == scene_type::moon_night) {
        return 85 * row + (col < 5 ? 20 * (5 - col) : 0) + 70;
    } else if (s == scene_type::fog || s == scene_type::pool) {
        return 85 * row + 80;
    } else {
        return 100 * row + 80;
    }
}

int get_row_by_x_and_y(scene_type s, int x, int y) {
    int col = get_col_by_x(x);
    if (col == -1 || y < 80) {
        return -1;
    }

    switch (s) {
    case scene_type::day:
    case scene_type::night:
        return std::max(0, (y - 80) / 100);

    case scene_type::pool:
    case scene_type::fog:
        return std::max(0, std::min(5, (y - 80) / 85));

    case scene_type::roof:
    case scene_type::moon_night: {
        int row;

        if (col < 5) {
            row = (y - 20 * (4 - col) - 80) / 85;
        } else {
            row = (y - 80) / 85;
        }

        return std::max(0, std::min(4, row));
    }

    default:
        assert(false);
        return -1;
    }
}

float zombie_init_y(scene_type type, const zombie& z, unsigned int row) {
	auto y = get_y_by_row_and_x(type, row, z.x + 40) - 30;

	if (z.type == zombie_type::balloon) {
		y -= 30;
	} else if (z.type == zombie_type::pogo) {
		y -= 16;
	}

	return y;
}

bool is_slowed(scene &scene, zombie &z) {
    if (z.countdown.slow > 0) {
        return true;
    }

    object::zombie* p;
    if (z.type == zombie_type::backup_dancer) {
        p = scene.zombies.get(z.master_id);

        if (p == nullptr) {
            return false;
        }

        if (p->countdown.slow > 0) {
            return true;
        }
    } else if (z.type == zombie_type::dancing) {
        p = &z;
    } else {
        return false;
    }

    for (auto i : p->partners) {
        auto t = scene.zombies.get(i);
        if (t && t->countdown.slow > 0) {
            return true;
        }
    }

    return false;
}

bool is_not_movable(scene& scene, zombie& z) {
    if (z.is_eating ||
        z.countdown.freeze > 0 ||
        z.countdown.butter > 0 ||
        z.status == zombie_status::jackbox_pop ||
        z.status == zombie_status::newspaper_destoryed ||
        z.status == zombie_status::gargantuar_throw ||
        z.status == zombie_status::gargantuar_smash ||
        z.status == zombie_status::catapult_shoot ||
        z.status == zombie_status::catapult_idle ||
        z.status == zombie_status::digger_drill ||
        z.status == zombie_status::digger_lost_dig ||
        z.status == zombie_status::digger_landing ||
        z.status == zombie_status::digger_dizzy ||
        z.status == zombie_status::dancing_point ||
        z.status == zombie_status::dancing_wait_summoning ||
        z.status == zombie_status::dancing_summoning ||
        z.status == zombie_status::backup_spawning ||
        z.status == zombie_status::imp_flying ||
        z.status == zombie_status::imp_landing ||
        z.status == zombie_status::ladder_placing ||
        z.action == zombie_action::fall_from_sky ||
        z.status == zombie_status::dancing_armrise1 ||
        z.status == zombie_status::dancing_armrise2 ||
        z.status == zombie_status::dancing_armrise3 ||
        z.status == zombie_status::dancing_armrise4 ||
        z.status == zombie_status::dancing_armrise5 ||
        static_cast<int>(z.action) == 8 ||
        z.type == zombie_type::bungee)
    {
        return true;
    }

    zombie* p;

    if (z.type == zombie_type::dancing) {
        p = &z;
    } else if (z.type == zombie_type::backup_dancer) {
        p = scene.zombies.get(z.master_id);
    } else {
        return false;
    }

    if (p == nullptr) {
        return false;
    }

    if (p->is_eating || p->countdown.butter > 0 || p->countdown.freeze > 0) {
        return true;
    }

    for (auto i : p->partners) {
        auto t = scene.zombies.get(i);

        if (t && (t->is_eating ||
            t->countdown.butter > 0 ||
            t->countdown.freeze > 0))                
        {
            return true;
        }
    }

    return false;
}

bool is_target_of_kelp(scene &s, zombie &z) {
    if (s.type != scene_type::fog && s.type != scene_type::pool ||
        z.row != 2 && z.row != 3)
    {
        return false;
    }

    if (z.action == zombie_action::caught_by_kelp) {
        return true;
    }

    for (int i = 0; i < 9; i++) {
        if (s.plant_map[z.row][i].content &&
            s.plant_map[z.row][i].content->type == plant_type::tangle_kelp &&
            s.plant_map[z.row][i].content->target == &z)
        {
            return true;
        }
    }

    return false;
}

}