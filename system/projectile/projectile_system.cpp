#include <vector>
#include <algorithm>
#include "system/util.h"
#include "projectile_system.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

bool projectile_system::is_in_torchwood(object::projectile& proj) {
    if (proj.type != projectile_type::pea &&
        proj.type != projectile_type::snow_pea)
    {
        return false;
    }

    rect proj_rect;
    proj.get_attack_box(proj_rect);

    for (auto& p : scene.plants) {
        if (p.row == proj.row &&
            p.type == plant_type::torchwood &&
            !p.is_smashed &&
            !p.is_dead &&
            proj.last_torchwood_col != p.col)
        {
            rect pr;
            p.get_attack_box(pr);

            if (proj_rect.get_overlap_len(pr) > 10) {
                return true;
            }
        }
    }

    return false;
}

zombie* projectile_system::find_zombie_target(projectile& proj) {
    if (is_in_torchwood(proj)) {
        return nullptr;
    }

    rect proj_rect;
    proj.get_attack_box(proj_rect);

    int min_x;
    zombie* target = nullptr;

    for (auto& z : scene.zombies) {
        if (z.row == proj.row &&
            damage.can_be_attacked(z, proj.flags) &&
            (z.status != zombie_status::snorkel_swim || proj.dy1 > 45) &&
            (proj.type != projectile_type::star ||
                proj.time_since_created >= 25 ||
                proj.dx < 0 ||
                z.type != zombie_type::digger))
        {
            rect zr;
            z.get_hit_box(zr);

            if (proj_rect.get_overlap_len(zr) >= 0 &&
                (target == nullptr || z.int_x < min_x))
            {
                target = &z;
                min_x = z.int_x;
            }
        }
    }

    return target;
}

plant* projectile_system::find_plant_target(projectile& proj) {
    rect proj_rect;
    proj.get_attack_box(proj_rect);

    plant* target = nullptr;

    for (auto& p : scene.plants) {
        if (p.row == proj.row &&
            p.type != plant_type::puffshroom &&
            p.type != plant_type::sunshroom &&
            p.type != plant_type::potato_mine &&
            p.type != plant_type::spikeweed &&
            p.type != plant_type::lily_pad)
        {
            rect pr;
            p.get_hit_box(pr);

            if (proj_rect.get_overlap_len(pr) > 8) {
                target = &p;
                break;
            }
        }
    }

    if (target == nullptr) {
        return nullptr;
    }

    auto status = get_grid_plant_status(scene, target->row, target->col);
    target = status.coffee_bean;
    if (target == nullptr) {
        target = status.content;
        if (target == nullptr) {
            target = status.pumpkin;
            if (target == nullptr) {
                target = status.base;
            }
        }
    }

    return target;
}

bool projectile_system::is_covered_by_suppter(
    const projectile& proj,
    const zombie& z)
{
    rect proj_rect;
    proj.get_attack_box(proj_rect);

    rect zr;
    z.get_hit_box(zr);

    if ((z.type == zombie_type::catapult ||
        z.type == zombie_type::zomboni ||
        z.accessory_2.type == zombie_accessories_type_2::screen_door ||
        z.accessory_2.type == zombie_accessories_type_2::ladder) &&
        proj.type == projectile_type::fire_pea)
    {
        return false;
    }

    if (proj.type == projectile_type::fire_pea && proj.row != z.row ||
        abs(static_cast<int>(proj.row) - static_cast<int>(z.row)) > 1 ||
        !damage.can_be_attacked(z, proj.flags))
    {
        return false;
    }

    return zr.get_overlap_len(proj_rect) >= 0;
}

void projectile_system::suppter_attack(projectile& proj, zombie* main_target) {
    int n = 0;
    std::vector<zombie*> targets;

    for (auto& z : scene.zombies) {
        if (is_covered_by_suppter(proj, z)) {
            targets.emplace_back(&z);

            if (&z != main_target) {
                n++;
            }
        }
    }

    int o = projectile::DAMAGE[static_cast<int>(proj.type)];
    int m = proj.type == projectile_type::fire_pea ?  o : 7 * o;

    int dmg = o / 3;

    if (n * dmg > m) {
        dmg = m / n >= 1 ? m / n : 1;
    }

    for (auto z : targets) {
        auto flags = proj.get_flags_with_zombie(*z);

        if (z == main_target) {
            damage.take(*z, o, flags);
        } else {
            damage.take(*z, dmg, flags);
        }
    }
}

