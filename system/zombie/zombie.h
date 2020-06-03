#pragma once
#include <array>
#include <utility>
#include <unordered_map>

#include "object/scene.h"
#include "object/zombie.h"
#include "system/util.h"
#include "system/rng.h"
#include "system/reanim.h"
#include "system/projectile/projectile_factory.h"

namespace pvz_emulator::system {

class zombie_base {
protected:
    object::scene& scene;

    system::reanim reanim;
    system::rng rng;

    static constexpr unsigned int GARLIC_MOVE_TIME = 170;

    void set_common_fields(object::zombie &z);

public:
    bool can_attack_plant(
        object::zombie& z,
        object::plant& p,
        zombie_attack_type type);

    object::plant* find_target(object::zombie &z, zombie_attack_type type);

    void init(object::zombie& z, object::zombie_type type, unsigned int row);

    long double predict_after(object::zombie& z, float cs);

    zombie_base(object::scene& s) : scene(s), reanim(s), rng(s) {}
};

class common_zombie : public zombie_base {
public:
    static const std::array<float, 504> _ground;

    void init(object::zombie &z, object::zombie_type type, unsigned int row);
    common_zombie(object::scene& s) : zombie_base(s) {}
};

class zombie_yeti : public zombie_base {
public:
    static const std::array<float, 105> _ground;

    void update(object::zombie& z);
    void init(object::zombie& z, unsigned int row);
    zombie_yeti(object::scene& s) : zombie_base(s) {}
};

class zombie_ladder : public zombie_base {
public:
    static const std::array<float, 215> _ground;

    void update(object::zombie& z);
    void init(object::zombie &z, unsigned int row);
    zombie_ladder(object::scene& s) : zombie_base(s) {}
};

class zombie_bungee : public zombie_base {
    void select_target(object::zombie &z);
    object::plant* find_umbrella(object::zombie& z);

public:
    void update(object::zombie& z);

    void init(object::zombie &z, unsigned int row);
    zombie_bungee(object::scene& s) : zombie_base(s) {}
};

class zombie_football : public zombie_base {
public:
    static const std::array<float, 105> _ground;

    void init(object::zombie &z, unsigned int row);
    zombie_football(object::scene& s) : zombie_base(s) {}
};

class zombie_digger : public zombie_base {
private:
    void update_dy(object::zombie& z);

public:
    static const std::array<float, 200> _ground;

    void update(object::zombie &z);
    void init(object::zombie &z, unsigned int row);
    zombie_digger(object::scene& s) : zombie_base(s) {}
};


class zombie_pole_vaulting : public zombie_base {
public:
    static const std::array<float, 193> _ground;

    void init(object::zombie &z, unsigned int row);
    void update(object::zombie &z);

    zombie_pole_vaulting(object::scene& s) : zombie_base(s) {}
};

class zombie_dolphin_rider : public zombie_base {
    void set_attrs_after_drop_dolphin(object::zombie &z, float dy);

public:
    static const std::array<float, 289> _ground;

    void update(object::zombie &z);
    void init(object::zombie &z, unsigned int row);
    zombie_dolphin_rider(object::scene& s) :
        zombie_base(s) {}
};

class zombie_gargantuar : public zombie_base {
    void set_walk(object::zombie& z);

public:
    static const std::array<float, 197> _ground;

    void update(object::zombie& z);
    void init(object::zombie& z, object::zombie_type type, unsigned int row);
    zombie_gargantuar(object::scene& s) : zombie_base(s) {}
};

class zombie_zomboni : public zombie_base {
public:
    void update(object::zombie& z);
    void init(object::zombie &z, unsigned int row);
    zombie_zomboni(object::scene& s) : zombie_base(s) {}
};

class zombie_catapult : public zombie_base {
    system::projectile_factory projectile_factory;

    object::plant* find_target(object::zombie& z);

public:
    void init(object::zombie& z, unsigned int row);
    void update(object::zombie& z);

