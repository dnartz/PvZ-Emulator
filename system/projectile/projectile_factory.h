#pragma once
#include <cassert>

#include "object/scene.h"
#include "object/zombie.h"
#include "object/projectile.h"
#include "system/rng.h"
#include "system/util.h"

namespace pvz_emulator::system {

class projectile_factory {
private:
    object::scene& scene;
    system::rng rng;

    object::projectile&
    alloc_and_init(object::projectile_type type, int row, int x, int y);

public:
    projectile_factory(object::scene& s): scene(s), rng(s) {}

    void destroy(object::projectile& proj) {
        assert(!proj.is_disappeared);
        proj.is_disappeared = true;
    }

    void create(object::projectile_type type, object::zombie& z, object::plant* p);

    object::projectile&
    create(object::projectile_type type, unsigned int row, int x, int y);
};

}