void projectile_system::attack_target_zombie(projectile& proj, zombie* z) {
    if (proj.type == projectile_type::fire_pea &&
        z != nullptr &&
        (z->type == zombie_type::catapult ||
        z->type == zombie_type::zomboni ||
        z->accessory_2.type == zombie_accessories_type_2::ladder ||
        z->accessory_2.type == zombie_accessories_type_2::screen_door) ||
        proj.type != projectile_type::melon &&
        proj.type != projectile_type::wintermelon &&
        proj.type != projectile_type::fire_pea)
    {
        if (z) {
            damage.take(
                *z,
                projectile::DAMAGE[static_cast<int>(proj.type)],
                proj.get_flags_with_zombie(*z));
        }
    } else {
        if (proj.type == projectile_type::fire_pea && z != nullptr) {
            debuff.remove_by_fire_pea(*z);
        }

        suppter_attack(proj, z);
    }

    if (proj.type == projectile_type::butter) {
        if (z != nullptr) {
            debuff.set_butter(*z);
        }
    }

    projectile_factory.destroy(proj);
}

void projectile_system::parabola_do_attack(projectile& proj, zombie* z) {
    if (proj.type == projectile_type::cob_cannon) {
        damage.take_instant_kill(
            proj.row,
            static_cast<int>(proj.x + 80),
            static_cast<int>(proj.y + 40),
            115,
            1,
            true,
            proj.flags);
        
        attack_target_zombie(proj, nullptr);
    } else {
        attack_target_zombie(proj, z);
    }
}

void projectile_system::do_parabola_motion(projectile& proj) {
    if (proj.type == projectile_type::cob_cannon && proj.dy1 < -700) {
        proj.ddy = 8;
        proj.row = proj.cannon_row;
        proj.x = proj.cannon_x;

        proj.y = static_cast<float>(get_y_by_row_and_col(
            scene.type,
            proj.cannon_row,
            std::max(0, get_col_by_x(static_cast<int>(proj.cannon_x)))));

        proj.shadow_y = proj.y + 67;
    }

    proj.ddy += proj.dddy;
    proj.x += proj.dx;
    proj.y += proj.dy2;
    proj.dy1 += proj.ddy;

    bool ddy_below_0 = proj.ddy < 0;
    if (ddy_below_0 &&
        (proj.type == projectile_type::basketball ||
        proj.type == projectile_type::cob_cannon))
    {
        return;
    }

    if (proj.time_since_created > 20) {
        if (ddy_below_0) {
            return;
        }

        float top_dy1;
        switch (proj.type) {
        case projectile_type::butter:
            top_dy1 = -32;
            break;

        case projectile_type::basketball:
            top_dy1 = 60;
            break;

        case projectile_type::melon:
        case projectile_type::wintermelon:
            top_dy1 = -35;
            break;

        case projectile_type::cabbage:
        case projectile_type::kernel:
            top_dy1 = -30;
            break;

        case projectile_type::cob_cannon:
            top_dy1 = -60;
            break;

        default:
            top_dy1 = 0;
            break;
        }

        if (scene.is_water_grid(proj.row, 0)) {
            top_dy1 += 40;
        }

        if (top_dy1 >= proj.dy1) {
            return;
        }
    }

    plant* plant_target = nullptr;
    zombie* zombie_target = nullptr;
    if (proj.type == projectile_type::basketball) {
        plant_target = find_plant_target(proj);
    } else {
        zombie_target = find_zombie_target(proj);
    }

    if (zombie_target) {
        parabola_do_attack(proj, zombie_target);
    } else if (plant_target == nullptr) {
        if (proj.dy1 <= (proj.type == projectile_type::cob_cannon ? -40 : 80)) {
            return;
        }

        parabola_do_attack(proj, zombie_target);
    } else {
        for (auto& p : scene.plants) {
            if (p.type == plant_type::umbrella_leaf &&
                !p.is_smashed &&
                p.edible != plant_edible_status::invisible_and_not_edible &&
                !p.is_dead &&
                abs(static_cast<int>(p.col) -
                    static_cast<int>(plant_target->col)) <= 1 &&
                abs(static_cast<int>(p.row) -
                    static_cast<int>(plant_target->row)) <= 1)
            {
                if (p.status == plant_status::umbrella_leaf_shrink) {
                    projectile_factory.destroy(proj);
                } else if (p.status != plant_status::umbrella_leaf_block) {
                    damage.activate_plant(p);
                }

                return;
            }
        }

        plant_target->hp -= projectile::DAMAGE[static_cast<int>(proj.type)];
        projectile_factory.destroy(proj);
    }
}

