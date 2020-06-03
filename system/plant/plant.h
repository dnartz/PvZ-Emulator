#pragma once
#include <utility>
#include "object/scene.h"
#include "system/rng.h"

#define TRIVAL_INIT(base, name) \
    void init(object::plant& p, unsigned int row, unsigned int col) {\
        base ## _base::init(p, object::plant_type::name, row, col);\
    }\

namespace pvz_emulator::system {

class plant_base {
protected:
    object::scene& scene;
    system::rng rng;

    void init(
        object::plant& p,
        object::plant_type type,
        int row,
        int col,
        object::plant_type imitater_target = object::plant_type::none);

public:
    object::zombie*
    find_target(object::plant& p, unsigned int row, bool is_alt_attack);

    plant_base(object::scene& s) : scene(s), rng(s) {}
    void set_launch_countdown(object::plant& p, bool is_alt_attack);
};

class mushroom_base : public virtual plant_base {
protected:
    void init(
        object::plant& p,
        object::plant_type type,
        unsigned int row,
        unsigned int col);
public:
    mushroom_base(object::scene& s) : plant_base(s) {}
};

class sun_plant_base : public virtual plant_base {
protected:
    sun_plant_base(object::scene& s) : plant_base(s) {}

public:
    void produce_sun(object::plant& p);
};

class puff_mushroom_base : public mushroom_base {
protected:
    void init(
        object::plant& p,
        object::plant_type type,
        unsigned int row,
        unsigned int col)
    {
        mushroom_base::init(p, type, row, col);

        p.x += rng.randint(10) - 5;
        p.y += rng.randint(6) - 3;
    }
public:
    puff_mushroom_base(object::scene& s) : plant_base(s), mushroom_base(s) {}
};

class pea_base : public plant_base {
protected:
    void init(
        object::plant& p,
        object::plant_type type,
        unsigned int row,
        unsigned int col);

public:
    pea_base(object::scene& s) : plant_base(s) {}
};

class plant_pea_shooter : public pea_base {
public:
    static const std::pair<float, float> PEA_OFFSETS[104];
    TRIVAL_INIT(pea, pea_shooter)
    plant_pea_shooter(object::scene &s) : pea_base(s) {}
};

class plant_sunflower : public sun_plant_base {
public:
    TRIVAL_INIT(plant, sunflower)
    plant_sunflower(object::scene& s) : plant_base(s), sun_plant_base(s) {}
};

class plant_cherry_bomb : public plant_base {
public:
    void init(object::plant& p, unsigned int row, unsigned int col) {
        plant_base::init(p, object::plant_type::cherry_bomb, row, col);

        p.countdown.effect = 100;
        p.set_reanim_frame(object::plant_reanim_name::anim_explode);
        p.reanim.type = object::reanim_type::once;
    }

    plant_cherry_bomb(object::scene& s) : plant_base(s) {}
};

class plant_wallnut : public plant_base {
public:
    TRIVAL_INIT(plant, wallnut)
    plant_wallnut(object::scene& s): plant_base(s) {}
};

class plant_potato_mine : public plant_base {
public:
    void update(object::plant& p);
    void init(object::plant& p, unsigned int row, unsigned int col);
    plant_potato_mine(object::scene& s) : plant_base(s) {}
};

class plant_snow_pea : public pea_base {
public:
    static const std::pair<float, float> PEA_OFFSETS[104];
    TRIVAL_INIT(pea, snow_pea)
    plant_snow_pea(object::scene& s) : pea_base(s) {}
};

class plant_chomper: public plant_base {
public:
    void update(object::plant& p);
    void init(object::plant& p, unsigned int row, unsigned int col);
    plant_chomper(object::scene& s) : plant_base(s) {}
};

class plant_repeater: public pea_base {
public:
    static const std::pair<float, float> PEA_OFFSETS[104];
    TRIVAL_INIT(pea, repeater)
    plant_repeater(object::scene& s): pea_base(s) {}
};

class plant_puffshroom: public puff_mushroom_base {
public:
    TRIVAL_INIT(puff_mushroom, puffshroom)
    plant_puffshroom(object::scene& s): plant_base(s), puff_mushroom_base(s) {}
};

class plant_sunshroom: public mushroom_base, sun_plant_base {
public:
    void update(object::plant& p);
    void init(object::plant& p, unsigned int row, unsigned int col);
    plant_sunshroom(object::scene& s):
        plant_base(s),
        mushroom_base(s),
        sun_plant_base(s) {}
};

class plant_fumeshroom: public mushroom_base {
public:
    TRIVAL_INIT(mushroom, fumeshroom)
    plant_fumeshroom(object::scene& s): plant_base(s), mushroom_base(s) {}
};

class plant_grave_buster: public plant_base {
public:
    void update(object::plant& p);
    void init(object::plant& p, unsigned int row, unsigned int col);
    plant_grave_buster(object::scene& s): plant_base(s) {}
};

class plant_hypnoshroom: public mushroom_base {
public:
    TRIVAL_INIT(mushroom, hypnoshroom)
    plant_hypnoshroom(object::scene& s): plant_base(s), mushroom_base(s) {}
};

class plant_scaredyshroom: public mushroom_base {
public:
    void update(object::plant& p);

