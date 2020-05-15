#include "reanim.h"

namespace pvz_emulator::system {

using namespace pvz_emulator::object;

void reanim::set(zombie &z, zombie_reanim_name name, reanim_type type, float fps) {
    if (fps != 0) {
        z.reanim.fps = fps;
    }

    z.reanim.type = type;
    z.reanim.n_repeated = 0;

    z.reanim.progress = static_cast<float>(z.reanim.fps >= 0 ? 0 : 0.99999988);
    z.reanim.prev_progress = -1;

    z.set_reanim_frame(name);

    if (z.reanim.fps == 0) {
        update_fps(z);
    } else {
        z.reanim.prev_fps = z.reanim.fps;
        update_fps(z);
    }
}

void reanim::update_dx(object::zombie& z, bool do_update_fps) {
    if (z.status == zombie_status::snorkel_swim) {
        z.dx = 0.30000001f;
    } else if (z.status == zombie_status::digger_walk_right) {
        z.dx = 0.12f;
    } else if (z.status == zombie_status::yeti_escape) {
        z.dx = 0.40000001f;
    } else if (z.type == zombie_type::yeti) {
        z.dx = 0.40000001f;
    } else if (z.type == zombie_type::dancing ||
        z.type == zombie_type::backup_dancer ||
        z.type == zombie_type::pogo ||
        z.type == zombie_type::flag)
    {
        z.dx = 0.44999999f;
    } else if (z.status == zombie_status::digger_dig ||
        z.status == zombie_status::pole_valuting_running ||
        z.type == zombie_type::football ||
        z.type == zombie_type::snorkel ||
        z.type == zombie_type::jack_in_the_box)
    {
        z.dx = rng.randfloat(0.66000003, 0.68000001);
    } else if (z.status == zombie_status::ladder_walking) {
        z.dx = rng.randfloat(0.79000002, 0.81);
    } else if (z.status == zombie_status::newspaper_running ||
        z.status == zombie_status::dophin_walk_with_dophin ||
        z.status == zombie_status::dophin_walk_without_dophin)
    {
        z.dx = rng.randfloat(0.88999999, 0.91000003);
    } else {
        z.dx = rng.randfloat(0.23, 0.37);

        if (z.dx >= 0.3) {
            z.garlic_tick.a = 15;
        } else {
            z.garlic_tick.a = 12;
        }
    }

    if (do_update_fps) {
        update_fps(z);
    }
}

void reanim::update_fps(zombie& z) {
    if (z.countdown.freeze > 0 ||
        z.countdown.butter > 0 ||
        z.has_eaten_garlic && z.time_since_ate_garlic < 170)
    {
        set_fps(z, 0);
        return;
    }

    if (z.status == zombie_status::snorkel_up_to_eat ||
        z.status == zombie_status::snorkel_finied_eat ||
        z.has_death_status() || z.is_dead)
    {
        set_fps(z, z.reanim.prev_fps);
        return;
    }

    if (z.is_eating) {
        if (z.type == zombie_type::pole_vaulting ||
            z.type == zombie_type::balloon ||
            z.type == zombie_type::imp ||
            z.type == zombie_type::digger ||
            z.type == zombie_type::jack_in_the_box ||
            z.type == zombie_type::snorkel ||
            z.type == zombie_type::yeti)
        {
            set_fps(z, 20);
        } else {
            set_fps(z, 36);
        }

        return;
    }

    if (is_not_movable(scene, z) ||
        z.type == zombie_type::catapult ||
        z.status == zombie_status::dophin_ride ||
        z.status == zombie_status::snorkel_swim)
    {
        set_fps(z, z.reanim.prev_fps);
        return;
    }

    if (z._ground) {
        float d = z._ground[z.reanim.begin_frame - 1 + z.reanim.n_frames] -
            z._ground[z.reanim.begin_frame];

        if (d >= 0.000001) {
            float fps = (z.reanim.n_frames / d) * z.dx * 47;
            set_fps(z, fps);
        }
    }
}

void reanim::update_status(zombie& z) {
    update_dx(z);

    if (z.status == zombie_status::ladder_walking) {
        set(z, zombie_reanim_name::anim_ladderwalk, reanim_type::repeat, 0);
    } else if (z.status == zombie_status::newspaper_running) {
        set(z, zombie_reanim_name::anim_walk_nopaper, reanim_type::repeat, 0);
    } else if (z.is_in_water &&
        z.action != zombie_action::entering_pool &&
        z.action != zombie_action::leaving_pool &&
        z.has_reanim(zombie_reanim_name::anim_swim))
    {
        set(z, zombie_reanim_name::anim_swim, reanim_type::repeat, 0);
    } else if (!scene.is_zombie_dance ||
        z.type != zombie_type::zombie &&
        z.type != zombie_type::conehead &&
        z.type != zombie_type::buckethead)
    {
        if (z.type != zombie_type::flag && rng.randint(2) ||
            !z.has_reanim(zombie_reanim_name::anim_walk2))
        {
            if (z.has_reanim(zombie_reanim_name::anim_walk)) {
                set(z, zombie_reanim_name::anim_walk, reanim_type::repeat, 0);
            }
        } else {
            set(z, zombie_reanim_name::anim_walk2, reanim_type::repeat, 0);
        }
    } else {
        set(z, zombie_reanim_name::anim_dance, reanim_type::repeat, 0);
    }
}

void reanim::update_progress(object::reanim& r) {
    if (r.n_frames == 0) {
        return;
    }

    r.prev_progress = r.progress;
    r.progress += static_cast<float>(r.fps * 0.00999999977648258 / r.n_frames);

    if (r.fps <= 0) {
        if (r.type == reanim_type::once) {
            if (r.progress < 0) {
                r.progress = 0;
                r.n_repeated = 1;
            }
        } else {
            for (; r.progress < 0; r.n_repeated++, r.progress++);
        }
    } else if (r.type == reanim_type::once) {
        if (r.progress >= 1) {
            r.n_repeated = 1;
            r.progress = 1;
        }
    } else {
        for (; r.progress >= 1; r.n_repeated++, r.progress--);
    }
}

}
