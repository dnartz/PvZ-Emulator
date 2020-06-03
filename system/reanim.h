#pragma once
#include "object/scene.h"
#include "object/zombie.h"
#include "system/rng.h"
#include "system/util.h"

namespace pvz_emulator::system {

class reanim {
private:
    object::scene& scene;
    system::rng rng;

public:
    void set(
        object::zombie& z,
        object::zombie_reanim_name name,
        object::reanim_type type,
        float fps);

    void set_fps(object::zombie& z, float fps) {
        z.reanim.fps = is_slowed(scene, z) ? 0.5f * fps : fps;
    }

    void update_dx(object::zombie& z, bool do_update_fps = true);

    void update_fps(object::zombie& z);
    void update_status(object::zombie& z);

    void update_progress(object::reanim& r);

    reanim(object::scene& s) : scene(s), rng(s) {}
};

}
