#include <iostream>

#include "emulator.h"

int main(int argc, char * argv[])
{

    chip8::Emulator e{15, "./roms/Space Invaders [David Winter].ch8"};
    // chip8::Emulator e{15, "./roms/Brick.ch8"};

    e.run();

}