void projectile_system::others_do_attack(projectile& proj) {
    if (proj.motion_type == projectile_motion_type::puff &&
        proj.time_since_created >= 75 ||
        proj.x > 800 ||
        proj.attack_box.width + proj.x < 0)
    {
        projectile_factory.destroy(proj);
        return;
    }

    if (proj.motion_type == projectile_motion_type::cattail) {
        if (scene.zombies.is_active(proj.target) &&
            damage.can_be_attacked(*proj.target, proj.flags))
        {
            rect proj_rect;
            proj.get_attack_box(proj_rect);

            rect zr;
            proj.target->get_hit_box(zr);

            if (zr.get_overlap_len(proj_rect) >= 0 &&
                zr.y < proj.y &&
                zr.height + zr.y > proj.y)
            {
                attack_target_zombie(proj, proj.target);
            }
        }

        return;
    }

    if (proj.type == projectile_type::star && (proj.y > 600 || proj.y < 40)) {
        projectile_factory.destroy(proj);
        return;
    }

    if (static_cast<int>(proj.type) > 0 && proj.type != projectile_type::star ||
        proj.shadow_y - proj.y <= 90)
    {
        if (auto target = find_zombie_target(proj)) {
            attack_target_zombie(proj, target);
        }
    }
}

void projectile_system::roof_set_disappear(projectile& proj) {
    auto diff = proj.shadow_y - proj.y;

    if ((proj.type == projectile_type::pea ||
        proj.type == projectile_type::snow_pea ||
        proj.type == projectile_type::fire_pea ||
        proj.type == projectile_type::cactus ||
        proj.type == projectile_type::cob_cannon) &&
        diff < 28)
    {
        projectile_factory.destroy(proj);
    } else if (proj.type == projectile_type::puff && diff < 0) {
        projectile_factory.destroy(proj);
    } else if (proj.type == projectile_type::star && diff < 23) {
        projectile_factory.destroy(proj);
    }
}

static void get_vector_cos_and_sin(float x, float y, float& c, float& s) {
    auto d = sqrt(x * x + y * y);

    if (d == 0) {
        c = x;
        s = y;
    }
    else {
        c = x / d;
        s = y / d;
    }
}

void projectile_system::do_other_motion(projectile& proj) {
    switch (proj.motion_type) {
    case projectile_motion_type::left_straight:
        proj.x -= 3.329999923706055f;
        break;

    case projectile_motion_type::cattail:
        if (scene.zombies.is_active(proj.target) &&
            damage.can_be_attacked(*proj.target, proj.flags))
        {
            rect zr;
            proj.target->get_hit_box(zr);

            float a2[2];

            get_vector_cos_and_sin(
                static_cast<float>(
                    zr.x + zr.width / 2.0 - proj.attack_box.width / 2 - proj.x),
                static_cast<float>(
                    zr.y + zr.height / 2 - proj.attack_box.height / 2 - proj.y),
                a2[0],
                a2[1]);

            float res[2];

            get_vector_cos_and_sin(
                proj.dx + a2[0] * proj.time_since_created * 0.001000000047497451f,
                proj.dy2 + a2[1] * proj.time_since_created * 0.001000000047497451f,
                res[0],
                res[1]);

            proj.dx = res[0] * 2;
            proj.dy2 = res[1] * 2;
        }
        proj.y += proj.dy2;
        proj.x += proj.dx;
        proj.shadow_y += proj.dy2;

        proj.row = std::max(
            0,
            get_row_by_x_and_y(
                scene.type,
                static_cast<int>(std::max(40.0f, proj.x)),
                static_cast<int>(proj.y)));

        break;

    case projectile_motion_type::starfruit:
        proj.y += proj.dy2;
        proj.x += proj.dx;
        proj.shadow_y += proj.dy2;

        if (proj.dy2 != 0) {
            proj.row = get_star_row_by_x_and_y(
                static_cast<int>(proj.x),
                static_cast<int>(proj.y));
        }

        break;

    default:
        proj.x += 3.329999923706055f;

        if (proj.motion_type == projectile_motion_type::switch_way) {
            proj.y += proj.dy2;
            proj.dy2 *= 0.9700000286102295;
            proj.shadow_y += proj.dy2;
        }

        break;
    }

    others_do_attack(proj);
    roof_set_disappear(proj);
}

void projectile_system::update() {
    for (auto& proj : scene.projectiles) {
        proj.time_since_created++;

        if (proj.countdown > 0) {
            proj.countdown--;
        }

        auto row = proj.row;
        float y_before = get_y_by_row_and_x(scene.type, proj.row, proj.x);

        if (proj.motion_type == projectile_motion_type::parabola) {
            do_parabola_motion(proj);
        } else {
            do_other_motion(proj);
        }

        float y_after = get_y_by_row_and_x(scene.type, row, proj.x);

        float diff = y_after - y_before;

        if (proj.motion_type == projectile_motion_type::parabola) {
            proj.y += diff;
            proj.dy1 = proj.dy1 - diff;
        }

        proj.shadow_y += diff;

        proj.int_x = static_cast<int>(proj.x);
        proj.int_y = static_cast<int>(proj.dy1 + proj.y);
    }
}

}
