import { Reanim } from "./Reanim";

export type PlantType =
    "none" |
    "pea_shooter" |
    "sunflower" |
    "cherry_bomb" |
    "wallnut" |
    "potato_mine" |
    "snow_pea" |
    "chomper" |
    "repeater" |
    "puffshroom" |
    "sunshroom" |
    "fumeshroom" |
    "grave_buster" |
    "hypnoshroom" |
    "scaredyshroom" |
    "iceshroom" |
    "doomshroom" |
    "lily_pad" |
    "squash" |
    "threepeater" |
    "tangle_kelp" |
    "jalapeno" |
    "spikeweed" |
    "torchwood" |
    "tallnut" |
    "seashroom" |
    "plantern" |
    "cactus" |
    "blover" |
    "split_pea" |
    "starfruit" |
    "pumpkin" |
    "magnetshroom" |
    "cabbagepult" |
    "flower_pot" |
    "kernelpult" |
    "coffee_bean" |
    "garlic" |
    "umbrella_leaf" |
    "marigold" |
    "melonpult" |
    "gatling_pea" |
    "twin_sunflower" |
    "gloomshroom" |
    "cattail" |
    "winter_melon" |
    "gold_magnet" |
    "spikerock" |
    "cob_cannon" |
    "imitater";

export type PlantStatus =
    "idle" |
    "wait" |
    "work" |
    "squash_look" |
    "squash_jump_up" |
    "squash_stop_in_the_air" |
    "squash_jump_down" |
    "squash_crushed" |
    "grave_buster_land" |
    "grave_buster_idle" |
    "chomper_bite_begin" |
    "chomper_bite_success" |
    "chomper_bite_fail" |
    "chomper_chew" |
    "chomper_swallow" |
    "potato_sprout_out" |
    "potato_armed" |
    "spike_attack" |
    "scaredyshroom_scared" |
    "scaredyshroom_scared_idle" |
    "scaredyshroom_grow" |
    "sunshroom_small" |
    "sunshroom_grow" |
    "sunshroom_big" |
    "magnetshroom_working" |
    "magnetshroom_inactive_idle" |
    "cactus_short_idle" |
    "cactus_grow_tall" |
    "cactus_tall_idle" |
    "cactus_get_short" |
    "tangle_kelp_grab" |
    "cob_cannon_unaramed_idle" |
    "cob_cannon_charge" |
    "cob_cannon_launch" |
    "cob_cannon_aramed_idle" |
    "kernelpult_launch_butter" |
    "umbrella_leaf_block" |
    "umbrella_leaf_shrink" |
    "imitater_explode" |
    "flower_pot_placed" |
    "lily_pad_placed";

export type PlantEdibleType =
    "visible_and_edible" |
    "invisible_and_edible" |
    "invisible_and_not_edible";

export interface Plant {
    id: number,
    type: PlantType,
    status: PlantStatus,
    x: number,
    y: number,
    cannon: {
        x: number,
        y: number
    },
    hp: number,
    max_hp: number,
    hit_box: {
        x: number,
        y: number,
        width: number,
        height: number
    },
    attack_box: {
        x: number,
        y: number,
        width: number,
        height: number
    },
    row: 0 | 1 | 2 | 3 | 4 | 5,
    col: 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8,
    max_boot_delay: number,
    direction: "right" | "left",
    target: number | null,
    imitater_target: PlantType,
    countdown: {
        status: number,
        generate: number,
        launch: number,
        eaten: number,
        awake: number,
        effect: number,
        dead: number,
        blover_disappear: number
    },
    reanim: Reanim,
    edible: PlantEdibleType,
    threepeater_time_since_first_shot: number,
    split_pea_attack_flags: {
        front: boolean,
        back: boolean
    },
    is_dead: boolean,
    is_smashed: boolean,
    is_sleeping: boolean,
    can_attack: boolean
}

export interface Card {
    type: PlantType,
    imitater_type: PlantType,
    cold_down: number
}