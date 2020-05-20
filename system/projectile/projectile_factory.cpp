#include <cassert>
#include "system/util.h"
#include "projectile_factory.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

projectile&
projectile_factory::alloc_and_init(projectile_type type, int row, int x, int y) {
    auto& p = scene.projectiles.alloc();

    p.type = type;
    p.motion_type = projectile_motion_type::straight;

    p.row = row;

    p.x = static_cast<float>(x);
    p.y = static_cast<float>(y);
    p.shadow_y = static_cast<float>(
        get_y_by_row_and_col(scene.type, row, std::max(0, get_col_by_x(x)))) + 67;

    if ((scene.type == scene_type::roof ||
        scene.type == scene_type::moon_night) &&
        x < 480)
    {
        p.shadow_y -= 12;
    }

    p.flags = 0;

    p.dx = p.dy1 = p.dy2 = p.ddy = p.dddy = 0;

    p.cannon_x = 0;
    p.cannon_row = 0;

    p.attack_box.width = p.attack_box.height = 40;
    p.time_since_created = 0;

    p.is_disappeared = false;
    p.is_visible = true;

    if (p.type == projectile_type::cob_cannon) {
        p.attack_box.width = 162;
        p.attack_box.height = 78;
    } else if (p.type == projectile_type::star) {
        p.shadow_y += 15;
    }
    
    p.int_x = static_cast<int>(p.x);
    p.int_y = static_cast<int>(p.y);

    p.last_torchwood_col = -1;

    p.target = -1;

    return p;
}

void projectile_factory::create(projectile_type type, zombie& z, plant* p) {
    assert(type == object::projectile_type::basketball);

    double x_end = p == nullptr ? z.x - 300 : p->x;
    double y_end = static_cast<double>(p == nullptr ? 0 : p->y) - z.y + 44;

    auto& o = alloc_and_init(
        type,
        z.row,
        static_cast<int>(z.x + 113),
        static_cast<int>(z.y - 44));

    o.motion_type = projectile_motion_type::parabola;
    o.dx = static_cast<float>(-std::max(40.0, o.x - x_end - 20) / 120);
    o.dy2 = 0;
    o.ddy = static_cast<float>(y_end / 120 - 7);
    o.dddy = 0.115f;
}

projectile& projectile_factory::create(
    projectile_type type,
    unsigned int row,
    int x,
    int y)
{
    assert(type != projectile_type::none);
    return alloc_and_init(type, row, x, y);
}

}