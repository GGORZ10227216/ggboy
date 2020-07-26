# ggboy
Ggboy is a gameboy emulator written in C++.

## Features

* CPU emulation
* LCD Controller
* Joypad
* Timer
* Sound(Using Blargg's Audio Libraries)
* MBC1 mapper

# Build
```
sudo apt install libsdl2-dev
git clone https://github.com/GGORZ10227216/ggboy.git
cd ./ggboy/
mkdir build && cd build
cmake ../
make
```

# Usage
```
./ggboy ROM_FILE
```

# Control
```
Button A --> Z
Button B --> X
Up       --> Up arrow key
Down     --> Down arrow key
Left     --> Left arrow key
Right    --> Right arrow key
Start    --> Enter
Select   --> Right Shift
```
# Screenshot
<img src="https://i.imgur.com/evMwnIU.png" alt="Zelda-00" width="200"/> <img src="https://i.imgur.com/jOqoGkA.png" alt="Zelda-00" width="200"/>
<img src="https://i.imgur.com/eNzoQeI.png" alt="Zelda-00" width="200"/> <img src="https://i.imgur.com/aKQZpgy.png" alt="Zelda-00" width="200"/>

<img src="https://i.imgur.com/ELlxsFR.png" alt="Kirby-00" width="200"/> <img src="https://i.imgur.com/epeTpdz.png" alt="Kirby-00" width="200"/>
<img src="https://i.imgur.com/b3edZYh.png" alt="Kirby-00" width="200"/> <img src="https://i.imgur.com/09XQS4Q.png" alt="Kirby-00" width="200"/>

# Resources
* [Awesome-gbdev](https://github.com/gbdev/awesome-gbdev)
* [Gameboy opcodes](http://pastraiser.com/cpu/gameboy/gameboy_opcodes.html)
* [BGB emulator](http://bgb.bircd.org/)
* [Test roms](https://github.com/retrio/gb-test-roms)
