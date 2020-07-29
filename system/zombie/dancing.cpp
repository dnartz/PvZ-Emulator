#include "zombie.h"
#include "system/zombie/zombie_factory.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

bool zombie_dancing::need_summon(object::zombie& z) {
    for (int i = 0; i < 4; i++) {
        if (scene.zombies.get(z.partners[i]) == nullptr) {
            int row;

            if (i > 0) {
                if (i != 1) {
                    return true;
                }
                else {
                    row = z.row + 1;
                }
            }
            else {
                row = z.row - 1;
            }

            switch (scene.type) {
            case scene_type::fog:
            case scene_type::pool:
                return row == 0 || row == 1 || row == 4 || row == 5;

            default:
                return row >= 0 && row <= 4;
            }
        }
    }

    return false;
}

void zombie_dancing::create_backup_dancers(zombie& z) {
    if (!z.is_not_dying) {
        return;
    }

    for (int i = 0; i < 4; i++) {
        if (scene.zombies.get(z.partners[i])) {
            continue;
        }

        int row;
        float x;

        switch (i) {
        case 0:
            row = z.row - 1;
            x = z.x;
            break;

        case 1:
            row = z.row + 1;
            x = z.x;
            break;

        case 2:
            row = z.row;
            x = z.x - 100;
            break;

        case 3:
            row = z.row;
            x = z.x + 100;
            break;

        default:
            continue;
        }

        switch (scene.type) {
        case scene_type::fog:
        case scene_type::pool:
            if (row != 0 && row != 1 && row != 4 && row != 5) {
                continue;
            }
            break;

        default:
            if (row < 0 || row > 4) {
                continue;
            }
        }

        auto& backup = zombie_factory(scene).create(zombie_type::backup_dancer, false);

        backup.row = row;

        backup.x = x;
        backup.y = zombie_init_y(scene.type, z, row);

        backup.int_x = static_cast<int>(x);
        backup.int_y = static_cast<int>(backup.y);
        
        backup.dy = -200;
        backup.status = zombie_status::backup_spawning;

        backup.master_id = static_cast<int>(scene.zombies.get_index(z));

        backup.countdown.action = 150;
        backup.reanim.prev_fps = 0;
        reanim.set_fps(backup, 0);

        backup.is_hypno = z.is_hypno;

        scene.zombie_map[backup.row].insert(&backup);

        z.partners[i] = static_cast<int>(scene.zombies.get_index(backup));
    }
}

unsigned int zombie_dancer_base::get_clock(zombie& z) {
    if (z.countdown.freeze > 0 || z.countdown.butter > 0) {
        return 0;
    } else if (z.status == zombie_status::dancing_moonwalk) {
        return scene.zombie_dancing_clock % 110 / 10;
    } else {
        return scene.zombie_dancing_clock % 460 / 20;
    }
}

zombie_status zombie_dancer_base::get_status_by_clock(zombie& z) {
    auto t = get_clock(z);

    if (t > 11) {
        if (t > 12) {
            if (t > 15) {
                if (t > 18) {
                    if (t <= 21) {
                        return zombie_status::dancing_armrise5;
                    } else {
                        return zombie_status::dancing_armrise4;
                    }
                } else {
                    return zombie_status::dancing_armrise2;
                }
            } else {
                return zombie_status::dancing_armrise3;
            }
        } else {
            return zombie_status::dancing_armrise1;
        }
    } else {
        return zombie_status::dancing_walking;
    }
}

void zombie_dancing::update(object::zombie& z) {
    if (z.catapult_or_jackson.summon_countdown > 0 &&
        --z.catapult_or_jackson.summon_countdown == 0)
    {
        if (get_clock(z) == 12 && z.is_not_dying && z.x < 700) {
            z.status = zombie_status::dancing_wait_summoning;
            reanim.set(z, zombie_reanim_name::anim_point, reanim_type::once, 24);
        } else {
            z.catapult_or_jackson.summon_countdown = 1;
        }
    }

    if (z.status == zombie_status::dancing_moonwalk) {
        if (z.is_not_dying && z.countdown.action == 0) {
            z.status = zombie_status::dancing_point;
            reanim.set(z, zombie_reanim_name::anim_point, reanim_type::once, 24);
            reanim.update_dx(z);
        }
        return;
    }

    if (z.status != zombie_status::dancing_point &&
        z.status != zombie_status::dancing_wait_summoning)
    {
        if (z.status == zombie_status::dancing_summoning) {
            if (z.countdown.action) {
                return;
            }

            z.status = zombie_status::dancing_walking;
            reanim.set(z, zombie_reanim_name::anim_walk, reanim_type::repeat, 0);
        }

        auto next_status = get_status_by_clock(z);
        if (z.status != next_status) {
            z.status = next_status;

            if (next_status == zombie_status::dancing_walking) {
                reanim.set(z, zombie_reanim_name::anim_walk, reanim_type::repeat, 0);
            } else if (next_status == zombie_status::dancing_armrise1) {
                reanim.set(z, zombie_reanim_name::anim_armraise, reanim_type::repeat, 18);
                z.reanim.progress = 0.60000002f;
            } else {
                reanim.set(z, zombie_reanim_name::anim_armraise, reanim_type::repeat, 18);
            }
        }

        if (z.is_not_dying &&
            z.catapult_or_jackson.summon_countdown == 0 &&
            need_summon(z))
        {
            z.catapult_or_jackson.summon_countdown = 100;
        }

        return;
    }

    if (z.reanim.n_repeated > 0) {
        create_backup_dancers(z);
        z.status = zombie_status::dancing_summoning;
        z.countdown.action = 200;
    }
}

void zombie_dancing::init(zombie& z, unsigned int row) {
    z._ground = _ground.data();

    zombie_base::init(z, zombie_type::dancing, row);

    z.hp = 500;

    z.dx = 0.5;
    z.status = zombie_status::dancing_moonwalk;

    z.countdown.action = rng.randint(12) + 300;

    reanim.set(z, zombie_reanim_name::anim_moonwalk, reanim_type::repeat, 24);

    set_common_fields(z);
}

}