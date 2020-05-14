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

void zombie::set_reanim_frame(const char * name) {
    if (type == zombie_type::backup_dancer)  {
        if(name == "anim_walk") {
            reanim.begin_frame = 0;
            reanim.n_frames = 21;
        } else if (name == "anim_armraise") {
            reanim.begin_frame = 21;
            reanim.n_frames = 11;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 32;
            reanim.n_frames = 33;
        } else if (name == "anim_death") {
            reanim.begin_frame = 65;
            reanim.n_frames = 35;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::balloon)  {
        if(name == "anim_idle") {
            reanim.begin_frame = 0;
            reanim.n_frames = 13;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 84;
            reanim.n_frames = 41;
        } else if (name == "anim_pop") {
            reanim.begin_frame = 31;
            reanim.n_frames = 28;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 59;
            reanim.n_frames = 25;
        } else if (name == "anim_death") {
            reanim.begin_frame = 125;
            reanim.n_frames = 28;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::bungee)  {
        if(name == "anim_idle") {
            reanim.begin_frame = 0;
            reanim.n_frames = 20;
        } else if (name == "anim_grab") {
            reanim.begin_frame = 29;
            reanim.n_frames = 15;
        } else if (name == "anim_drop") {
            reanim.begin_frame = 20;
            reanim.n_frames = 9;
        } else if (name == "anim_raise") {
            reanim.begin_frame = 54;
            reanim.n_frames = 13;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::catapult)  {
        if(name == "anim_idle") {
            reanim.begin_frame = 57;
            reanim.n_frames = 13;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 0;
            reanim.n_frames = 25;
        } else if (name == "anim_bounce") {
            reanim.begin_frame = 70;
            reanim.n_frames = 15;
        } else if (name == "anim_shoot") {
            reanim.begin_frame = 25;
            reanim.n_frames = 32;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::dancing)  {
        if(name == "anim_walk") {
            reanim.begin_frame = 46;
            reanim.n_frames = 21;
        } else if (name == "anim_moonwalk") {
            reanim.begin_frame = 0;
            reanim.n_frames = 27;
        } else if (name == "anim_armraise") {
            reanim.begin_frame = 67;
            reanim.n_frames = 11;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 78;
            reanim.n_frames = 34;
        } else if (name == "anim_point") {
            reanim.begin_frame = 27;
            reanim.n_frames = 10;
        } else if (name == "anim_death") {
            reanim.begin_frame = 112;
            reanim.n_frames = 35;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::digger)  {
        if(name == "anim_idle") {
            reanim.begin_frame = 0;
            reanim.n_frames = 18;
        } else if (name == "anim_drill") {
            reanim.begin_frame = 146;
            reanim.n_frames = 19;
        } else if (name == "anim_dizzy") {
            reanim.begin_frame = 179;
            reanim.n_frames = 21;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 18;
            reanim.n_frames = 37;
        } else if (name == "anim_dig") {
            reanim.begin_frame = 128;
            reanim.n_frames = 18;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 55;
            reanim.n_frames = 33;
        } else if (name == "anim_death") {
            reanim.begin_frame = 88;
            reanim.n_frames = 40;
        } else if (name == "anim_landing") {
            reanim.begin_frame = 165;
            reanim.n_frames = 14;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::dolphin_rider)  {
        if(name == "anim_dolphinjump") {
            reanim.begin_frame = 143;
            reanim.n_frames = 23;
        } else if (name == "anim_ride") {
            reanim.begin_frame = 138;
            reanim.n_frames = 5;
        } else if (name == "anim_jumpinpool") {
            reanim.begin_frame = 93;
            reanim.n_frames = 45;
        } else if (name == "anim_idle") {
            reanim.begin_frame = 0;
            reanim.n_frames = 15;
        } else if (name == "anim_swim") {
            reanim.begin_frame = 208;
            reanim.n_frames = 42;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 54;
            reanim.n_frames = 39;
        } else if (name == "anim_walkdolphin") {
            reanim.begin_frame = 15;
            reanim.n_frames = 39;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 166;
            reanim.n_frames = 42;
        } else if (name == "anim_death") {
            reanim.begin_frame = 250;
            reanim.n_frames = 39;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::football)  {
        if(name == "anim_idle") {
            reanim.begin_frame = 0;
            reanim.n_frames = 21;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 21;
            reanim.n_frames = 30;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 51;
            reanim.n_frames = 36;
        } else if (name == "anim_death") {
            reanim.begin_frame = 87;
            reanim.n_frames = 18;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::gargantuar ||
        type == zombie_type::giga_gargantuar)
    {
        if(name == "anim_smash") {
            reanim.begin_frame = 71;
            reanim.n_frames = 33;
        } else if (name == "anim_idle") {
            reanim.begin_frame = 0;
            reanim.n_frames = 22;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 22;
            reanim.n_frames = 49;
        } else if (name == "anim_death") {
            reanim.begin_frame = 138;
            reanim.n_frames = 59;
        } else if (name == "anim_throw") {
            reanim.begin_frame = 104;
            reanim.n_frames = 34;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::imp)  {
        if(name == "anim_walk") {
            reanim.begin_frame = 0;
            reanim.n_frames = 33;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 33;
            reanim.n_frames = 27;
        } else if (name == "anim_death") {
            reanim.begin_frame = 60;
            reanim.n_frames = 22;
        } else if (name == "anim_land") {
            reanim.begin_frame = 98;
            reanim.n_frames = 6;
        } else if (name == "anim_thrown") {
            reanim.begin_frame = 82;
            reanim.n_frames = 16;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::jack_in_the_box)  {
        if(name == "anim_idle") {
            reanim.begin_frame = 13;
            reanim.n_frames = 17;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 30;
            reanim.n_frames = 19;
        } else if (name == "anim_pop") {
            reanim.begin_frame = 64;
            reanim.n_frames = 16;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 49;
            reanim.n_frames = 15;
        } else if (name == "anim_death") {
            reanim.begin_frame = 80;
            reanim.n_frames = 23;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::ladder)  {
        if(name == "anim_idle") {
            reanim.begin_frame = 0;
            reanim.n_frames = 25;
        } else if (name == "anim_ladderwalk") {
            reanim.begin_frame = 25;
            reanim.n_frames = 47;
        } else if (name == "anim_laddereat") {
            reanim.begin_frame = 72;
            reanim.n_frames = 24;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 132;
            reanim.n_frames = 47;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 179;
            reanim.n_frames = 24;
        } else if (name == "anim_death") {
            reanim.begin_frame = 96;
            reanim.n_frames = 36;
        } else if (name == "anim_placeladder") {
            reanim.begin_frame = 203;
            reanim.n_frames = 12;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::newspaper)  {
        if(name == "anim_idle") {
            reanim.begin_frame = 0;
            reanim.n_frames = 25;
        } else if (name == "anim_walk_nopaper") {
            reanim.begin_frame = 145;
            reanim.n_frames = 47;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 25;
            reanim.n_frames = 47;
        } else if (name == "anim_gasp") {
            reanim.begin_frame = 132;
            reanim.n_frames = 13;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 72;
            reanim.n_frames = 24;
        } else if (name == "anim_eat_nopaper") {
            reanim.begin_frame = 192;
            reanim.n_frames = 24;
        } else if (name == "anim_death") {
            reanim.begin_frame = 96;
            reanim.n_frames = 36;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::pogo)  {
        if(name == "anim_idle") {
            reanim.begin_frame = 0;
            reanim.n_frames = 29;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 29;
            reanim.n_frames = 47;
        } else if (name == "anim_pogo") {
            reanim.begin_frame = 155;
            reanim.n_frames = 11;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 76;
            reanim.n_frames = 40;
        } else if (name == "anim_death") {
            reanim.begin_frame = 116;
            reanim.n_frames = 39;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::pole_vaulting)  {
        if(name == "anim_idle") {
            reanim.begin_frame = 0;
            reanim.n_frames = 13;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 93;
            reanim.n_frames = 45;
        } else if (name == "anim_run") {
            reanim.begin_frame = 13;
            reanim.n_frames = 37;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 165;
            reanim.n_frames = 28;
        } else if (name == "anim_death") {
            reanim.begin_frame = 138;
            reanim.n_frames = 27;
        } else if (name == "anim_jump") {
            reanim.begin_frame = 50;
            reanim.n_frames = 43;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::snorkel)  {
        if(name == "anim_jumpinpool") {
            reanim.begin_frame = 59;
            reanim.n_frames = 19;
        } else if (name == "anim_idle") {
            reanim.begin_frame = 0;
            reanim.n_frames = 22;
        } else if (name == "anim_swim") {
            reanim.begin_frame = 78;
            reanim.n_frames = 7;
        } else if (name == "anim_uptoeat") {
            reanim.begin_frame = 85;
            reanim.n_frames = 12;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 22;
            reanim.n_frames = 37;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 97;
            reanim.n_frames = 25;
        } else if (name == "anim_death") {
            reanim.begin_frame = 122;
            reanim.n_frames = 35;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::yeti)  {
        if(name == "anim_idle") {
            reanim.begin_frame = 0;
            reanim.n_frames = 15;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 15;
            reanim.n_frames = 34;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 49;
            reanim.n_frames = 29;
        } else if (name == "anim_death") {
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
        if(name == "anim_idle") {
            reanim.begin_frame = 0;
            reanim.n_frames = 29;
        } else if (name == "anim_swim") {
            reanim.begin_frame = 250;
            reanim.n_frames = 42;
        } else if (name == "anim_walk2") {
            reanim.begin_frame = 91;
            reanim.n_frames = 47;
        } else if (name == "anim_walk") {
            reanim.begin_frame = 44;
            reanim.n_frames = 47;
        } else if (name == "anim_dance") {
            reanim.begin_frame = 454;
            reanim.n_frames = 50;
        } else if (name == "anim_eat") {
            reanim.begin_frame = 138;
            reanim.n_frames = 40;
        } else if (name == "anim_death") {
            reanim.begin_frame = 178;
            reanim.n_frames = 39;
        } else if (name == "anim_death2") {
            reanim.begin_frame = 217;
            reanim.n_frames = 33;
        } else if (name == "anim_superlongdeath") {
            reanim.begin_frame = 292;
            reanim.n_frames = 137;
        } else if (name == "anim_waterdeath") {
            reanim.begin_frame = 429;
            reanim.n_frames = 25;
        } else {
            assert(false);
        }
    } else if (type == zombie_type::zomboni)  {
        if(name == "anim_drive") {
            reanim.begin_frame = 0;
            reanim.n_frames = 13;
        } else if (name == "anim_wheelie1") {
            reanim.begin_frame = 13;
            reanim.n_frames = 16;
        } else if (name == "anim_wheelie2") {
            reanim.begin_frame = 29;
            reanim.n_frames = 33;
        } else {
            assert(false);
        }
    } else {
        assert(false);
    }
}

bool zombie::has_reanim(const char *name) const {
   if (name == "_ground") {
       return type != zombie_type::bungee &&
           type != zombie_type::zomboni &&
           type != zombie_type::catapult;
   }

   switch(type) {
    case zombie_type::backup_dancer:
        return name == "anim_walk" ||
            name == "anim_armraise" ||
            name == "anim_eat" ||
            name == "anim_death";

    case zombie_type::balloon:
        return name == "anim_idle" ||
            name == "anim_walk" ||
            name == "anim_pop" ||
            name == "anim_eat" ||
            name == "anim_death";

    case zombie_type::bungee:
        return name == "anim_idle" ||
            name == "anim_grab" ||
            name == "anim_drop" ||
            name == "anim_raise";

    case zombie_type::catapult:
        return name == "anim_idle" ||
            name == "anim_walk" ||
            name == "anim_bounce" ||
            name == "anim_shoot";

    case zombie_type::dancing:
        return name == "anim_walk" ||
            name == "anim_moonwalk" ||
            name == "anim_armraise" ||
            name == "anim_eat" ||
            name == "anim_point" ||
            name == "anim_death";

    case zombie_type::digger:
        return name == "anim_idle" ||
            name == "anim_drill" ||
            name == "anim_dizzy" ||
            name == "anim_walk" ||
            name == "anim_dig" ||
            name == "anim_eat" ||
            name == "anim_death" ||
            name == "anim_landing";

    case zombie_type::dolphin_rider:
        return name == "anim_dolphinjump" ||
            name == "anim_ride" ||
            name == "anim_jumpinpool" ||
            name == "anim_idle" ||
            name == "anim_swim" ||
            name == "anim_walk" ||
            name == "anim_walkdolphin" ||
            name == "anim_eat" ||
            name == "anim_death";

    case zombie_type::football:
        return name == "anim_idle" ||
            name == "anim_walk" ||
            name == "anim_eat" ||
            name == "anim_death";

    case zombie_type::gargantuar:
    case zombie_type::giga_gargantuar:
        return name == "anim_smash" ||
            name == "anim_idle" ||
            name == "anim_walk" ||
            name == "anim_death" ||
            name == "anim_throw";

    case zombie_type::imp:
        return name == "anim_walk" ||
            name == "anim_eat" ||
            name == "anim_death" ||
            name == "anim_land" ||
            name == "anim_thrown";

    case zombie_type::jack_in_the_box:
        return name == "anim_idle" ||
            name == "anim_walk" ||
            name == "anim_pop" ||
            name == "anim_eat" ||
            name == "anim_death";

    case zombie_type::ladder:
        return name == "anim_idle" ||
            name == "anim_ladderwalk" ||
            name == "anim_laddereat" ||
            name == "anim_walk" ||
            name == "anim_eat" ||
            name == "anim_death" ||
            name == "anim_placeladder";

    case zombie_type::newspaper:
        return name == "anim_idle" ||
            name == "anim_walk_nopaper" ||
            name == "anim_walk" ||
            name == "anim_gasp" ||
            name == "anim_eat" ||
            name == "anim_eat_nopaper" ||
            name == "anim_death";

    case zombie_type::pogo:
        return name == "anim_idle" ||
            name == "anim_walk" ||
            name == "anim_pogo" ||
            name == "anim_eat" ||
            name == "anim_death";

    case zombie_type::pole_vaulting:
        return name == "anim_idle" ||
            name == "anim_walk" ||
            name == "anim_run" ||
            name == "anim_eat" ||
            name == "anim_death" ||
            name == "anim_jump";

    case zombie_type::snorkel:
        return name == "anim_jumpinpool" ||
            name == "anim_idle" ||
            name == "anim_swim" ||
            name == "anim_uptoeat" ||
            name == "anim_walk" ||
            name == "anim_eat" ||
            name == "anim_death";

    case zombie_type::yeti:
        return name == "anim_idle" ||
            name == "anim_walk" ||
            name == "anim_eat" ||
            name == "anim_death";

    case zombie_type::zombie:
    case zombie_type::conehead:
    case zombie_type::buckethead:
    case zombie_type::screendoor:
    case zombie_type::flag:
    case zombie_type::ducky_tube:
        return name == "anim_idle" ||
            name == "anim_swim" ||
            name == "anim_walk2" ||
            name == "anim_walk" ||
            name == "anim_dance" ||
            name == "anim_eat" ||
            name == "anim_death" ||
            name == "anim_death2" ||
            name == "anim_superlongdeath" ||
            name == "anim_waterdeath";

    case zombie_type::zomboni:
        return name == "anim_drive" ||
            name == "anim_wheelie1" ||
            name == "anim_wheelie2";

    default:
        return false;
    }
}

}
