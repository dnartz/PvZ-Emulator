import { Reanim } from './Reanim';

export type ZombieType =
    "none" |
    "zombie" |
    "flag" |
    "conehead" |
    "pole_vaulting" |
    "buckethead" |
    "newspaper" |
    "screendoor" |
    "football" |
    "dancing" |
    "backup_dancer" |
    "ducky_tube" |
    "snorkel" |
    "zomboni" |
    "dolphin_rider" |
    "jack_in_the_box" |
    "balloon" |
    "digger" |
    "pogo" |
    "yeti" |
    "bungee" |
    "ladder" |
    "catapult" |
    "gargantuar" |
    "imp" |
    "giga_gargantuar";

export type ZombieStatus =
    "walking" |
    "dying" |
    "dying_from_instant_kill" |
    "dying_from_lawnmower" |
    "bungee_target_drop" |
    "bungee_body_drop" |
    "bungee_idle_after_drop" |
    "bungee_grab" |
    "bungee_raise" |
    "bungee_idle" |
    "pole_valuting_running" |
    "pole_valuting_jumpping" |
    "pole_vaulting_walking" |
    "rising_from_ground" |
    "jackbox_walking" |
    "jackbox_pop" |
    "pogo_with_stick" |
    "pogo_idle_before_target" |
    "pogo_jump_across" |
    "newspaper_walking" |
    "newspaper_destoryed" |
    "newspaper_running" |
    "digger_dig" |
    "digger_drill" |
    "digger_lost_dig" |
    "digger_landing" |
    "digger_dizzy" |
    "digger_walk_right" |
    "digger_walk_left" |
    "digger_idle" |
    "dancing_moonwalk" |
    "dancing_point" |
    "dancing_wait_summoning" |
    "dancing_summoning" |
    "dancing_walking" |
    "dancing_armrise1" |
    "dancing_armrise2" |
    "dancing_armrise3" |
    "dancing_armrise4" |
    "dancing_armrise5" |
    "backup_spawning" |
    "dophin_walk_with_dophin" |
    "dophin_jump_in_pool" |
    "dophin_ride" |
    "dophin_jump" |
    "dophin_walk_in_pool" |
    "dophin_walk_without_dophin" |
    "snorkel_walking" |
    "snorkel_jump_in_the_pool" |
    "snorkel_swim" |
    "snorkel_up_to_eat" |
    "snorkel_eat" |
    "snorkel_finied_eat" |
    "catapult_shoot" |
    "catapult_idle" |
    "balloon_flying" |
    "balloon_falling" |
    "balloon_walking" |
    "imp_flying" |
    "imp_landing" |
    "gargantuar_throw" |
    "gargantuar_smash" |
    "ladder_walking" |
    "ladder_placing" |
    "yeti_escape";

export type ZombieAccessoriesType1 =
    "none" |
    "roadcone" |
    "bucket" |
    "football_cap" |
    "miner_hat";

export type ZombieAccessoriesType2 =
    "none" |
    "screen_door" |
    "newspaper" |
    "ladder";

export interface Zombie {
    id: number,
    type: ZombieType,
    status: ZombieStatus,
    int_x: number,
    int_y: number,
    x: number,
    y: number,
    dx: number,
    dy: number,
    d2y: number,
    reanim: Reanim,
    row: number,
    bungee_col: number,
    ladder_col: number,
    hit_box: {
        x: number,
        y: number,
        width: number,
        height: number,
    },
    attack_box: {
        x: number,
        y: number,
        width: number,
        height: number
    },
    countdown: {
        butter: number,
        freeze: number,
        slow: number,
        action: number,
        dead: number
    },
    spawn_wave: number,
    time_since_ate_garlic: number,
    time_since_spawn: number,
    hp: number,
    max_hp: number,
    accessory_1: {
        type: ZombieAccessoriesType1,
        hp: number,
        max_hp: number
    },
    accessory_2: {
        type: ZombieAccessoriesType2,
        hp: number,
        max_hp: number
    },
    master_id: number | null,
    partners: [number, number, number, number],
    garlic_tick: {
        a: number,
        b: number,
        c: number
    },
    bungee_target: number | null,
    is_eating: boolean,
    is_dead: boolean,
    is_blown: boolean,
    is_not_dying: boolean,
    is_hypno: boolean,
    has_item_or_walk_left: boolean,
    is_in_water: boolean,
    has_balloon: boolean,
    has_eaten_garlic: boolean,

    n_basketballs?: number,
    summon_countdown?: number
}