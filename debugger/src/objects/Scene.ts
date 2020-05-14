import { Zombie, ZombieType } from "./Zombie";
import { Plant, Card } from "./Plant";
import { Griditem } from "./Griditem";
import { Projectile } from "./Projectile";

export type SceneType = "day" | "night" | "fog" | "pool" | "roof" | "moon_night";

export interface Scene {
    type: SceneType,
    rows: 5 | 6,
    zombies: Zombie[],
    plants: Plant[],
    griditems: Griditem[],
    projectiles: Projectile[],
    spawn: {
        spawn_list: ZombieType[],
        wave: 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20,
        hp: {
            initial: number,
            threshold: number,
        },
        countdown: {
            next_wave: number,
            next_wave_initial: number,
            lurking_squad: number,
            hugewave_fade: number,
            endgame: number,
        },
        spawn_flags: {
            zombie: boolean,
            flag: boolean,
            conehead: boolean,
            pole_vaulting: boolean,
            buckethead: boolean,
            newspaper: boolean,
            screendoor: boolean,
            football: boolean,
            dancing: boolean,
            backup_dancer: boolean,
            ducky_tube: boolean,
            snorkel: boolean,
            zomboni: boolean,
            dolphin_rider: boolean,
            jack_in_the_box: boolean,
            balloon: boolean,
            digger: boolean,
            pogo: boolean,
            yeti: boolean,
            bungee: boolean,
            ladder: boolean,
            catapult: boolean,
            gargantuar: boolean,
            imp: boolean,
            giga_gargantuar: boolean
        },
        is_hugewave_shown: false
    },
    sun: {
        sun: number,
        natural_sun_generated: number,
        natural_sun_countdown: number
    },
    ice_path: {
        countdown: number[],
        x: number[]
    },
    cards: Card[],
    is_zombie_dance: boolean,
    is_future_enabled: boolean,
    is_game_over: boolean,
    stop_spawn: boolean
}