    void init(object::plant& p, unsigned int row, unsigned int col) {
        mushroom_base::init(p, object::plant_type::scaredyshroom, row, col);
        p.status = object::plant_status::wait;
    }

    plant_scaredyshroom(object::scene& s): plant_base(s), mushroom_base(s) {}
};

class plant_iceshroom: public mushroom_base {
public:
    void update(object::plant& p);
    TRIVAL_INIT(mushroom, iceshroom)
    plant_iceshroom(object::scene& s): plant_base(s), mushroom_base(s) {}
};

class plant_doomshroom: public mushroom_base {
public:
    void update(object::plant& p);
    TRIVAL_INIT(mushroom, doomshroom)
    plant_doomshroom(object::scene& s): plant_base(s), mushroom_base(s) {}
};

class plant_lily_pad: public plant_base {
public:
    void init(object::plant& p, unsigned int row, unsigned int col) {
        plant_base::init(p, object::plant_type::lily_pad, row, col);
        p.status = object::plant_status::lily_pad_placed;
        p.countdown.status = 100;
    }

    plant_lily_pad(object::scene& s): plant_base(s) {}
};

class plant_squash: public plant_base {
private:
    void kill(object::plant& p);
    object::zombie* find_target(object::plant& p);

    int get_jump_up_pos(object::plant& p, int a, int b);

public:
    void update(object::plant& p);
    TRIVAL_INIT(plant, squash)
    plant_squash(object::scene& s): plant_base(s) {}
};

class plant_threepeater: public pea_base {
public:
    TRIVAL_INIT(pea, threepeater)
    void set_launch_countdown(object::plant& p);
    plant_threepeater(object::scene& s): pea_base(s) {}
};

class plant_tangle_kelp: public plant_base {
public:
    void update(object::plant& p);
    TRIVAL_INIT(plant, tangle_kelp)
    plant_tangle_kelp(object::scene& s): plant_base(s) {}
};

class plant_jalapeno: public plant_base {
public:
    void init(object::plant& p, unsigned int row, unsigned int col) {
        plant_base::init(p, object::plant_type::jalapeno, row, col);

        p.countdown.effect = 100;
        p.set_reanim_frame(object::plant_reanim_name::anim_explode);
        p.reanim.type = object::reanim_type::once;
    }

    plant_jalapeno(object::scene& s): plant_base(s) {}
};

class plant_spikeweed: public plant_base {
private:
    void attack(object::plant& p);
    void set_idle_reanim(object::plant& p);
public:
    void update(object::plant& p);
    TRIVAL_INIT(plant, spikeweed)
    plant_spikeweed(object::scene& s): plant_base(s) {}
};

class plant_torchwood: public plant_base {
public:
    void update(object::plant& p);
    TRIVAL_INIT(plant, torchwood)
    plant_torchwood(object::scene& s): plant_base(s) {}
};

class plant_tallnut: public plant_base {
public:
    void init(object::plant& p, unsigned int row, unsigned int col);
    plant_tallnut(object::scene& s): plant_base(s) {}
};

class plant_seashroom: public puff_mushroom_base {
public:
    TRIVAL_INIT(puff_mushroom, puffshroom)
    plant_seashroom(object::scene& s): plant_base(s), puff_mushroom_base(s) {}
};

class plant_plantern: public plant_base {
public:
    void init(object::plant& p, unsigned int row, unsigned int col) {
        plant_base::init(p, object::plant_type::plantern, row, col);
        p.countdown.status = 50;
    }

