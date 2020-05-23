# PvZ-Emulator
Plants vs. Zombies Survival Endless emulator.

This emulator can emulate all kinds of plants and zombies except Dr. Zomboss
and Zombie Bobsled Team (They don't occur in Survival Endless). It supports
all 6 scenes (Day, Night, Pool, Fog, Roof, and Moon Night).

## Usage
This emulator is provided as a static library. Include `pvz-emulator/world.h`
before to your source code. All functions and methods are thread-safe.

### Create and Update a Game Instance
```cpp
int main(void) {
    pvz_emulator::world w(pvz_emulator::object::scene_type::pool);

    // Ths first wave of zombies will spawn after 6 seconds.
    for (int i = 0; i < 600; i++) {
        w.update();
    }

    return 0;
}
```

### Control Flags
You can set some bool flags in the `scene` objects to enable or disable some
easter eggs and bugs that affect motions of objects.
* `is_zombie_dance` Enable the "dance" effect if set to `true`.
* `is_future_enalbed` Enable the "future" effect if set to `true`.
* `stop_spawn` Stop spawning zombies if set to `true`.
* `enable_split_pea_bug` Enable the "future" effect if set to `true`.

## Debugging
I wrote a simple web debugger to visualize the game state. To compile the
debugger, you need to [install node.js](https://nodejs.org/en/download/) and
[extract](https://plantsvszombies.fandom.com/wiki/Modify_Plants_vs._Zombies)
some images from `main.pak` in the game installation directory.

After unpacked `main.pak`, copy these files to `debugger/src/assets`:
```console
$ cd images
$ cp background1.jpg PATH_TO_THE_SOURCE_CODE/debugger/src/assets/day.jpg
$ cp background2.jpg PATH_TO_THE_SOURCE_CODE/debugger/src/assets/night.jpg
$ cp background3.jpg PATH_TO_THE_SOURCE_CODE/debugger/src/assets/pool.jpg
$ cp background4.jpg PATH_TO_THE_SOURCE_CODE/debugger/src/assets/fog.jpg
$ cp background5.jpg PATH_TO_THE_SOURCE_CODE/debugger/src/assets/roof.jpg
$ cp background6.jpg PATH_TO_THE_SOURCE_CODE/debugger/src/assets/moon_night.jpg
```

Then you can build the debug server:
```console
$ cmake PATH_TO_THE_SOURCE_CODE -DPVZEMU_BUILD_DEBUGGER=True
$ make
```

Run the debug server:
```console
$ ./debug-server PATH_TO_THE_SOURCE_CODE/debugger/dist
```

Now you can Open http://localhost:3000 on your browser.