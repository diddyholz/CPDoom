# CPDoom
A doom source port for the Classpad II (fx-CP400).

## Installation
Download the [latest .zip file]() and extract it into the root directory of your calculator. Your directory structure should then be as follows:
```
├── run.bin (Hollyhock launcher)
├── CPDoom.bin (Main CPDoom executable)
└── CPDoom/
    ├── bin/
    |   └── main.bin
    └── wad/
        ├── doom1.wad (The included shareware version of doom)
        └── ** Put other wads in here **
``` 

## Using other wads
To use other wads (such as a legaly acquired registered version), add them to the `/CPDoom/wad/` directory and rename your other wads to something like `doom1.wad.bak` so they do not get loaded.

Doom will search for wads in this order and selects the first it finds:

| Filename     | Game Version      |
|--------------|-------------------|
| doom2f.wad   | Doom 2 (French)   |
| doom2.wad    | Doom 2            |
| plutonia.wad | Plutonia          |
| tnt.wad      | TNT: Evilution    |
| doomu.wad    | Ultimate Doom     |
| doom.wad     | Doom (Registered) |
| doom1.wad    | Doom (Shareware)  |

## Controls
Default Controls are as follows but can be changed in Options > Controls.

### Ingame

| Key     | Action          |
|---------|-----------------|
| [8]     | Forward         |
| [5]     | Backward        |
| [6]     | Turn Right      |
| [4]     | Turn Left       |
| [9]     | Strafe Right    |
| [7]     | Strafe Left     |
| [+]     | Sprint          |
| [EXE]   | Fire            |
| [-]     | Use             |
| [3]     | Next Weapon     |
| [1]     | Previous Weapon |
| [Shift] | Map             |

### Menu

| Key         | Action    |
|-------------|-----------|
| [Keyboard]  | Open Menu |
| [Backspace] | Go Back   |
| [Clear]     | Enter     |
| [Up]        | Up        |
| [Down]      | Down      |
| [Left]      | Left      |
| [Right]     | Right     |
| [y]         | y         |
| [x]         | n         |