    plant_plantern(object::scene& s): plant_base(s) {}
};

class plant_cactus: public plant_base {
public:
    void update(object::plant& p);
    void init(object::plant& p, unsigned int row, unsigned int col);
    plant_cactus(object::scene& s): plant_base(s) {}
};

class plant_blover: public plant_base {
public:
    void update(object::plant& p);
    void init(object::plant& p, unsigned int row, unsigned int col);
    plant_blover(object::scene& s): plant_base(s) {}
};

class plant_split_pea: public pea_base {
public:
    static const std::pair<float, float> PEA_OFFSETS[132];
    TRIVAL_INIT(pea, split_pea)
    plant_split_pea(object::scene& s): pea_base(s) {}
};

class plant_starfruit: public plant_base {
private:
    bool has_target(object::plant& p);

public:
    void attack(object::plant& p);
    TRIVAL_INIT(plant, starfruit)
    void set_launch_countdown(object::plant& p);
    plant_starfruit(object::scene& s): plant_base(s) {}
};

class plant_pumpkin: public plant_base {
public:
    void init(object::plant& p, unsigned int row, unsigned int col);
    plant_pumpkin(object::scene& s): plant_base(s) {}
};

class plant_magnetshroom: public mushroom_base {
private:
    bool attack_zombie(object::plant& p);
    void attack_ladder(object::plant& p);
public:
    void update(object::plant& p);
    TRIVAL_INIT(mushroom, magnetshroom)
    plant_magnetshroom(object::scene& s): plant_base(s), mushroom_base(s) {}
};

class plant_cabbagepult: public plant_base {
public:
    TRIVAL_INIT(plant, cabbagepult)
    plant_cabbagepult(object::scene& s): plant_base(s) {}
};

class plant_flower_pot: public plant_base {
public:
    void init(object::plant& p, unsigned int row, unsigned int col) {
        plant_base::init(p, object::plant_type::flower_pot, row, col);
        p.status = object::plant_status::flower_pot_placed;
        p.countdown.status = 100;
    }

    plant_flower_pot(object::scene& s): plant_base(s) {}
};

class plant_kernelpult: public plant_base {
public:
    TRIVAL_INIT(plant, kernelpult)
    plant_kernelpult(object::scene& s): plant_base(s) {}
};

class plant_coffee_bean: public plant_base {
public:
    void update(object::plant& p);
    void init(object::plant& p, unsigned int row, unsigned int col);
    plant_coffee_bean(object::scene& s): plant_base(s) {}
};

class plant_garlic: public plant_base {
public:
    TRIVAL_INIT(plant, garlic)
    plant_garlic(object::scene& s): plant_base(s) {}
};

class plant_umbrella_leaf: public plant_base {
public:
    void update(object::plant& p);
    TRIVAL_INIT(plant, umbrella_leaf)
    plant_umbrella_leaf(object::scene& s): plant_base(s) {}
};

class plant_marigold: public plant_base {
public:
    void init(object::plant& p, unsigned int row, unsigned int col) {
        plant_base::init(p, object::plant_type::marigold, row, col);
        p.reanim.fps = rng.randfloat(15, 20);
    }

    plant_marigold(object::scene& s): plant_base(s) {}
};

class plant_melonpult: public plant_base {
public:
    TRIVAL_INIT(plant, melonpult)
    plant_melonpult(object::scene& s): plant_base(s) {}
};

class plant_gatling_pea: public pea_base {
public:
    static const std::pair<float, float> PEA_OFFSETS[93];
    TRIVAL_INIT(pea, gatling_pea)
    plant_gatling_pea(object::scene& s): pea_base(s) {}
};

class plant_twin_sunflower: public sun_plant_base {
public:
    TRIVAL_INIT(plant, twin_sunflower)
    plant_twin_sunflower(object::scene& s): plant_base(s), sun_plant_base(s) {}
};

class plant_gloomshroom: public mushroom_base {
public:
    TRIVAL_INIT(mushroom, gloomshroom)
    plant_gloomshroom(object::scene& s): plant_base(s), mushroom_base(s) {}
};

class plant_cattail: public plant_base {
public:
    TRIVAL_INIT(plant, cattail)
    plant_cattail(object::scene& s): plant_base(s) {}
};

class plant_winter_melon: public plant_base {
public:
    TRIVAL_INIT(plant, winter_melon)
    plant_winter_melon(object::scene& s): plant_base(s) {}
};

class plant_spikerock : public plant_spikeweed {
public:
    void reduce_life(object::plant& p);
    TRIVAL_INIT(plant, spikerock)
    plant_spikerock(object::scene& s) : plant_spikeweed(s) {}
};

class plant_gold_magnet: public plant_base {
public:
    TRIVAL_INIT(plant, gold_magnet)
    plant_gold_magnet(object::scene& s): plant_base(s) {}
};

class plant_cob_cannon: public plant_base {
public:
    void update(object::plant& p);
    bool launch(object::plant& p, int x, int y);

