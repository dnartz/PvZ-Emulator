#include <cassert>
#include "object/zombie.h"

namespace pvz_emulator::object {

void zombie::init_reanim() {
    reanim.prev_progress = -1;
    switch (type) {
    case zombie_type::backup_dancer:
        reanim.n_frames = 100;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::balloon:
        reanim.n_frames = 155;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::bungee:
        reanim.n_frames = 67;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::catapult:
        reanim.n_frames = 85;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::dancing:
        reanim.n_frames = 147;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::digger:
        reanim.n_frames = 200;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::dolphin_rider:
        reanim.n_frames = 289;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::football:
        reanim.n_frames = 105;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::giga_gargantuar:
    case zombie_type::gargantuar:
        reanim.n_frames = 197;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::imp:
        reanim.n_frames = 104;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::jack_in_the_box:
        reanim.n_frames = 103;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::ladder:
        reanim.n_frames = 215;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::newspaper:
        reanim.n_frames = 216;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::pogo:
        reanim.n_frames = 166;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::pole_vaulting:
        reanim.n_frames = 193;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::snorkel:
        reanim.n_frames = 249;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::yeti:
        reanim.n_frames = 105;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::conehead:
    case zombie_type::buckethead:
    case zombie_type::screendoor:
    case zombie_type::flag:
    case zombie_type::ducky_tube:
    case zombie_type::zombie:
        reanim.n_frames = 504;
        reanim.fps = 12.000000f;
        break;
    case zombie_type::zomboni:
        reanim.n_frames = 62;
        reanim.fps = 12.000000f;
        break;
    default:
        assert(false);
    }
}

void zombie::set_reanim_frame(zombie_reanim_name name) {
    if (type == zombie_type::backup_dancer)  {
        if(name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 0;
            reanim.n_frames = 21;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 32;
            reanim.n_frames = 33;
        } else if (name == zombie_reanim_name::anim_armraise) {
            reanim.begin_frame = 21;
            reanim.n_frames = 11;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 65;
            reanim.n_frames = 35;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::balloon)  {
        if(name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 13;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 84;
            reanim.n_frames = 41;
        } else if (name == zombie_reanim_name::anim_pop) {
            reanim.begin_frame = 31;
            reanim.n_frames = 28;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 59;
            reanim.n_frames = 25;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 125;
            reanim.n_frames = 28;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::bungee)  {
        if(name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 20;
        } else if (name == zombie_reanim_name::anim_grab) {
            reanim.begin_frame = 29;
            reanim.n_frames = 15;
        } else if (name == zombie_reanim_name::anim_drop) {
            reanim.begin_frame = 20;
            reanim.n_frames = 9;
        } else if (name == zombie_reanim_name::anim_raise) {
            reanim.begin_frame = 54;
            reanim.n_frames = 13;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::catapult)  {
        if(name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 57;
            reanim.n_frames = 13;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 0;
            reanim.n_frames = 25;
        } else if (name == zombie_reanim_name::anim_bounce) {
            reanim.begin_frame = 70;
            reanim.n_frames = 15;
        } else if (name == zombie_reanim_name::anim_shoot) {
            reanim.begin_frame = 25;
            reanim.n_frames = 32;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::dancing)  {
        if(name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 46;
            reanim.n_frames = 21;
        } else if (name == zombie_reanim_name::anim_moonwalk) {
            reanim.begin_frame = 0;
            reanim.n_frames = 27;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 78;
            reanim.n_frames = 34;
        } else if (name == zombie_reanim_name::anim_point) {
            reanim.begin_frame = 27;
            reanim.n_frames = 10;
        } else if (name == zombie_reanim_name::anim_armraise) {
            reanim.begin_frame = 67;
            reanim.n_frames = 11;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 112;
            reanim.n_frames = 35;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::digger)  {
        if(name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 18;
        } else if (name == zombie_reanim_name::anim_drill) {
            reanim.begin_frame = 146;
            reanim.n_frames = 19;
        } else if (name == zombie_reanim_name::anim_dizzy) {
            reanim.begin_frame = 179;
            reanim.n_frames = 21;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 18;
            reanim.n_frames = 37;
        } else if (name == zombie_reanim_name::anim_dig) {
            reanim.begin_frame = 128;
            reanim.n_frames = 18;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 55;
            reanim.n_frames = 33;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 88;
            reanim.n_frames = 40;
        } else if (name == zombie_reanim_name::anim_landing) {
            reanim.begin_frame = 165;
            reanim.n_frames = 14;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::dolphin_rider)  {
        if(name == zombie_reanim_name::anim_dolphinjump) {
            reanim.begin_frame = 143;
            reanim.n_frames = 23;
        } else if (name == zombie_reanim_name::anim_ride) {
            reanim.begin_frame = 138;
            reanim.n_frames = 5;
        } else if (name == zombie_reanim_name::anim_jumpinpool) {
            reanim.begin_frame = 93;
            reanim.n_frames = 45;
        } else if (name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 15;
        } else if (name == zombie_reanim_name::anim_swim) {
            reanim.begin_frame = 208;
            reanim.n_frames = 42;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 54;
            reanim.n_frames = 39;
        } else if (name == zombie_reanim_name::anim_walkdolphin) {
            reanim.begin_frame = 15;
            reanim.n_frames = 39;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 166;
            reanim.n_frames = 42;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 250;
            reanim.n_frames = 39;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::football)  {
        if(name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 21;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 21;
            reanim.n_frames = 30;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 51;
            reanim.n_frames = 36;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 87;
            reanim.n_frames = 18;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::gargantuar ||
        type == zombie_type::giga_gargantuar)
    {
        if(name == zombie_reanim_name::anim_smash) {
            reanim.begin_frame = 71;
            reanim.n_frames = 33;
        } else if (name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 22;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 22;
            reanim.n_frames = 49;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 138;
            reanim.n_frames = 59;
        } else if (name == zombie_reanim_name::anim_throw) {
            reanim.begin_frame = 104;
            reanim.n_frames = 34;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::imp)  {
        if(name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 0;
            reanim.n_frames = 33;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 33;
            reanim.n_frames = 27;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 60;
            reanim.n_frames = 22;
        } else if (name == zombie_reanim_name::anim_land) {
            reanim.begin_frame = 98;
            reanim.n_frames = 6;
        } else if (name == zombie_reanim_name::anim_thrown) {
            reanim.begin_frame = 82;
            reanim.n_frames = 16;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::jack_in_the_box)  {
        if(name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 13;
            reanim.n_frames = 17;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 30;
            reanim.n_frames = 19;
        } else if (name == zombie_reanim_name::anim_pop) {
            reanim.begin_frame = 64;
            reanim.n_frames = 16;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 49;
            reanim.n_frames = 15;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 80;
            reanim.n_frames = 23;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::ladder)  {
        if(name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 25;
        } else if (name == zombie_reanim_name::anim_ladderwalk) {
            reanim.begin_frame = 25;
            reanim.n_frames = 47;
        } else if (name == zombie_reanim_name::anim_laddereat) {
            reanim.begin_frame = 72;
            reanim.n_frames = 24;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 132;
            reanim.n_frames = 47;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 179;
            reanim.n_frames = 24;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 96;
            reanim.n_frames = 36;
        } else if (name == zombie_reanim_name::anim_placeladder) {
            reanim.begin_frame = 203;
            reanim.n_frames = 12;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::newspaper)  {
        if(name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 25;
        } else if (name == zombie_reanim_name::anim_walk_nopaper) {
            reanim.begin_frame = 145;
            reanim.n_frames = 47;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 25;
            reanim.n_frames = 47;
        } else if (name == zombie_reanim_name::anim_gasp) {
            reanim.begin_frame = 132;
            reanim.n_frames = 13;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 72;
            reanim.n_frames = 24;
        } else if (name == zombie_reanim_name::anim_eat_nopaper) {
            reanim.begin_frame = 192;
            reanim.n_frames = 24;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 96;
            reanim.n_frames = 36;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::pogo)  {
        if(name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 29;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 29;
            reanim.n_frames = 47;
        } else if (name == zombie_reanim_name::anim_pogo) {
            reanim.begin_frame = 155;
            reanim.n_frames = 11;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 76;
            reanim.n_frames = 40;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 116;
            reanim.n_frames = 39;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::pole_vaulting)  {
        if(name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 13;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 93;
            reanim.n_frames = 45;
        } else if (name == zombie_reanim_name::anim_run) {
            reanim.begin_frame = 13;
            reanim.n_frames = 37;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 165;
            reanim.n_frames = 28;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 138;
            reanim.n_frames = 27;
        } else if (name == zombie_reanim_name::anim_jump) {
            reanim.begin_frame = 50;
            reanim.n_frames = 43;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::snorkel)  {
        if(name == zombie_reanim_name::anim_jumpinpool) {
            reanim.begin_frame = 59;
            reanim.n_frames = 19;
        } else if (name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 22;
        } else if (name == zombie_reanim_name::anim_swim) {
            reanim.begin_frame = 78;
            reanim.n_frames = 7;
        } else if (name == zombie_reanim_name::anim_uptoeat) {
            reanim.begin_frame = 85;
            reanim.n_frames = 12;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 22;
            reanim.n_frames = 37;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 97;
            reanim.n_frames = 25;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 122;
            reanim.n_frames = 35;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::yeti)  {
        if(name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 15;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 15;
            reanim.n_frames = 34;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 49;
            reanim.n_frames = 29;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 78;
            reanim.n_frames = 27;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::zombie ||
        type == zombie_type::conehead ||
        type == zombie_type::buckethead ||
        type == zombie_type::screendoor ||
        type == zombie_type::flag ||
        type == zombie_type::ducky_tube)
    {
        if(name == zombie_reanim_name::anim_idle) {
            reanim.begin_frame = 0;
            reanim.n_frames = 29;
        } else if (name == zombie_reanim_name::anim_swim) {
            reanim.begin_frame = 250;
            reanim.n_frames = 42;
        } else if (name == zombie_reanim_name::anim_walk2) {
            reanim.begin_frame = 91;
            reanim.n_frames = 47;
        } else if (name == zombie_reanim_name::anim_walk) {
            reanim.begin_frame = 44;
            reanim.n_frames = 47;
        } else if (name == zombie_reanim_name::anim_dance) {
            reanim.begin_frame = 454;
            reanim.n_frames = 50;
        } else if (name == zombie_reanim_name::anim_eat) {
            reanim.begin_frame = 138;
            reanim.n_frames = 40;
        } else if (name == zombie_reanim_name::anim_death) {
            reanim.begin_frame = 178;
            reanim.n_frames = 39;
        } else if (name == zombie_reanim_name::anim_death2) {
            reanim.begin_frame = 217;
            reanim.n_frames = 33;
        } else if (name == zombie_reanim_name::anim_superlongdeath) {
            reanim.begin_frame = 292;
            reanim.n_frames = 137;
        } else if (name == zombie_reanim_name::anim_waterdeath) {
            reanim.begin_frame = 429;
            reanim.n_frames = 25;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::zomboni)  {
        if(name == zombie_reanim_name::anim_drive) {
            reanim.begin_frame = 0;
            reanim.n_frames = 13;
        } else if (name == zombie_reanim_name::anim_wheelie1) {
            reanim.begin_frame = 13;
            reanim.n_frames = 16;
        } else if (name == zombie_reanim_name::anim_wheelie2) {
            reanim.begin_frame = 29;
            reanim.n_frames = 33;
        } else {
            assert(false);
        }
    } else {
        assert(false);
    }
}

bool zombie::has_reanim(zombie_reanim_name name) const {
   if (name == zombie_reanim_name::_ground) {
       return type != zombie_type::bungee &&
           type != zombie_type::zomboni &&
           type != zombie_type::catapult;
   }

   switch(type) {
    case zombie_type::backup_dancer:
        return name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_armraise ||
            name == zombie_reanim_name::anim_death;

    case zombie_type::balloon:
        return name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_pop ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_death;

    case zombie_type::bungee:
        return name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_grab ||
            name == zombie_reanim_name::anim_drop ||
            name == zombie_reanim_name::anim_raise;

    case zombie_type::catapult:
        return name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_bounce ||
            name == zombie_reanim_name::anim_shoot;

    case zombie_type::dancing:
        return name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_moonwalk ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_point ||
            name == zombie_reanim_name::anim_armraise ||
            name == zombie_reanim_name::anim_death;

    case zombie_type::digger:
        return name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_drill ||
            name == zombie_reanim_name::anim_dizzy ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_dig ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_death ||
            name == zombie_reanim_name::anim_landing;

    case zombie_type::dolphin_rider:
        return name == zombie_reanim_name::anim_dolphinjump ||
            name == zombie_reanim_name::anim_ride ||
            name == zombie_reanim_name::anim_jumpinpool ||
            name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_swim ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_walkdolphin ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_death;

    case zombie_type::football:
        return name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_death;

    case zombie_type::gargantuar:
    case zombie_type::giga_gargantuar:
        return name == zombie_reanim_name::anim_smash ||
            name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_death ||
            name == zombie_reanim_name::anim_throw;

    case zombie_type::imp:
        return name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_death ||
            name == zombie_reanim_name::anim_land ||
            name == zombie_reanim_name::anim_thrown;

    case zombie_type::jack_in_the_box:
        return name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_pop ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_death;

    case zombie_type::ladder:
        return name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_ladderwalk ||
            name == zombie_reanim_name::anim_laddereat ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_death ||
            name == zombie_reanim_name::anim_placeladder;

    case zombie_type::newspaper:
        return name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_walk_nopaper ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_gasp ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_eat_nopaper ||
            name == zombie_reanim_name::anim_death;

    case zombie_type::pogo:
        return name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_pogo ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_death;

    case zombie_type::pole_vaulting:
        return name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_run ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_death ||
            name == zombie_reanim_name::anim_jump;

    case zombie_type::snorkel:
        return name == zombie_reanim_name::anim_jumpinpool ||
            name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_swim ||
            name == zombie_reanim_name::anim_uptoeat ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_death;

    case zombie_type::yeti:
        return name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_death;

    case zombie_type::zombie:
    case zombie_type::conehead:
    case zombie_type::buckethead:
    case zombie_type::screendoor:
    case zombie_type::flag:
    case zombie_type::ducky_tube:
        return name == zombie_reanim_name::anim_idle ||
            name == zombie_reanim_name::anim_swim ||
            name == zombie_reanim_name::anim_walk2 ||
            name == zombie_reanim_name::anim_walk ||
            name == zombie_reanim_name::anim_dance ||
            name == zombie_reanim_name::anim_eat ||
            name == zombie_reanim_name::anim_death ||
            name == zombie_reanim_name::anim_death2 ||
            name == zombie_reanim_name::anim_superlongdeath ||
            name == zombie_reanim_name::anim_waterdeath;

    case zombie_type::zomboni:
        return name == zombie_reanim_name::anim_drive ||
            name == zombie_reanim_name::anim_wheelie1 ||
            name == zombie_reanim_name::anim_wheelie2;

    default:
        return false;
    }
}

}
