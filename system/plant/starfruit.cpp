#define _USE_MATH_DEFINES
#include <cmath>
#include "plant.h"
#include "system/damage.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

bool plant_starfruit::has_target(object::plant& p) {
    if (p.countdown.eaten > 0) {
        return true;
    }

    auto flags = p.get_attack_flags(false);

    auto px = p.x + 40;
    auto py = p.y + 40;

    for (auto& z : scene.zombies) {
        if (!damage(scene).can_be_attacked(z, flags)) {
            continue;
        }

        rect zr;
        z.get_hit_box(zr);

        if (z.row == p.row) {
            if (zr.x + zr.width < px) {
                return true;
            }

            continue;
        }

        if (z.type == zombie_type::digger) {
            zr.width += 10;
        }

        int hzw = zr.width / 2 + zr.x;
        int hzh = zr.height / 2 + zr.y;

        auto t = sqrt(
            pow(hzw - static_cast<double>(px), 2) +
            pow(hzh - static_cast<double>(py), 2)
        ) / 3.329999923706055;

        auto predict = zombie_base(scene).predict_after(z, static_cast<float>(t));

        if (px < predict + zr.width / 2 && predict - zr.width / 2 < px) {
            return true;
        }

        int x_predict = static_cast<int>(predict + zr.width / 2 - px);
        int y_predict = static_cast<int>(zr.y + zr.height / 2 - py);

        auto deg = atan2(y_predict, x_predict) * 57.2957763671875;

        if (abs(static_cast<int>(p.row) - static_cast<int>(z.row)) >= 2) {
            if (deg > 25 && deg < 35 || deg > -38 && deg < -28) {
                return true;
            }
        } else if (deg > 20 && deg < 40 || deg > -45 && deg < -25) {
            return true;
        }
    }

    return false;
}

void plant_starfruit::attack(plant& p) {
    projectile_factory projectile_factory(scene);

    constexpr float v_cos = 2.883864528529686f;
    constexpr float v_sin = 1.6649999618530275f;

    for (int i = 0; i < 5; i++) {
        auto& proj = projectile_factory.create(
            projectile_type::star,
            p.row,
            p.x + 25,
            p.y + 25);

        proj.flags = p.get_attack_flags();
        proj.motion_type = projectile_motion_type::starfruit;

        switch (i) {
        case 0:
            proj.dx = -3.3299999f;
            proj.dy2 = 0;
            break;

        case 1:
            proj.dx = 0;
            proj.dy2 = 3.3299999f;
            break;

        case 2:
            proj.dx = 0;
            proj.dy2 = -3.3299999f;
            break;

        case 3:
            proj.dx = v_cos;
            proj.dy2 = v_sin;
            break;

        case 4:
            proj.dx = v_cos;
            proj.dy2 = -v_sin;
            break;

        default:
            break;
        }
    }
}

void plant_starfruit::set_launch_countdown(plant & p) {
    if (has_target(p)) {
        p.set_reanim(plant_reanim_name::anim_shoot, reanim_type::once, 28);
        p.countdown.launch = 40;
    }
}

}