    void init(object::plant& p, int row, int col, unsigned int countdown = 500);

    plant_cob_cannon(object::scene& s): plant_base(s) {}
};

class plant_imitater: public plant_base {
public:
    void update(object::plant& p);
    void init(
        object::plant& p,
        object::plant_type target,
        unsigned int row,
        unsigned int col);
    plant_imitater(object::scene& s): plant_base(s) {}
};

struct plant_subsystems {
    plant_base base;
    plant_pea_shooter pea_shooter;
    plant_sunflower sunflower;
    plant_cherry_bomb cherry_bomb;
    plant_wallnut wallnut;
    plant_potato_mine potato_mine;
    plant_snow_pea snow_pea;
    plant_chomper chomper;
    plant_repeater repeater;
    plant_puffshroom puffshroom;
    plant_sunshroom sunshroom;
    plant_fumeshroom fumeshroom;
    plant_grave_buster grave_buster;
    plant_hypnoshroom hypnoshroom;
    plant_scaredyshroom scaredyshroom;
    plant_iceshroom iceshroom;
    plant_doomshroom doomshroom;
    plant_lily_pad lily_pad;
    plant_squash squash;
    plant_threepeater threepeater;
    plant_tangle_kelp tangle_kelp;
    plant_jalapeno jalapeno;
    plant_spikeweed spickweed;
    plant_torchwood torchwood;
    plant_tallnut tallnut;
    plant_seashroom seashroom;
    plant_plantern plantern;
    plant_cactus cactus;
    plant_blover blover;
    plant_split_pea split_pea;
    plant_starfruit starfruit;
    plant_pumpkin pumpkin;
    plant_magnetshroom magnetshroom;
    plant_cabbagepult cabbagepult;
    plant_flower_pot flower_pot;
    plant_kernelpult kernelpult;
    plant_coffee_bean coffee_bean;
    plant_garlic garlic;
    plant_umbrella_leaf umbrella_leaf;
    plant_marigold marigold;
    plant_melonpult melonpult;
    plant_gatling_pea gatling_pea;
    plant_twin_sunflower twin_sunflower;
    plant_gloomshroom gloomshroom;
    plant_cattail cattail;
    plant_winter_melon winter_melon;
    plant_gold_magnet gold_magnet;
    plant_spikerock spikerock;
    plant_cob_cannon cob_cannon;
    plant_imitater imitater;

    plant_subsystems(object::scene& s):
        base(s),
        pea_shooter(s),
        sunflower(s),
        cherry_bomb(s),
        wallnut(s),
        potato_mine(s),
        snow_pea(s),
        chomper(s),
        repeater(s),
        puffshroom(s),
        sunshroom(s),
        fumeshroom(s),
        grave_buster(s),
        hypnoshroom(s),
        scaredyshroom(s),
        iceshroom(s),
        doomshroom(s),
        lily_pad(s),
        squash(s),
        threepeater(s),
        tangle_kelp(s),
        jalapeno(s),
        spickweed(s),
        torchwood(s),
        tallnut(s),
        seashroom(s),
        plantern(s),
        cactus(s),
        blover(s),
        split_pea(s),
        starfruit(s),
        pumpkin(s),
        magnetshroom(s),
        cabbagepult(s),
        flower_pot(s),
        kernelpult(s),
        coffee_bean(s),
        garlic(s),
        umbrella_leaf(s),
        marigold(s),
        melonpult(s),
        gatling_pea(s),
        twin_sunflower(s),
        gloomshroom(s),
        cattail(s),
        winter_melon(s),
        gold_magnet(s),
        spikerock(s),
        cob_cannon(s),
        imitater(s) {}
};

}

#undef TRIVAL_INIT