    zombie_catapult(object::scene& s) :
        zombie_base(s),
        projectile_factory(s) {}
};

class zombie_snorkel : public zombie_base {
public:
    static const std::array<float, 249> _ground;

    void update(object::zombie &z);
    void init(object::zombie &z, unsigned int row);
    zombie_snorkel(object::scene& s) : zombie_base(s) {}
};

class zombie_jack_in_the_box : public zombie_base {
private:
    void kill_plants(object::zombie& z, int x, int y);
public:
    static const std::array<float, 103> _ground;

    void update(object::zombie& z);
    void init(object::zombie& z, unsigned int row);
    zombie_jack_in_the_box(object::scene& s) : zombie_base(s) {}
};

class zombie_pogo : public zombie_base {
public:
    static const std::array<float, 166> _ground;

    void remove_stick(object::zombie& z);
    void update(object::zombie& z);
    void init(object::zombie &z, unsigned int row);
    zombie_pogo(object::scene& s) : zombie_base(s) {}
};

class zombie_newspaper : public zombie_base {
public:
    static const std::array<float, 216> _ground;

    void update(object::zombie &z);
    void init(object::zombie &z, unsigned int row);
    zombie_newspaper(object::scene& s) : zombie_base(s) {}
};

class zombie_balloon : public zombie_base {
public:
    static const std::array<float, 155> _ground;

    void update(object::zombie &z);
    void init(object::zombie &z, unsigned int row);
    zombie_balloon(object::scene& s) : zombie_base(s) {}
};

class zombie_dancer_base : public zombie_base {
protected:
    unsigned int get_clock(object::zombie &z);
    object::zombie_status get_status_by_clock(object::zombie& z);

    zombie_dancer_base(object::scene& s): zombie_base(s) {}
};

class zombie_dancing : public zombie_dancer_base {
private:
    bool need_summon(object::zombie& z);
    void create_backup_dancers(object::zombie& z);

public:
    static const std::array<float, 147> _ground;

    void update(object::zombie& z);
    void init(object::zombie& z, unsigned int row);
    zombie_dancing(object::scene& s) : zombie_dancer_base(s) {}
};

class zombie_backup_dancer : public zombie_dancer_base {
public:
    static const std::array<float, 100> _ground;

    void update(object::zombie& z);

    void init(object::zombie &z, unsigned int row);
    zombie_backup_dancer(object::scene& s) : zombie_dancer_base(s) {}
};

class zombie_imp : public zombie_base {
public:
    static const std::array<float, 104> _ground;

    void update(object::zombie& z);

    void init(object::zombie &z, unsigned int row);
    zombie_imp(object::scene& s) : zombie_base(s) {}
};

struct zombie_subsystems {
    common_zombie zombie;
    zombie_pole_vaulting pole_vaulting;
    zombie_newspaper newspaper;
    zombie_football football;
    zombie_dancing dancing;
    zombie_backup_dancer backup_dancer;
    zombie_snorkel snorkel;
    zombie_zomboni zomboni;
    zombie_dolphin_rider dolphin_rider;
    zombie_jack_in_the_box jack_in_the_box;
    zombie_balloon balloon;
    zombie_digger digger;
    zombie_pogo pogo;
    zombie_yeti yeti;
    zombie_bungee bungee;
    zombie_ladder ladder;
    zombie_catapult catapult;
    zombie_gargantuar gargantuar;
    zombie_imp imp;

    zombie_subsystems(object::scene& s):
    zombie(s),
    pole_vaulting(s),
    newspaper(s),
    football(s),
    dancing(s),
    backup_dancer(s),
    snorkel(s),
    zomboni(s),
    dolphin_rider(s),
    jack_in_the_box(s),
    balloon(s),
    digger(s),
    pogo(s),
    yeti(s),
    bungee(s),
    ladder(s),
    catapult(s),
    gargantuar(s),
    imp(s)
    {}
};

}
