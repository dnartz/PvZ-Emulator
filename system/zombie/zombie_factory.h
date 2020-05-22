#pragma once
#include "object/scene.h"
#include "object/zombie.h"
#include "system/rng.h"
#include "system/reanim.h"
#include "system/zombie/zombie.h"

namespace pvz_emulator::system {

class zombie_factory {
private:
    object::scene& scene;
    object::scene::spawn_data& data;
    reanim reanim;
    rng rng;

    zombie_subsystems subsystems;
    
    bool can_spawn_at_row(object::zombie_type type, unsigned int row);
    unsigned int get_spawn_row(object::zombie_type type);

    void create_pool_or_night_lurking(object::zombie_type type, unsigned int row, unsigned int col);
    void create_roof_lurking(object::zombie_type type, unsigned int row, unsigned int col);

public:
    zombie_factory(object::scene& s) :
        scene(s),
        data(s.spawn),
        reanim(s),
        rng(s),
        subsystems(s) {}

    object::zombie& create(object::zombie_type type);
    void create_lurking(object::zombie_type type, unsigned int row, unsigned int col);

    void destroy(object::zombie& z);
};

}
