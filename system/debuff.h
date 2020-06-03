#pragma once
#include "object/scene.h"
#include "object/zombie.h"
#include "system/reanim.h"

namespace pvz_emulator::system {

class debuff {
private:
    object::scene& scene;
    system::reanim reanim;
    
public:
    void set_slowed(object::zombie& z, unsigned int countdown);

    void set_butter(object::zombie& z);

    void remove_freeze(object::zombie& z) {
        z.countdown.freeze = 0;
        reanim.update_fps(z);
    }

    void remove_slow(object::zombie& z) {
        z.countdown.slow = 0;
        reanim.update_fps(z);
    }

    void remove_by_fire_pea(object::zombie& z);

    debuff(object::scene& s) : scene(s), reanim(s) { }
};

}
